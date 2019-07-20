struct thread_spinlock{
	char *name;
	uint locked;       // Is the lock held?
};
struct thread_mutex{
	char *name;
	uint locked;       // Is the lock held?
};


void thread_spin_init(struct thread_spinlock *lk, char *name);
void thread_spin_lock(struct thread_spinlock *lk);
void thread_spin_unlock(struct thread_spinlock *lk);


void thread_mutex_init(struct thread_mutex *m, char *name);
void thread_mutex_lock(struct thread_mutex *m);
void thread_mutex_unlock(struct thread_mutex *m);
