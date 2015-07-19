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

#define  IOCTL_SET_INTERRUPTION     _IOW( IOPIN_IOCTL_IDENTIFIER, 1, ulong )
#define  PIN_INTERRUPTION_RISING    0x00000001     // Set interruption to rising edge
#define  PIN_INTERRUPTION_FALLING   0x00000002     // Set interruption to falling edge
#define  PIN_INTERRUPTION_HIGH      0x00000004     // Set interruption to high state detected
#define  PIN_INTERRUPTION_LOW       0x00000008     // Set interruption to low state detected

#define  IOCTL_SET_PULL             _IOW( IOPIN_IOCTL_IDENTIFIER, 2, ulong )
#define  PIN_PULL_OFF               0
#define  PIN_PULL_DOWN              1
#define  PIN_PULL_UP                2

#endif
