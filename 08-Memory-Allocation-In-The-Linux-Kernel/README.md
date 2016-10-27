# 08-Memory-Allocation-In-The-Linux-Kernel

There are really 2 functions to call when you need some memory "dynamically" allocated by the kernel.

#### kmalloc
```c
void * kmalloc(size_t size, int flags)
```

#### vmalloc
```c
void * vmalloc(unsigned long size)
```

#### Why two functions ? 

The first one , **kmalloc** allocates memory that is **both** contiguous in physical and virtual memory. Making it suitable for **DMA** transferts for example. However , **kmalloc** cannot allocate very big chunks of memory.

The second one , **vmalloc** allocates memory that is **only** contiguous in virtual memory. Also because it would have to remap the buffer in a virtual contiguous range **vmalloc** is usually slower.

#### What are the flags for kmaloc ?

- __GFP_WAIT : The allocator can sleep.
- __GFP_HIGH : The allocator can access emergency pools.
- __GFP_IO : The allocator can start disk I/O.
- __GFP_FS : The allocator can start filesystem I/O.
- __GFP_COLD : The allocator should use cache cold pages.
- __GFP_NOWARN : The allocator will not print failure warnings.
- __GFP_REPEAT : The allocator will repeat the allocation if it fails, but the allocation can potentially fail.
-  __GFP_NOFAIL : The allocator will indefinitely repeat the allocation. The allocation cannot fail.
- __GFP_NORETRY : The allocator will never retry if the allocation fails.
- __GFP_NO_GROW : Used internally by the slab layer.
- __GFP_COMP : Add compound page metadata. Used internally by the hugetlb code.
