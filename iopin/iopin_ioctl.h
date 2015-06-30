#ifndef _IOPIN_IOCTL_H_
#define _IOPIN_IOCTL_H_

#define  IOPIN_IOCTL_IDENTIFIER     'G'

/*
 * Set the pin function (0 = input and 1 = output)
 * Any value diferent from 0 and 1 shall return EINVAL
 */
#define  IOCTL_SET_FUNCTION         _IOW( IOPIN_IOCTL_IDENTIFIER, 0, ulong )
#define  PIN_FUNCTION_INPUT         0     // Set pin as input
#define  PIN_FUNCTION_OUTPUT        1     // Set pin as output

#endif