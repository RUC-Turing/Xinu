/* stdio.h - definintions and constants for standard I/O functions */

#include "syscalls.h"

/* Prototypes for formatted input functions */

extern	int32	_doscan(char *,int32 *, int32 (*)(void),
			int32 (*)(char), int32, int32);
extern	int32	sscanf(char *, char *, int32);
extern	int32	fscanf(int32, char *, int32);
#define	scanf(fmt, args)	fscanf(CONSOLE, fmt, args)

#define syscall_fscanf(...) \
		do_generic_syscall(int32, SYSCALL_FSCANF, __VA_ARGS__)
#define syscall_scanf(...) syscall_fscanf(CONSOLE, __VA_ARGS__)

/* Definintion of standard input/ouput/error used with shell commands */

#define	stdin	((proctab[currpid]).prdesc[0])
#define	stdout	((proctab[currpid]).prdesc[1])
#define	stderr	((proctab[currpid]).prdesc[2])


/* Prototypes for formatted output functions */

extern	int32	fprintf(int, char *, ...);
extern	int32	printf(const char *, ...);
extern	int32	sprintf(char *, char *, ...);

#define syscall_fprintf(...) \
		do_generic_syscall(int32, SYSCALL_FPRINTF, __VA_ARGS__)
#define syscall_printf(...) \
		do_generic_syscall(int32, SYSCALL_PRINTF, __VA_ARGS__)

/* Prototypes for character input and output functions */

extern	int32	fgetc(int);
extern	char	*fgets(char *, int32, int32);
extern	int32	fputc(int32, int32);
extern	int32	fputs(char *, int32);
extern	int32	putchar(int32 c);
extern	int32	getchar(void);
