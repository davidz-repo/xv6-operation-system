/* David Zhang
 * CS 238P - HW3 - System Call

 * Part 1 - basic <int memtop(void);>
    *******************************
    * $ mtop                      *
	* available memory: 232607744 *
    *******************************
    *
    * == files changed ==
    * <sysproc.c>   implementation of int sys_memtop(void)
    * <defs.h>      declare int pagesleft(void)
    * <usys.S>      add to SYSCALL(memtop)
    * <user.h>      declare int memtop(void)
    * <syscall.h>   define SYS_memtop as call 22
    * <syscall.c>   extern sys_memtop, add to vector [SYS_memtop] sys_memtop
    * <kalloc.c>    Implementation of int pagesleft(void)

 
 * Part 2 - extension <int getmeminfo(int pid, char *name, int len);>
    * pid - int
    * name - a pointer to a buffer where the syscall using pid to copy the proc name
    * len - length of the name buffer
    * return - bytes of the pid/name
    *
    * == files changed ==
    * <usys.S> - SYSCALL(getmeminfo)
    * <syscall.h> - define SYS_getmeminfo 23
    * <syscall.c> - extern and add to vector
    * <user.h> - declaration
    * <sysproc.c> - pass parameters and invoke getprocinfo()
    * <proc.c> - implementation of getprocinfo() and walkleaves()
    * <defs.h> - declare getprocinfo()
    * <mtop.c> - call getmeminfo()
 */

#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  printf(1,"available memory: %d\n", memtop());
  int max_pid = fork();
  if(max_pid>0){
    wait();
  }
  else{
    max_pid = getpid();
    int i = 1, len = 16;
    char name[len];
    for (; i < max_pid; ++i) {
      int procsz = getmeminfo(i, name, len);
      if(procsz)
        printf(1, "pid: %d\tname: %s\tmem: %d\n", i, name, procsz);
    }
  }
  exit();
}
