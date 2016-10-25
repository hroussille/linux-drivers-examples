#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define  DEVICE_NAME "example_device"
#define  CLASS_NAME  "example_class"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hector Roussille");
MODULE_DESCRIPTION("Example of char device driver");

static int    majorNumber;
static struct class *example_Class;
static struct device *example_Device;

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
	pr_info("Example driver read");
	return 0;
}

static ssize_t example_write(struct file *filep, const char *buffer, size_t len,
			 loff_t *offset)
{
	pr_info("Example driver write");

	/*
	 * Return -EPERM and not 0 to signal the writer that the function is
	 * not allowed to be used.
	 */

	return -EPERM;
}

static int example_release(struct inode *inodep, struct file *filep)
{
	pr_info("Example driver release");
	return 0;
}

module_init(example_init);
module_exit(example_exit);
