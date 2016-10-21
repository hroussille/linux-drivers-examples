#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hector Roussille <hector.roussille@gmail.com");
MODULE_DESCRIPTION("Basic module with parameters");

static int param_int = 10;

module_param(param_int, int, 0600);
MODULE_PARM_DESC(param_int, "Integer parameter");

static char *param_string = "test string";

module_param(param_string, charp, 0600);
MODULE_PARM_DESC(param_string, "String parameter");

static int param_int_array[2] = { 0, 0 };
static int elements_in_array;

module_param_array(param_int_array, int, &elements_in_array, 0600);
MODULE_PARM_DESC(param_int_array, "Integer array parameter");

void print_params(void)
{
	int i;

	pr_info("Parameter int : %d\n", param_int);
	pr_info("Parameter string : %s\n", param_string);
	for (i = 0 ; i < sizeof(param_int_array) / sizeof(int) ; i++)
		pr_info("Parameter Array[%d] : %d\n", i, param_int_array[i]);
}

static int dummy_init(void)
{
	pr_info("Module Parameters Init\n");
	print_params();
	return 0;
}

static void dummy_exit(void)
{
	pr_info("Module Parameters Exit\n");
	print_params();
}


module_init(dummy_init);
module_exit(dummy_exit);
