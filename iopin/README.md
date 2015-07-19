#IO-Raspberry-PI-Module
####A basic module for Raspberry PI to control GPIO.

######How to load the driver:
insmod ./iopin.ko pins=[device list]
   The device list is a comma-separated list, for example: 7,10,15...

To automatically assign 0666 permission to all devices created by the driver, copy the 99-iopin.rules file to /etc/udev/rules.d and run "udevadm control --reload-rules" before loading the driver
   
######TO DO:
* Implement debounce in kernel space
* Use the kernel API for GPIO (this was not done because I wanted to learn to change the registers by hand)

