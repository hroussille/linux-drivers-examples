# 02-Module-Parameters

#### Declaring a parameter :

```c
module_param(variable_name, type , permissions);
MODULE_PARM_DESC(variable_name , description);
```

#### if the parameter is an array : 

```c
module_param_array(variable_name, type of *variable_name , ptr to arraySize , permissions);
MODULE_PARM_DESC(variable_name, description);
```
ex : for an int * array :

```c
static int *array = { 0, 0 };
static int elements_in_array;

module_param_array(array, int, &elements_in_array, 0000);
MODULE_PARM_DESC(array, "Integer array parameter");
```

####  one special case for string (char *) :

use the first expression , as for declaring a simple parameter that is not an array , but write **charp** as type.

ex :
```c
module_param(param_string, charp, 0600);
MODULE_PARM_DESC(param_string, "String parameter");

```




