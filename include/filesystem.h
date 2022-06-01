#ifndef _filesystem_h

#define _filesystem_h

uint32 read_file(const char* filename, char *buffer, int size);
uint32 get_elf_entrypoint(const char* elf);

#endif
