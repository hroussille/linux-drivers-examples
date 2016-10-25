
# 04-Locking-In-The-Linux-Kernel

No code present here , the purpose of this file is to explain some basic locking mechanism in the linux kernel . In the next chapter ( 05-Module-File-Operations ) you will find a concrete example.

There are 2 main type of locks in the linux kernel.

- Spinlock
- Semaphore

## Spinlock

#### What is a spinlock ? 

A spinlock is a very simple and fast locking mechanism. Basically when using a spinlock your program does not sleep.
That's why it is called a spinlock. You will spin around that lock until you can aquire it.

A spin lock is by nature **uninterruptible** !

This can consume a lot of processor power so use it only in the places where the lock would be held for **very** short period of time , or in critical context , for example **interupt handlers**. Of course you **cannot** sleep while holding a spinlock.

In the linux kernel , a spinlock has the type **spinlock_t**

#### How to create a spinlock ?

There is two ways to instantiate and initiate a spin lock.

At compilation time :

```c
spinlock_t my_lock = SPIN_LOCK_UNLOCKED;
```

Or at runtime :

```c
spinlock_t my_lock;
void spin_lock_init(spinlock_t *lock);
```

#### How to use a spinlock ?

To aquire a spinlock , you must call :

```c
void spin_lock(spinlock_t *lock);
```

To release it :

```c
void spin_unlock(spinlock_t *lock);
```

#### That's it ? 

No ! , there are many many spinlock assiociated functions, also different types of spinlock like reader / writer spinlock which allow several readers but a single writer.. To have more informations about thoose types of spinlock and how to use them please explore the documentation and **experiment** with them !

## Semaphore

#### What is a semaphone ? 

A semaphore is another lock mechanism that allow you to sleep for a long period of time.

There are two types of semaphore : 

- Binary semaphore
- Normal semaphore

A binary semaphore is a semaphore that can be aquired by only one process at a given time. Also known as a mutex. A normal semaphore can be held by several processes at a given time.

One big advantage of semaphore is that when you a process is holding it , **it is allowed to sleep**

#### How to create a semaphore ?

There is two ways to create a semaphore :

At compilation time :

```c
 DEFINE_SEMAPHORE(name)
```

This will create a binary semaphore with the name given in parameter.

At runtime :

```c
static inline void sema_init(struct semaphore *sem, int val)
```
This will create a semaphore that can be hold by "val" processes. Passing 1 will create a binary semaphore while passing any positive value other than 1 will create a normal semaphore.

#### How to use a semaphore ?

To aquire a semaphore you must call : 

```c
void down(struct semaphore *sem);
```

To release it : 

```c
void up(struct semaphore *sem);
```

#### That's it ? 

Same question , same answer !
For semaphores too , there are several other functions with different features. Once again I strongly advise you to check the documentation if you need a special locking mechanism , chances are that it is already implemented in the linux kernel.

