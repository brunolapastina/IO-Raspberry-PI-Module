/*  
 *  iopin.c - A simple IO module for the raspberry pi
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>

#define DRIVER_AUTHOR   "Bruno La Pastina"
#define DRIVER_DESC     "A basic IO module for the Raspberry PI"

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( DRIVER_AUTHOR );
MODULE_DESCRIPTION( DRIVER_DESC );

#define DEVICE_NAME     "iopin"   // Dev name as it appears in /proc/devices

//-----[ Module parameters ]------
static unsigned long NumOfDevices = 1;

module_param( NumOfDevices, ulong, S_IRUGO );
MODULE_PARM_DESC( NumOfDevices, "Number of devices to be created" );

//------[ General definitions ]------
struct SIOPinDev
{
	struct cdev stCdev;
   int   iMinor;
};

static int ContructDevice( struct SIOPinDev* pobjDev, int iMinor, struct class* pobjClass );
int iopin_open(struct inode *inode, struct file *filp);
int iopin_release(struct inode *inode, struct file *filp);
ssize_t iopin_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t iopin_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

//------[ Module operations ]------
struct file_operations g_stIOPinFops =
{
   .owner   = THIS_MODULE,
   .read    = iopin_read,
   .write   = iopin_write,
   .open    = iopin_open,
   .release = iopin_release
};

//------[ Global variables ]------
static int g_iIOPinMajor;
static struct class* g_pobjIOPinClass = NULL;
static struct SIOPinDev* g_astIOPinDevices = NULL;

static int __init iopin_init(void)
{
   dev_t dev = 0;
   int   iRet;
   int   i;
   
   printk( KERN_INFO "[IOPin] Initing module...\n" );
   
   // Register the driver, let the kernel assing a major number and request some minors
   iRet = alloc_chrdev_region( &dev, 0, NumOfDevices, DEVICE_NAME );
   if ( 0 > iRet )
   {
      printk( KERN_ERR "[IOPin] Error registering driver - ret=%d\n", iRet );
      return iRet;
   }
   g_iIOPinMajor = MAJOR(dev);
   
   // Create device class (before allocation of the array of devices)
   g_pobjIOPinClass = class_create( THIS_MODULE, DEVICE_NAME );
   if ( IS_ERR( g_pobjIOPinClass ) )
   {
      iRet = PTR_ERR( g_pobjIOPinClass );
      unregister_chrdev_region( MKDEV(g_iIOPinMajor, 0), NumOfDevices );
      return iRet;
   }
   
   // Allocate the array of devices
   g_astIOPinDevices = (struct SIOPinDev *)kzalloc( NumOfDevices * sizeof(struct SIOPinDev), GFP_KERNEL );
   if ( NULL == g_astIOPinDevices )
   {
      unregister_chrdev_region( MKDEV(g_iIOPinMajor, 0), NumOfDevices );
      return -ENOMEM;
   }
   
   for( i = 0; i < NumOfDevices; i++ )
   {  // Create /dev devices
      iRet = ContructDevice( &g_astIOPinDevices[i], i, g_pobjIOPinClass );
      if ( iRet )
      {  // Error
         for( i -= 1; i >= 0; i-- )
         {  // Destroy the devices that have been created successfully
            device_destroy( g_pobjIOPinClass, MKDEV( g_iIOPinMajor, i ) );
            cdev_del(&g_astIOPinDevices[i].stCdev);
         }
         kfree( g_astIOPinDevices );
         g_astIOPinDevices = NULL;
         class_destroy( g_pobjIOPinClass );
         unregister_chrdev_region( MKDEV(g_iIOPinMajor, 0), NumOfDevices );
         return iRet;
      }
   }
   
   printk( KERN_INFO "[IOPin] Initialized\n" );
   
   return 0;
}

static void __exit iopin_exit(void)
{
   int i;
	// Get rid of all the /dev devices created on the __init
	
	if (g_astIOPinDevices)
   {
		for ( i = 0; i < NumOfDevices; i++ )
      {
         device_destroy( g_pobjIOPinClass, MKDEV( g_iIOPinMajor, i ) );
         cdev_del(&g_astIOPinDevices[i].stCdev);
		}
		kfree( g_astIOPinDevices );
	}
	
	if ( g_pobjIOPinClass )
   {
		class_destroy( g_pobjIOPinClass );
   }
   
   unregister_chrdev_region( MKDEV(g_iIOPinMajor, 0), NumOfDevices );
   
   printk( KERN_INFO "[IOPin] Module removed\n" );
}

module_init( iopin_init );
module_exit( iopin_exit );

static int ContructDevice( struct SIOPinDev* pobjDev, int iMinor, struct class* pobjClass )
{
	int iRet;
	dev_t devno = MKDEV( g_iIOPinMajor, iMinor );
	struct device* pstDevice = NULL;
	
	BUG_ON( (NULL == pobjDev) || (NULL == pobjClass) );
	
	cdev_init( &pobjDev->stCdev, &g_stIOPinFops );
	pobjDev->stCdev.owner = THIS_MODULE;
   pobjDev->iMinor = iMinor;
	
	iRet = cdev_add( &pobjDev->stCdev, devno, 1 );
	if (iRet)
	{
		printk( KERN_WARNING "[IOPin] Error %d while trying to add %s%d\n", iRet, DEVICE_NAME, iMinor );
		return iRet;
	}

	pstDevice = device_create( pobjClass, NULL, devno, NULL, DEVICE_NAME "%d", iMinor);
	if ( IS_ERR( pstDevice ) )
   {
		iRet = PTR_ERR( pstDevice );
		printk(KERN_WARNING "[IOPin] Error %d while trying to create %s%d\n", iRet, DEVICE_NAME, iMinor);
		cdev_del( &pobjDev->stCdev );
		return iRet;
	}
   
	return 0;
}

int iopin_open(struct inode *inode, struct file *filp)
{
	unsigned int iMajor = imajor(inode);
	unsigned int iMinor = iminor(inode);
   struct SIOPinDev* dev = NULL;
	
	if ( (iMajor != g_iIOPinMajor) || (0 > iMinor) || (iMinor >= NumOfDevices) )
	{
		printk(KERN_WARNING "[IOPin] No device found with Major=%d and Minor=%d\n", iMajor, iMinor);
		return -ENODEV;
	}
	
	// Store a pointer to struct SIOPinDev here for other methods
	dev = &g_astIOPinDevices[iMinor];
	filp->private_data = dev;
   
   if (inode->i_cdev != &dev->stCdev)
	{
		printk(KERN_WARNING "[IOPin] open: internal error\n");
		return -ENODEV;
	}
   
   printk(KERN_INFO "[IOPin] Open on %d:%d\n", iMajor, iMinor);
	
	return 0;
}

int iopin_release(struct inode *inode, struct file *filp)
{
   struct SIOPinDev* dev = (struct SIOPinDev*)filp->private_data;
   
   printk(KERN_INFO "[IOPin] Release on minor %d\n", dev->iMinor );
   
	return 0;
}

ssize_t iopin_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
   struct SIOPinDev* dev = (struct SIOPinDev*)filp->private_data;
   
   printk(KERN_INFO "[IOPin] Read on minor %d\n", dev->iMinor );
   
	return 0;
}

ssize_t iopin_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
   struct SIOPinDev* dev = (struct SIOPinDev*)filp->private_data;
   
   printk(KERN_INFO "[IOPin] Write on minor %d\n", dev->iMinor );
   
	return count;
}