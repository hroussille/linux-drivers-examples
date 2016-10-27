#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hector Roussille <hector.roussille@gmail.com");
MODULE_DESCRIPTION("Basic Module With Sysfs Entry");

/* Pointer to the actual kobject */
static struct kobject *example_kobject;

/* This is the internal attribute that can be written or read */
static int example_attribute;

/* Declaration of the show function called when reading the attribute */
static ssize_t example_show(struct kobject *, struct kobj_attribute *, char *);

/* Declaration of the store function called when writing the attribute */
static ssize_t example_store(struct kobject *, struct kobj_attribute *,
			     const char *, size_t);

/*
 * Actual declaration of the kobj_attribute
 * The macro __ATTR expand to the actual definition of the kobj_attribute
 * structure. This call is basically setting up everything for our file
 * attribute :
 * - name
 * - permisissions
 * - function to call on read
 * - function to call on write
 */
struct kobj_attribute example_kobj_attribute = __ATTR(example_kobj_attribute,
						      0644,
						      example_show,
						      example_store);

/* Initialisation function */
static int example_init(void)
{
	/* Create and add the kobject to the sysfs */
	example_kobject = kobject_create_and_add("kobject_example",
						 kernel_kobj);

	/*
	 * At this point a folder is created in /sys/kernel but it is empty. The
	 * following calls will add a file attribute to this folder
	 */

	if (!example_kobject)
		return -ENOMEM;

	pr_info("Created Kobject\n");

	/* Create the attribute file */
	if (sysfs_create_file(example_kobject, &example_kobj_attribute.attr))
		pr_debug("failed to create the file in /sys/kernel/kobject_example\n");

	return 0;
}

/* Exit function */
static void example_exit(void)
{
	/* No need to check for the return value of kobject_put , this function
	 * returns void and because of it's nature cannot fail
	 */

	kobject_put(example_kobject);
	pr_info("Destroyed Kobject\n");
}

/*
 * Implementation of the example_show function
 * This function is called everytime a read is issued on our attribute file
 * present in the sysfs
 */
static ssize_t example_show(struct kobject *kobj, struct kobj_attribute *attr,
			    char *buf)
{
	pr_info("Example_show called !\n");
	return sprintf(buf, "%d\n", example_attribute);
}

/*
 * Implementation of the example_store function
 * This function is called everytime a write is issued on our attribute file
 * present in the sysfs
 */
static ssize_t example_store(struct kobject *kobj, struct kobj_attribute *attr,
			     const char *buf, size_t count)
{
	pr_info("Example_store called !\n");
	if (sscanf(buf, "%du", &example_attribute) != 1) {
		pr_info("Wrong argument value\n");
		return -EINVAL;
	}
	return count;
}

module_init(example_init);
module_exit(example_exit);
