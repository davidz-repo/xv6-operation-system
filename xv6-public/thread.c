#include "types.h"
#include "stat.h"
#include "user.h"

/* hw4 - threads and locks
 *
 * implementation
 *    <thread.c>      | main test template
 *    <threadlock.c>  | define spinlocks and mutex for threads
 *    <threadlock.h>  | header file for threadlock
 *    <proc.c>        | implementations of thread_create(), thread_join, and thread_exit()
 *    <proc.h>        | add thread flag into struct proc
 *
 * linking
 *    <syscall.c>     | add system call vector
 *    <syscall.h>     | add system call number
 *    <user.h>        | connect thread functions to user
 *    <usys.S>        | add SYSCALL
 *    <sysproc.c>     | connect thread functions to system call
 *    <defs.h>        | declare thread functions
 *    <Makefile>      | add to OBJS, UPROGS, EXTRA
 */

#include "threadlock.h"

struct thread_spinlock lock_s; // Test 1 - spinlock
struct thread_mutex lock_m; // Test 2 - mutex

struct balance {
	char name[32];
	int amount;
};

volatile int total_balance = 0;

volatile unsigned int delay (unsigned int d) {
	unsigned int i;
	for (i = 0; i < d; i++) {
		__asm volatile( "nop" ::: );
	}
	
	return i;
}

void do_work(void *arg){
	int i;
	int old;
	
	struct balance *b = (struct balance*) arg;
	printf(1, "Starting do_work: s:%s\n", b->name);
	
	for (i = 0; i < b->amount; i++) {
		
		thread_spin_lock(&lock_s); // Test 1 - spinlock
		
		thread_mutex_lock(&lock_m); //  Test 2 - mutex
		
		old = total_balance;
		delay(100000);
		total_balance = old + 1;
		
		thread_spin_unlock(&lock_s); // Test 1 - spinlock
		
		thread_mutex_unlock(&lock_m); // Test 2 - mutex

	}
	
	printf(1, "Done s:%s\n", b->name);
	
	thread_exit();
	return;
}

int main(int argc, char *argv[]) {
	
	struct balance b1 = {"b1", 3200};
	struct balance b2 = {"b2", 2800};
	
	void *s1, *s2;
	int t1, t2, r1, r2;
	
	s1 = malloc(4096);
	s2 = malloc(4096);
	
	t1 = thread_create(do_work, (void*)&b1, s1);
	t2 = thread_create(do_work, (void*)&b2, s2);
	
	r1 = thread_join();
	r2 = thread_join();
	
	printf(1, "Threads finished: (%d):%d, (%d):%d, shared balance:%d\n",
	       t1, r1, t2, r2, total_balance);
	
	exit();
}
