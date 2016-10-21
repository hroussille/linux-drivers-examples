#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

/* Declare module license , author and description */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hector Roussille <hector.roussille@gmail.com");
MODULE_DESCRIPTION("Basic module with parameters");

/* Declare the variabe that we will use as an interger parameter */

static int param_int = 10;

/* 
 * Declare the previous variable as an interger parameter then
 * place a description for this parameter
 */

module_param(param_int, int, 0600);
MODULE_PARM_DESC(param_int, "Integer parameter");

/* Declare the variable that we will use as a string parameter */

static char *param_string = "test string";

/*
 * Declare the previous variable as a string parameter then
 * place a description for this parameter
 */

module_param(param_string, charp, 0600);
MODULE_PARM_DESC(param_string, "String parameter");

/*
 * Declare the variable that we will use as an interger array parameter
 * Also declare the variable that holds the number of elements presents in
 * the array
 */

static int param_int_array[2] = { 0, 0 };
static int elements_in_array;

/* Declare the previous variables as an integer array parameter and set a
 * pointer to the variable holding the number of elements currently presents
 * int it
 * Then place a description for this parameter
 */

module_param_array(param_int_array, int, &elements_in_array, 0600);
MODULE_PARM_DESC(param_int_array, "Integer array parameter");

/* print_params : prints the parameters values */

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
