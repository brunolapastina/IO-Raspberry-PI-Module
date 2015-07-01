#ifndef _IOPIN_H_
#define _IOPIN_H_

struct SIOPinDev
{
	struct cdev stCdev;
   int   iMinor;
   ulong ulPin;
};

static int ContructDevice( struct SIOPinDev* pobjDev, int iMinor, struct class* pobjClass );

int iopin_open(struct inode *inode, struct file *filp);
int iopin_release(struct inode *inode, struct file *filp);
long iopin_ioctl( struct file* filp, unsigned int ioctl_num, unsigned long ioctl_param );
ssize_t iopin_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t iopin_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

#endif