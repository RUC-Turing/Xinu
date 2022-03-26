/* exit.c - exit */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  exit  -  Cause the calling process to exit
 *------------------------------------------------------------------------
 */
void	exit(void)
{
	syscall_kill(syscall_getpid());		/* Kill the current process */
}
