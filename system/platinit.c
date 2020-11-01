/* platinit.c - platinit */
#include <xinu.h>

/*------------------------------------------------------------------
 * console_init - initialize the serial console.  The serial console
 * is on the second memory-mapped 16550 UART device.
 *------------------------------------------------------------------
 */
int console_init(void)
{
	return OK;
}

/*------------------------------------------------------------------
 * platinit - platform specific initialization for Galileo
 *------------------------------------------------------------------
 */
void	platinit()
{

	/* Initialize the console serial port */
	console_init();

}
