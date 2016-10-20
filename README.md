# linux-drivers-examples

##### Note : this repo is the content i am using / creating during my learning experience of linux driver development 

On this repository you will find ressources to use as an introduction to linux driver development.

  - Makefile example
  - Basic Hello World! module
  - Module parameters
  - Module with sysfs entry
  - Basic timer usage
  - Ioctl usage
  - etc...

You can also find in this repository some drivers specific for the Raspberrypi

  - Basic Led driver
  - etc...

# hardware required for the raspberrypi specific drivers
  - Raspberry pi (bcm 2835 or bcm2836 based board)
  - Leds & resistance
  - JY-MCU serial bluetooth module.

# Coding style

All the modules will be written with the linux kernel coding style.
A script to check that your file respect the linux kernel coding style is present in the kernel source directory : 
**scripts/checkpatch.pl**

Use it with the option **--file** followed by your file(s) name(s) to check for any mistake.

As a reminder the linux kernel coding style is defined in the linux kernel source tree : 
**/Documentation/CodingStyle**

License
----

GNU GPL V2

**Free Software, Hell Yeah!**
