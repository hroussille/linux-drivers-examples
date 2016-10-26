#ifndef MODULE_IOCTL_H
#define MODULE_IOCTL_H

#include <linux/ioctl.h>
#include <linux/types.h>

/* Define our magic number to identify our IOCTLs */
#define EXAMPLE_MAGIC 0xFF

/* Define the structure we will use to communicate using ioctls */
struct example_ioctl_data {

	/*
	 * Always use some well defined types in your ioctls since they can be
	 * used from 32bits user space programs or 64 bits
	 */

	__u32 call_number;
	__u8  buffer[255];
};

/* Define our actual IOCTLs */
#define EXAMPLE_IOCTL _IO(EXAMPLE_MAGIC, 0)
#define EXAMPLE_IOCTL_W _IOW(EXAMPLE_MAGIC, 1, struct example_ioctl_data)
#define EXAMPLE_IOCTL_R _IOR(EXAMPLE_MAGIC, 2, struct example_ioctl_data)
#define EXAMPLE_IOCTL_WR _IOWR(EXAMPLE_MAGIC, 3, struct example_ioctl_data)


#endif
