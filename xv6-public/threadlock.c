#include "types.h"
#include "stat.h"
#include "user.h"
#include "threadlock.h"

uint
myxchg(volatile uint *addr, uint newval)
{
	uint result;
	// The + in "+m" denotes a read−modify−write operand.
	asm volatile("lock; xchgl %0, %1" :
							 "+m" (*addr), "=a" (result) :
							 "1" (newval) :
				 		 	 "cc");
	return result;
}


void
thread_spin_init(struct thread_spinlock *lk, char *name)
	{
	lk->name = name;
	lk->locked = 0;
}

void
thread_spin_lock(struct thread_spinlock *lk)
{
	while(myxchg(&lk->locked, 1) != 0)
		;
	__sync_synchronize();
}

void
thread_spin_unlock(struct thread_spinlock *lk)
{
	if(!lk->locked){
		printf(1, "The lock is not acquired\n");
		return;
	}
	__sync_synchronize();
	asm volatile("movl $0, %0" : "+m" (lk->locked) : );
}


void
thread_mutex_init(struct thread_mutex *m, char *name)
{
	m->name = name;
	m->locked = 0;
}

void
thread_mutex_lock(struct thread_mutex *m)
{
	while(myxchg(&m->locked, 1) != 0)
		sleep(1);
	__sync_synchronize();
}

void
thread_mutex_unlock(struct thread_mutex *m)
{
	if(!m->locked)
		return;
	__sync_synchronize();
	asm volatile("movl $0, %0" : "+m" (m->locked) : );
}
