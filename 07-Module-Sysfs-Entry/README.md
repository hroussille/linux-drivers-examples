
# 07-Module-Sysfs-Entry

The sysfs in a virtual file system present in **/sys** , just like the procfs present in **/proc** it exist only in ram and is created by the kernel.

While the procfs holds informations relative to processes the sysfs hold informations relative to the devices and drivers.

#### How to create a basic sysfs entry ?

Inside a kernel module , one must call :

```c
struct kobject * kobject_create_and_add(const char *name, struct kobject *parent);
```

This function will create a kernel object and add it to the sysfs. If everything went good you will see your new folder in **/sys/kernel**.

This is generally done on the init function or the probe.

#### How to delete a basic sysfs entry ?

To delete the previously created entry , you have to call :

```c
void kobject_put(struct kobject *kobj);
```

This function will decrement an internal reference count on the kobject. And if this count reaches **0** , the kobject will be deleted.

#### How to populate the previously created folder ?

This folder is good , but we are not making any use of it currently.
One good thing is that we can put files in it that link directly to some attributes inside our module.

For instance let's have an internal attribute declared this way :

```c
static int example_attribute;
```

We simply want to be able to read and write this attribute from user space in a cleaner way than with ioctl.

So let's create a **kobj_attribute** , the name is pretty clear it represents an attribute of a **kobject**.
Here is the definition of a **kobj_attribute** :

```c
struct kobj_attribute {
        struct attribute attr;
        ssize_t (*show)(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf);
        ssize_t (*store)(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count);
};
```

Where attr is the definition of the attribute including : 
- name of the attribute
- permissions of attribute

show and store are the functions called when reading (show) and writing (store) to your attribute file.

Of course you need to define your functions for show and store , the implementation and name are all yours but they must have this prototype :

```c
static ssize_t example_show(struct kobject *, struct kobj_attribute *, char *);

static ssize_t example_store(struct kobject *, struct kobj_attribute *,
			     const char *, size_t);
```

Several maccros exist to declare this attribute , they are all defined in : **sysfs.h**

The one we will use is :

```c
#define __ATTR(_name, _mode, _show, _store) {                           \
         .attr = {.name = __stringify(_name),                            \
                  .mode = VERIFY_OCTAL_PERMISSIONS(_mode) },             \
         .show   = _show,                                                \
         .store  = _store,                                               \
}
```

We could do everything by hand of course , but the above macro would fill the structure for us so let's use it : 

```c
struct kobj_attribute example_kobj_attribute = __ATTR(example_kobj_attribute,
						      0644,
						      example_show,
						      example_store);
```

Now we have everything to add our file to the kobject ! This is done by calling :

```c
int sysfs_create_files(struct kobject *kobj, const struct attribute **attr);
```

If the call return 0 , then everything went right and our attribute file is present.

There is no special cleanup for the attribute files , since they always belong to a kobject , decrementing the reference count on the kobject and making it 0 will destroy all attributes and then the kobject itself.

