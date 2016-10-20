# 01-Hello-World

# MODULE MACROS

- MODULE_LICENSE : describe the license of the module
- MODULE_AUTHOR : describe the author of the module
- MODULE_DESCRIPTION : describe the module itself

# Module entry point

A linux kernel module entry point is defined the macro **module_init()**

```c
module_init(dummy_init);
```
This function is called when the module is linked against the running kernel.

# Module exit point

A linux kernel module exit point is defined by the macro **module_exit()**

```c
module_exit(dummy_exit);
```
# Logs

Logs inside a module can be very usefull for the developer and for the user too.

A lot of online examples make use of **printk()** to log informations visible later with the command 

```sh
?> dmesg
```

However , the direct use of printk is **NOT** a good practice.

Typically calling the **pr_** functions is better.

```c
pr_info(fmt, ...);
pr_emerg(fmt, ...); 
pr_alert(fmt, ...);
pr_crit(fmt, ...);
pr_err(fmt, ...);
pr_warning(fmt, ...);
pr_notice(fmt, ...);
```

Later the usage of **dev_dbg** will be covered too.

