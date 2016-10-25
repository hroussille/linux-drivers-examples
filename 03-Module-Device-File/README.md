
# 03-Module-Device-File

Everything in linux , including devices is handles as a file.
This is why , before even creating a device file , you must provide some functions for the following operations :
- Open
- Read
- Write
- Relase (Close)

Thoose are the mandatory ones for a char device.

This is done this way : 

```c
static int     example_open(struct inode *, struct file *);
static int     example_release(struct inode *, struct file *);
static ssize_t example_read(struct file *, char *, size_t, loff_t *);
static ssize_t example_write(struct file *, const char *, size_t, loff_t *);

static const struct file_operations fops = {
	.open = example_open,
	.read = example_read,
	.write = example_write,
	.release = example_release,
};
```

The struct file_operations is a structure holding pointers to specifics methods. If one was to open our device file , the function example_open would be called for example.

At this point , we are ready to create our device file :

First , register your char device :

```c
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
```

The three parameters are  :

- Device major number
- Device name
- Pointer to struct file_operations

If major number equals 0 , then one will be provided. **Check the return value !**

Next , create a device class structure :

```c
	example_Class = class_create(THIS_MODULE, CLASS_NAME);
```

The two parameters are : 

- Pointer to struct module (THIS_MODULE is the struct module defining the current module)
- Name of the class to be created

Finally , you can create your device :

```c
	example_Device = device_create(example_Class, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
```

The five parameters are  :

- Pointer to the struct class that your device should register to
- Pointer to the parent device (NULL means no parent)
- dev_t describing the device. (32 bit integer combining major and minor number)
- Pointer to additional data (drvdata) , unused here.
- Device Name

## That's it ! 

If everything went right , you now have a new device node present in **/dev** and a new class in **/sys/class/** , read .c file to get the device name and the class name. 


