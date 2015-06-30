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
#include <mach/platform.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include <iopin.h>
#include <iopin_ioctl.h>

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

//------[ Module operations ]------
struct file_operations g_stIOPinFops =
{
   .owner            = THIS_MODULE,
   .open             = iopin_open,
   .release          = iopin_release,
   .unlocked_ioctl   = iopin_ioctl,
   .read             = iopin_read,
   .write            = iopin_write,
};

//------[ Global variables ]------
static int g_iIOPinMajor;
static struct class* g_pobjIOPinClass = NULL;
static struct SIOPinDev* g_astIOPinDevices = NULL;
static struct SGpioRegistersMap* g_pstGpioRegisters = NULL;

static int __init iopin_init(void)
{
   dev_t dev = 0;
   int   iRet;
   int   i;
   
   g_pstGpioRegisters = (struct SGpioRegistersMap*) ioremap( GPIO_BASE, sizeof(struct SGpioRegistersMap) );    // Should the size be GPIO_SIZE???
   if( NULL == g_pstGpioRegisters )
   {
      printk( KERN_ERR "[IOPin] Failed to map GPIO registers\n" );
      return -ENOMEM;
   }
   
   // Allocate the array of devices
   g_astIOPinDevices = (struct SIOPinDev *)kzalloc( NumOfDevices * sizeof(struct SIOPinDev), GFP_KERNEL );
   if ( NULL == g_astIOPinDevices )
   {
      printk( KERN_ERR "[IOPin] Failed to allocate memory\n" );
      iounmap( g_pstGpioRegisters );
      g_pstGpioRegisters = NULL;
      return -ENOMEM;
   }
   
   // Register the driver, let the kernel assing a major number and request some minors
   iRet = alloc_chrdev_region( &dev, 0, NumOfDevices, DEVICE_NAME );
   if ( 0 > iRet )
   {
      printk( KERN_ERR "[IOPin] Error registering driver - ret=%d\n", iRet );
      kfree( g_astIOPinDevices );
      g_astIOPinDevices = NULL;
      iounmap( g_pstGpioRegisters );
      g_pstGpioRegisters = NULL;
      return iRet;
   }
   g_iIOPinMajor = MAJOR(dev);
   
   // Create device class (before allocation of the array of devices)
   g_pobjIOPinClass = class_create( THIS_MODULE, DEVICE_NAME );
   if ( IS_ERR( g_pobjIOPinClass ) )
   {
      iRet = PTR_ERR( g_pobjIOPinClass );
      unregister_chrdev_region( MKDEV(g_iIOPinMajor, 0), NumOfDevices );
      kfree( g_astIOPinDevices );
      g_astIOPinDevices = NULL;
      iounmap( g_pstGpioRegisters );
      g_pstGpioRegisters = NULL;
      return iRet;
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
         class_destroy( g_pobjIOPinClass );
         unregister_chrdev_region( MKDEV(g_iIOPinMajor, 0), NumOfDevices );
         kfree( g_astIOPinDevices );
         g_astIOPinDevices = NULL;
         iounmap( g_pstGpioRegisters );
         g_pstGpioRegisters = NULL;
         return iRet;
      }
   }
   
   printk( KERN_INFO "[IOPin] Module loaded\n" );
   
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
      g_astIOPinDevices = NULL;
   }
   
   if ( g_pobjIOPinClass )
   {
      class_destroy( g_pobjIOPinClass );
      g_pobjIOPinClass = NULL;
   }
   
   unregister_chrdev_region( MKDEV(g_iIOPinMajor, 0), NumOfDevices );
   
   if ( g_pstGpioRegisters )
   {
      iounmap( g_pstGpioRegisters );
      g_pstGpioRegisters = NULL;
   }
   
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
   #warning !!!!!! Change the pin number !!!!!!
   pobjDev->ulPin = 7;
   
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

