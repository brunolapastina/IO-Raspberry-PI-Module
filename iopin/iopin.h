#ifndef _IOPIN_H_
#define _IOPIN_H_

struct SIOPinDev
{
	struct cdev stCdev;
   int   iMinor;
   ulong ulPin;
};

struct SGpioRegistersMap
{
   uint32_t GPFSEL[6];     // [R/W] GPIO Function Select
   uint32_t Reserved1;
   uint32_t GPSET[2];      // [W]   GPIO Pin Output Set
   uint32_t Reserved2;
   uint32_t GPCLR[2];      // [W]   GPIO Pin Output Clear
   uint32_t Reserved3;
   uint32_t GPLEV[2];      // [R]   GPIO Pin Level
   uint32_t Reserved4;
   uint32_t GPEDS[2];      // [R/W] GPIO Pin Event Detect Status
   uint32_t Reserved5;
   uint32_t GPREN[2];      // [R/W] GPIO Pin Rising Edge Detect Enable
   uint32_t Reserved6;
   uint32_t GPFEN[2];      // [R/W] GPIO Pin Falling Edge Detect Enable
   uint32_t Reserved7;
   uint32_t GPHEN[2];      // [R/W] GPIO Pin High Detect Enable
   uint32_t Reserved8;
   uint32_t GPLEN[2];      // [R/W] GPIO Pin Low Detect Enable
   uint32_t Reserved9;
   uint32_t GPAREN[2];     // [R/W] GPIO Pin Async. Rising Edge Detect
   uint32_t Reserved10;
   uint32_t GPAFEN[2];     // [R/W] GPIO Pin Async. Falling Edge Detect
   uint32_t Reserved11;
   uint32_t GPPUD;         // [R/W] GPIO Pin Pull-up/down Enable
   uint32_t GPPUDCLK[2];   // [R/W] GPIO Pin Pull-up/down Enable Clock
};

static int ContructDevice( struct SIOPinDev* pobjDev, int iMinor, struct class* pobjClass );

int iopin_open(struct inode *inode, struct file *filp);
int iopin_release(struct inode *inode, struct file *filp);
long iopin_ioctl( struct file* filp, unsigned int ioctl_num, unsigned long ioctl_param );
ssize_t iopin_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t iopin_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

#endif