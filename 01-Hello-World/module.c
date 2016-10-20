#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hector Roussille <hector.roussille@gmail.com");
MODULE_DESCRIPTION("Basic Hello Wordl! module");

/* Initialisation fonction */
static int dummy_init(void)
{
	/* Always use the pr_ functions instead of printK */
	pr_info("Hello World!\n");
	return 0;
}

/* Exit fonction */
static void dummy_exit(void) { pr_info("Goodbye World !\n"); }

/* Specify the the funcions to call on :
 * Loading the module   : dummy_init
 * Unloading the module : dummy_exit
 */

module_init(dummy_init);
module_exit(dummy_exit);
