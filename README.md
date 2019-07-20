David Zhang  
Operating Systems - xv6 - Implement System Call  

## Description

 [Background](https://pdos.csail.mit.edu/6.828/2012/xv6.html) on xv6


#### Part 1 - Memtop system call (returns the amount of memory available)

 ##### Main approach

 xv6 holds a linked list of free memory managed by kalloc() and
 kfree(). The total available memory is just from the beginning of
 the list to the end.

 Related files

 - <sysproc.c>   implementation of int sys_memtop(void)
 - <defs.h>      declare int pagesleft(void)
 - <usys.S>      add to SYSCALL(memtop)
 - <user.h>      declare int memtop(void)
 - <syscall.h>   define SYS_memtop as call 22
 - <syscall.c>   extern sys_memtop, add to vector [SYS_memtop] sys_memtop
 - <kalloc.c>    Implementation of int pagesleft(void)


#### Part 2 - Print memory stats for each process running in the system

 ##### Main approach

 The total memory for a process consists of 4 parts (depends on how
 you count it): kernel stack, user stack, program memory, and overh-
 eads. We know the kernel stack is just 1 page, the user's memory
 can be easily computed by calling proc->sz. Overhead, the place to
 log where the actual memory are, is the hardest part.

 To get the overhead memory for each process, first is to get all
 running process. We can do so by checking the highest pid number by
 fork a process. Then, we can check the process table - ptable with
 the pids from 1 to that number. After that, we just need to walk
 the page directory, p->pgdir, by calling walkleaves(), defined in
 proc.c. To see how many pages are allocated, we can check the PTE_P
 flag.

 Related files

 - <usys.S>  SYSCALL(getmeminfo)
 - <syscall.h>  define SYS_getmeminfo 23
 - <syscall.c>  extern and add to vector
 - <user.h>  declaration
 - <sysproc.c>  pass parameters and invoke getprocinfo()
 - <proc.c>  implementation of getprocinfo() and walkleaves()
 - <defs.h>  declare getprocinfo()
 - <mtop.c>  call getmeminfo()

##### Program Usage

```
   ...                                    
   $ cd xv6-public/                       
   $ make clean && make && make qemu-nox  
   ...                                    
   $ mtop                                 
   ...                                    
 ```

##### Result:

    available memory: 232607744
    pid: 1	name: init	mem: 286720
    pid: 2	name: sh	mem: 290816
    pid: 3	name: mtop	mem: 286720

## POSIX Threads and locks, Synchronization

 Implementation
 *    <thread.c>       main test template
 *    <threadlock.c>   define spinlocks and mutex for threads
 *    <threadlock.h>   header file for threadlock
 *    <proc.c>         implementations of thread_create(), thread_join, and thread_exit()
 *    <proc.h>         add thread flag into struct proc

 linking
 *    <syscall.c>      add system call vector
 *    <syscall.h>      add system call number
 *    <user.h>         connect thread functions to user
 *    <usys.S>         add SYSCALL
 *    <sysproc.c>      connect thread functions to system call
 *    <defs.h>         declare thread functions