int iopin_open(struct inode* inode, struct file* filp)
{
   unsigned int iMajor = imajor(inode);
   unsigned int iMinor = iminor(inode);
   unsigned int uiFunction;
   struct SIOPinDev* dev = NULL;
   
   //printk(KERN_INFO "[IOPin] open on %d:%d\n", iMajor, iMinor);
   
   if ( (iMajor != g_iIOPinMajor) || (0 > iMinor) || (iMinor >= NumOfDevices) )
   {
      printk(KERN_WARNING "[IOPin] open: No device found with Major=%d and Minor=%d\n", iMajor, iMinor);
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
   
   // Check the current configuration. If it is not input nor output, it is probably been used by another driver.
   // In that case, we are going to fail the open
   uiFunction = (ioread32( &g_pstGpioRegisters->GPFSEL[dev->ulPin / 10] ) >> ((dev->ulPin % 10) * 3)) & 0b111;
   if( (PIN_FUNCTION_INPUT != uiFunction) && (PIN_FUNCTION_OUTPUT != uiFunction ) )
   {  // Neither input nor output
      printk( KERN_WARNING "[IOPin] open: GPIO%lu it no configures as an alternate function (%u)\n", dev->ulPin, uiFunction );
      return -EIO;
   }
   
   return 0;
}

int iopin_release(struct inode* inode, struct file* filp)
{
   struct SIOPinDev* dev = (struct SIOPinDev*)filp->private_data;
   
   //printk( KERN_INFO "[IOPin] release: Releasing minor %d\n", dev->iMinor );
   
   if (inode->i_cdev != &dev->stCdev)
   {
      printk( KERN_WARNING "[IOPin] release: internal error\n" );
      return -ENODEV;
   }
   
   return 0;
}

long iopin_ioctl( struct file* filp, unsigned int ioctl_num, unsigned long ioctl_param )
{
   struct SIOPinDev* dev = (struct SIOPinDev*)filp->private_data;
   
   switch (ioctl_num)
   {
      case IOCTL_SET_FUNCTION:
      {  // Configure the pin function
         unsigned int uiRegisterIndex = dev->ulPin / 10;
         unsigned int uiBit = (dev->ulPin % 10) * 3;
         unsigned int uiOldValue = ioread32( &g_pstGpioRegisters->GPFSEL[uiRegisterIndex] );
         unsigned int uiMask = 0b111 << uiBit;
         
         if( (PIN_FUNCTION_INPUT != ioctl_param) && (PIN_FUNCTION_OUTPUT != ioctl_param) )
         {
            printk( KERN_WARNING "[IOPin] ioctl: Invalid pin function %lu\n", ioctl_param );
            return -EINVAL;
         }
         
         printk( KERN_INFO "[IOPin] ioctl: Changing function of GPIO%lu from %x to %lx\n", dev->ulPin, (uiOldValue >> uiBit) & 0b111, ioctl_param );
         iowrite32( (uiOldValue & ~uiMask) | ((ioctl_param << uiBit) & uiMask), &g_pstGpioRegisters->GPFSEL[uiRegisterIndex] );
         
         break;
      }
      
      default:
      {
         printk( KERN_WARNING "[IOPin] Unkown ioctl %u\n", ioctl_num );
         return -EINVAL;
      }
   }
   
   return 0;
}

ssize_t iopin_read(struct file* filp, char __user* buf, size_t count, loff_t* f_pos)
{
   struct SIOPinDev* dev = (struct SIOPinDev*)filp->private_data;
   unsigned int uiValue;
   
   //printk(KERN_INFO "[IOPin] Read on minor %d\n", dev->iMinor );
   
   if( 1 > count )
   {
      return 0;
   }
   
   uiValue = ioread32( &g_pstGpioRegisters->GPLEV[dev->ulPin / 32] );
   uiValue &= (1 << (dev->ulPin % 32));      // Mask the desired bit
   
   if( uiValue )
   {
      put_user( '1', buf++);
   }
   else
   {
      put_user( '0', buf++);
   }
   
   return 1;
}

ssize_t iopin_write(struct file* filp, const char __user* buf, size_t count, loff_t* f_pos)
{
   struct SIOPinDev* dev = (struct SIOPinDev*)filp->private_data;
   unsigned int uiFunction;
   char  chValue;
   
   //printk(KERN_INFO "[IOPin] Write on minor %d\n", dev->iMinor );
   
   // Get configured function
   uiFunction = (ioread32( &g_pstGpioRegisters->GPFSEL[dev->ulPin / 10] ) >> ((dev->ulPin % 10) * 3)) & 0b111;
   
   // Check if pin is output
   if( PIN_FUNCTION_OUTPUT != uiFunction )
   {
      printk(KERN_INFO "[IOPin] GPIO%lu not configure as output (%x)\n", dev->ulPin, uiFunction );
      return -EPERM;
   }
   else if( 1 <= count )
   {
      // The first byte is the only one that matters for us
      get_user( chValue, buf );
      
      if( '1' == chValue )
      {
         iowrite32( (1 << (dev->ulPin % 32)), &g_pstGpioRegisters->GPSET[dev->ulPin / 32] );
      }
      else
      {
         iowrite32( (1 << (dev->ulPin % 32)), &g_pstGpioRegisters->GPCLR[dev->ulPin / 32] );
      }
   }
   
   return count;
}