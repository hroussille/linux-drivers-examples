#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>

#define  DEVICE_NAME "example_device"
#define  CLASS_NAME  "example_class"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hector Roussille");
MODULE_DESCRIPTION("Example of char device driver");

static int    majorNumber;
static struct class *example_Class;
static struct device *example_Device;

/*
 * Adding the semaphore for locking mechanism
 */
static struct semaphore lock;

/* Define the size of our internal buffer */
#define BUFFER_SIZE 512

/* Declare the internal buffer */
static char content[BUFFER_SIZE];

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

static int example_init(void)
{
	pr_info("Example driver init");

	/*
	 * Initialise the semaphore
	 * Here use a count of 1 so only one owner is allowed
	 */

	sema_init(&lock, 1);

	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);

	if (majorNumber < 0)
		return majorNumber;

	example_Class = class_create(THIS_MODULE, CLASS_NAME);

	if (IS_ERR(example_Class)) {
		unregister_chrdev(majorNumber, DEVICE_NAME);
		return PTR_ERR(example_Class);
	}

	example_Device = device_create(example_Class, NULL,
				MKDEV(majorNumber, 0), NULL, DEVICE_NAME);

	if (IS_ERR(example_Device)) {
		class_destroy(example_Class);
		unregister_chrdev(majorNumber, DEVICE_NAME);
		return PTR_ERR(example_Device);
	}
	return 0;
}

static void example_exit(void)
{
	device_destroy(example_Class, MKDEV(majorNumber, 0));
	class_unregister(example_Class);
	class_destroy(example_Class);
	unregister_chrdev(majorNumber, DEVICE_NAME);
	pr_info("Example driver exit");
}

static int example_open(struct inode *inodep, struct file *filep)
{
	pr_info("Example driver open");
	return 0;
}

static ssize_t example_read(struct file *filep, char *buffer, size_t len,
			loff_t *offset)
{
	int error;

	/* If the offset is out of range return 0 */
	if (*offset >= BUFFER_SIZE)
		return 0;

	/* If there is less than len bytes to read , truncate len */
	if (*offset + len >= BUFFER_SIZE)
		len = BUFFER_SIZE - *offset;

	/* Acquire the lock */
	down(&lock);

	/* Perform the actual read */
	error = copy_to_user(buffer, content, len);

	/* Release the lock */
	up(&lock);

	/* Update the offset */
	*offset += len - error;

	/* If any error occured notify the user */
	if (error != 0)
		return -ENOMEM;

	/* Else return the number of bytes read */
	return len - error;
}

static ssize_t example_write(struct file *filep, const char *buffer, size_t len,
			 loff_t *offset)
{
	int error;

	/* If the offset is out of range return 0 */
	if (*offset >= BUFFER_SIZE)
		return 0;

	/* If there is less than len bytes free , truncate len */
	if (*offset + len >= BUFFER_SIZE)
		len = BUFFER_SIZE - *offset;

	/* Acquire the lock */
	down(&lock);

	/* Perform the actual write */
	error = copy_from_user(content, buffer, len);

	/* Release the lock */
	up(&lock);

	/* Update the offset */
	*offset += len - error;

	/* If any error occured notify the user */
	if (error != 0)
		return -ENOMEM;

	/* Else return the number of bytes written */
	return len - error;

}

static int example_release(struct inode *inodep, struct file *filep)
{
	pr_info("Example driver release");
	return 0;
}

module_init(example_init);
module_exit(example_exit);
