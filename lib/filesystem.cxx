#include <xinu.h>

#include <elf.h>

struct __attribute__((packed)) PartitionTableEntry
{
    uint8 attr;
    unsigned chs_start : 24;
    uint8 type;
    unsigned chs_end : 24;
    uint32 lba_start;
    uint32 sector_count;
};

struct __attribute__((packed)) MasterBootRecord
{
    char bootstrap[440];
    uint32 disk_id;
    uint16 reserved;
    PartitionTableEntry parts[4];
};

struct __attribute__((packed)) PartitionBootRecord
{
    uint8 jmp_short[3];
    uint8 oem[8];
    uint16 bytes_per_sector;
    uint8 sectors_per_cluster;
    uint16 reserved_sectors;
    uint8 fat_num;
    uint16 root_directory_entry_num;
    uint16 total_sectors;
    uint8 media_descriptor_type;
    uint16 sectors_per_fat;
    uint16 sectors_per_track;
    uint16 heads;
    uint32 hidden_sectors;
    uint32 large_total_sectors;
};

struct __attribute__((packed)) Fat16ExtBS
{
    // extended fat12 and fat16 stuff
    unsigned char bios_drive_num;
    unsigned char reserved1;
    unsigned char boot_signature;
    unsigned int volume_id;
    unsigned char volume_label[11];
    unsigned char fat_type_label[8];
};

struct FSInfo
{
    PartitionBootRecord pbr;
    Fat16ExtBS feb;
    uint32 total_sectors;
    uint32 fat_size;
    uint32 root_dir_sectors;
    uint32 first_data_sector;
    uint32 first_fat_sector;
    uint32 data_sectors;
    uint32 total_clusters;
};

struct __attribute__((packed)) FileEntry
{
    enum
    {
        READ_ONLY = 0x01,
        HIDDEN = 0x02,
        SYSTEM = 0x04,
        VOLUME_ID = 0x08,
        DIRECTORY = 0x10,
        ARCHIVE = 0x20,
        LFN = READ_ONLY | HIDDEN | SYSTEM | VOLUME_ID
    };
    char filename[8];
    char extname[3];
    uint8 attr;
    uint8 reserved;
    uint8 create_time_milli_seconds;
    uint16 create_time;
    uint16 create_date;
    uint16 access_date;
    uint16 cluster_high;
    uint16 modify_date;
    uint16 modify_date1;
    uint16 cluster_low;
    uint32 size;
};

enum
{
    CLUSTER_END = 0xFFF8
};

extern "C"
{

uint32 read_file(const char* filename, char *buffer, int bsize)
{
    int part_start_lba = 0;
    MasterBootRecord mbr;

    seek(DISK, 0);
    read(DISK, (char *)&mbr, sizeof mbr);

    for (int i = 0; i < 4; i++)
        if(mbr.parts[i].lba_start != 0)
        {
            part_start_lba = mbr.parts[i].lba_start;
            break;
        }

    FSInfo info;
    char sector_buffer[512];
    memset(&info, 0, sizeof info);

    seek(DISK, 512 * part_start_lba);
    read(DISK, sector_buffer, 512);

    info.pbr = *(PartitionBootRecord *)sector_buffer;
    info.feb = *(Fat16ExtBS *)((PartitionBootRecord *)sector_buffer + 1);

    assert(info.pbr.bytes_per_sector == 512);
    assert(info.feb.boot_signature == 0x28 || info.feb.boot_signature == 0x29);
    assert(info.pbr.sectors_per_fat);

    info.total_sectors = (info.pbr.total_sectors == 0) ? info.pbr.large_total_sectors : info.pbr.total_sectors;
    info.fat_size = info.pbr.sectors_per_fat;
    assert(info.fat_size);
    info.root_dir_sectors = ((info.pbr.root_directory_entry_num * 32) + (info.pbr.bytes_per_sector - 1)) / info.pbr.bytes_per_sector;
    info.first_data_sector = info.pbr.hidden_sectors + (info.pbr.fat_num * info.fat_size) + info.root_dir_sectors;
    info.first_fat_sector = info.pbr.hidden_sectors;
    info.data_sectors = info.total_sectors - (info.pbr.hidden_sectors + (info.pbr.sectors_per_fat * info.fat_size) + info.root_dir_sectors);
    info.total_clusters = info.data_sectors / info.pbr.sectors_per_cluster;

    assert(info.total_clusters >= 4085 && info.total_clusters < 65525);

    // fat_file: info.first_fat_sector ~ info.first_fat_sector + info.fat_size
    // root: info.first_data_sector - info.root_dir_sectors ~ info.first_data_sector

    uint32 file_size = 0;
    uint32 start_cluster = 0;

    seek(DISK, 512 * part_start_lba + (info.first_data_sector - info.root_dir_sectors) * 512);
    FileEntry entry;
    for (int s = 0; s < info.root_dir_sectors * 512 / sizeof(FileEntry); s++)
    {
        read(DISK, (char *)&entry, sizeof(entry));

        if (*(char *)&entry == '\0')
            break;

        if (!(entry.attr & FileEntry::VOLUME_ID || entry.attr & FileEntry::HIDDEN || entry.filename[0] == '\xe5' /* deleted */))
        {
            int now = 0;
            char now_filename[15];
            for (int i = 0; i < 8 && entry.filename[i] != ' '; i++)
                now_filename[now++] = entry.filename[i];
            now_filename[now++] = '.';
            for (int i = 0; i < 3 && entry.extname[i] != ' '; i++)
                now_filename[now++] = entry.extname[i];
            now_filename[now++] = 0;
            
            if (strncmp(filename, now_filename, 15) == 0)
            {
                file_size = entry.size;
                start_cluster = entry.cluster_low | (entry.cluster_high << 16);
                break;
            }
        }
    }
    
    uint32 bytes_to_read = file_size;
    if (bsize < file_size)
        bytes_to_read = bsize;
    
    uint32 now_ind = 0;
    
    uint32 now_cluster = start_cluster;
    
    while (now_ind != bytes_to_read && now_cluster != CLUSTER_END)
    {
        uint32 cluster_sector_start = ((now_cluster - 2) * info.pbr.sectors_per_cluster) + info.first_data_sector;
        seek(DISK, 512 * part_start_lba + 512 * cluster_sector_start);
        for (int i = 0; bytes_to_read && i < info.pbr.sectors_per_cluster; i++)
        {
            for (int j = 0; j < 512 && now_ind != bytes_to_read; j++, now_ind++)
            {
                buffer[now_ind] = getc(DISK);
            }
        }

        uint32 cluster_sector = now_cluster / 256 + info.first_fat_sector;
        seek(DISK, (part_start_lba + cluster_sector) * 512 + now_cluster % 256 * 2);
        read(DISK, (char *)&now_cluster, 2);
    }

    return file_size;
}

uint32 get_elf_entrypoint(const char* elf)
{
    int num = 0;

    const elf32_hdr *hdr = (const elf32_hdr *)elf;

    const elf32_phdr *phdr = (const elf32_phdr *)(elf + hdr->e_phoff);

    for (int i = 0; i < hdr->e_phoff; i++)
    {
        if (phdr[i].p_type == PT_LOAD)
        {
            if (hdr->e_entry >= phdr[i].p_paddr && hdr->e_entry < phdr[i].p_paddr + phdr[i].p_memsz)
            return phdr[i].p_offset + hdr->e_entry;
            num++;
        }
    }

    return 0;
}

}
