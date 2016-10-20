# if KERNELRELEASE is not defined, we've been called directly from the     command line.
# Invoke the kernel build system.
ifeq (${KERNELRELEASE},)
	KDIR ?= /lib/modules/`uname -r`/build
default:
	make -C ${KDIR} M=${PWD}

clean:
	make clean -C ${KDIR} M=${PWD}


# Otherwise KERNELRELEASE is defined; we've been invoked from the
# kernel build system and can use its language.
else
    obj-m := module.o
endif
