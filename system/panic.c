/* panic.c - panic */

#include <xinu.h>

/*------------------------------------------------------------------------
 * panic  -  Display a message and stop all processing
 *------------------------------------------------------------------------
 */
void	panic (
	const char	*msg			/* Message to display		*/
	)
{
	disable();			/* Disable interrupts		*/
	kprintf("\n\n\rpanic: %s\n\n", msg);
	while(TRUE) {;}			/* Busy loop forever		*/
}
