# Makefie-example

In this makefile you can clearly see 2 parts.
because our makefile will be read twice. Once when we run make , and a second time by the kernel build system.

The first one  :

```Makefile
ifeq (${KERNELRELEASE},)
    KDIR ?= /lib/modules/`uname -r`/build
        default:
            make -C ${KDIR} M=${PWD}
        clean:
            make clean -C ${KDIR} M=${PWD}
```

Is invoking the kernel build system. The kernel build system knows that we are building an out of tree kernel module thanks to the option :

```Makefile
M=${PWD}
```

The kernel build system is going to read our makefile a second time after defining some usefull variables , like 

```Makefile
${KERNELRELEASE}
```

If this variable is defined the second part is read from the makefile :

```Makefile
else
	obj-m := main.o
	main-objs := file1.o file2.o
endif
```

This syntax is the kernel build system "language" , it is a very simple syntax allowing us to build modules easily.

