#include <xinu.h>

extern "C"
{

devcall	diskread(
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char	*buff,			/* Buffer of characters		*/
	  int32	count 			/* Count of character to read	*/
	)
{
    // TODO
}

devcall	diskgetc(
	  struct dentry	*devptr		/* Entry in device switch table	*/
    )
{
    // TODO
}

devcall	diskseek (
	  struct dentry *devptr,	/* Entry in device switch table */
	  uint32	offset		/* Byte position in the file	*/
	)
{
    // TODO
}

}
