
# 05-Module-File-Operations

A lot of the informations present in the code here is explained in the two previous parts : 
- 03-Module-Device-File
- 04-Locking-In-The-Linux-Kernel

Please make sur you read them and understand them.

#### The file operations structure 

All the possible file operations are defined in a structure :

```c
struct file_operations
```

This structure hold several pointers to functions that are called on a specific operation , here is the definition of this structure :

```c
struct file_operations {
    struct module *owner;
    loff_t (*llseek) (struct file *, loff_t, int);
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
    ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
    ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
    int (*readdir) (struct file *, void *, filldir_t);
    unsigned int (*poll) (struct file *, struct poll_table_struct *);
    long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
    long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
    int (*mmap) (struct file *, struct vm_area_struct *);
    int (*open) (struct inode *, struct file *);
    int (*flush) (struct file *, fl_owner_t id);
    int (*release) (struct inode *, struct file *);
    int (*fsync) (struct file *, loff_t, loff_t, int datasync);
    int (*aio_fsync) (struct kiocb *, int datasync);
    int (*fasync) (int, struct file *, int);
    int (*lock) (struct file *, int, struct file_lock *);
    ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
    unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
    int (*check_flags)(int);
    int (*flock) (struct file *, int, struct file_lock *);
    ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
    ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
    int (*setlease)(struct file *, long, struct file_lock **);
    long (*fallocate)(struct file *file, int mode, loff_t offset,
              loff_t len);
};
```

As you can see , there is **plenty** of diferent operations. However , they are not all required. In our examples we are writing char devices for now. Thoose only require 4 of them.
- open
- release
- write
- read

In the code provided , the open and the close functions are not doing anything so only write and read will be covered here.

#### Read

```c
ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
```
This functions takes 4 parameters.
- Pointer to struct file
- pointer to user buffer
- size to read
- offset

The offset is the current place in the file. If you read 10 bytes from a buffer of 100 and starting from an offset of 10 you will read the bytes 10 to 19 in the buffer.

Since you don't want someone to be able to write the file at the same time that you are reading , you have to use a locking mechanism. A read on a file can take some times , and might sleep so the prefered locking mechanish is the **Semaphore**. Read the previous chapter for more informations about it.

#### How to transfer data from kernel space to user space ?

This cannot be done with a simple memcpy call... 
The pointer you received might be invalid , or it can be valid but swapped.. Therefore you must use a special function for that.

```c
unsigned long copy_to_user (void __user *to, const void *from, unsigned long n);
```
This function copies **n** bytes from the adress pointed by **from** to the adress pointer by **to**. And is performing all the necessary check for you.

The return value is the number of bytes that couldn't be copied.

#### Write

```c
 ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
```
This functions takes 4 parameters.
- Pointer to struct file
- pointer to user buffer
- size to read
- offset
- 
### How to transfer data from user space to kernel space ?

For the same reasons as above , you must use a special function for this kind of transfert.

```c
unsigned long copy_from_user (void *to, const void __user *from, unsigned long n);
```
This function copies **n** bytes from the adress pointed by **from** to the adress pointer by **to**. And is performing all the necessary check for you.

The return value is the number of bytes that couldn't be copied.


#### What to do with the offset ?

It is up to the driver to modify the offset according to the operation that was performed.

#### What about the other operations ?

I strongly advice you to implement them as a learning experience , starting by some easy ones like :

```c
 loff_t (*llseek) (struct file *, loff_t, int);
```
And then go for some more complicated ones. It is a great exercise.
