
# 06-Module-Ioctl

**The use of ioctl in a linux module has more or less become a bad practice , not necessarely because of the nature of an ioctl but because of the usage people did of thoose.. You can talk directly to the driver with an ioctl , but there are better ways to do it that will be covered in the next chapter , Ioctls are okay , but don't place all your communication on it , there are some much cleaner ways**

#### What is an Ioctl ? 

An ioctl is a way to communicate directly to a device driver. This is done in user space with the function :

```c
int ioctl(int fildes, int request, ... /* arg */);
```

You cann pass any argument you need in **arg** , the format of **arg** is defined by the **request**.

#### How is represented an ioctl in a Linux driver ?

Since everything in linux , including devices are represented as files , the ioctl will be set in the struct file_operations.

There are two fields for ioctl in the struct file_operations :

- unlocked_ioctl
- compat_ioctl

##### What is the difference ?

In earlier versions of the Linux kernel , there was something called the Big Kernel Lock (BKL) , this lock was more or less preventing everything to be done elsewhere. And Ioctls were making a very big usage of it , resulting sometimes in long useless delay that could even hurt the performance of the system. This was a design not suited for multi-cores systems.

So **unlocked_ioctl** was added with **compat_ioctl**.

The first one , **unlocked_ioctl** is called like this because it doesn't make any use of the **BKL**.

The second ome , **compat_ioctl** is here for compatibility , if a 32 bit user space process calls the ioctl function , the compat_ioctl function will be called allowing a 32 bit user space process to make calls on a 64 bit kernel. (This is revelant for x86-64 and might not be true for every architecture)

**Always use well defined types in your ioctls to be compatible between 32 / 64 bits**
- __u32
- __u8
- etc...

##### How to create an ioctl ?

First of all , define your functions for **unlocked_ioctl** and **compat_ioctl**.

```c
static long example_unlocked_ioctl(struct file *, unsigned int, unsigned long);
static long example_compat_ioctl(struct file *, unsigned int, unsigned long);
```

Then you need to set the appropriates fields in the struct file_operations :

```c
static const struct file_operations fops = {
	.open = example_open,
	.read = example_read,
	.write = example_write,
	.release = example_release,
	/* Add our compat ioctl function to the file_operations structure */
	.compat_ioctl = example_compat_ioctl,
	/* Add our unlocked ioctl function to the file_operations structure */
	.unlocked_ioctl = example_unlocked_ioctl,
};
```

Next you need to actually create your ioctl number(s).

Some macros exist to guarantee or at least try to guarantee that an ioctl number is unique.

```c
#define _IO(type,nr)		_IOC(_IOC_NONE,(type),(nr),0)
#define _IOR(type,nr,size)	_IOC(_IOC_READ,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOW(type,nr,size)	_IOC(_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOWR(type,nr,size)	_IOC(_IOC_READ|_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
```

Thoose macros are self explaining , if any **READ** or **WRITE** needs to be done **from the point of vue of the user** then use the appropriate macro.

They all take :

- magic number
- command number

Only the 3 last ones also take :

- the type of the data transfered

Most drivers use their major number as the magic number , this is fine.
Check the documentation for more informations.

For the example , let's take this magic number :

```c
/* Define our magic number to identify our IOCTLs */
#define EXAMPLE_MAGIC 0xFF
```

And the data to be transfered on an ioctl would be defined like this :

```c
/* Define the structure we will use to communicate using ioctls */
struct example_ioctl_data {
	/*
	 * Always use some well defined types in your ioctls since they can be
	 * used from 32bits user space programs or 64 bits
	 */
	__u32 call_number;
	__u8  buffer[255];
};
```

Therefore if we were to create 4 ioctls , one of each kind :

```c
#define EXAMPLE_IOCTL _IO(EXAMPLE_MAGIC, 0)
#define EXAMPLE_IOCTL_W _IOW(EXAMPLE_MAGIC, 1, struct example_ioctl_data)
#define EXAMPLE_IOCTL_R _IOR(EXAMPLE_MAGIC, 2, struct example_ioctl_data)
#define EXAMPLE_IOCTL_WR _IOWR(EXAMPLE_MAGIC, 3, struct example_ioctl_data)
```

That's it ! all you need to do now is to check the request number in your **unlocked_ioctl** and **compat_ioctl** to know what to do. Most of the time a switch is used.


