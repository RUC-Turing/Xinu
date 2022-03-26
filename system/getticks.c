/* getticks.c - getticks */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getticks  -  Retrieve the number of clock ticks since CPU reset
 *------------------------------------------------------------------------
 */
void  	getticks(uint64 *pret)
{
	uint64 ret;
	asm volatile ( "rdtsc" : "=A"(ret) );
	*pret = ret;
}
