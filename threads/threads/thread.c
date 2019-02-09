#include <assert.h>
#include <stdlib.h>
#include <ucontext.h>
#include "thread.h"
#include "interrupt.h"

/* This is the wait queue structure */
struct wait_queue {
	/* ... Fill this in Lab 3 ... */
};

/* This is the thread control block */
struct thread {
	Tid t_id;
	struct ucontext* t_ucontext;
};typedef struct thread thread;

/* This is the ready queue which holds all awaiting threads*/
struct readyQ {
	thread* q_thread;
	struct readyQ* q_next;
};typedef struct readyQ readyQ;

//global variables
readyQ* q_head;		//head of the ready queue
thread* cr_thread;	//currently running thread
Tid cr_tid;
volatile int setcontext_called = 0;

static void
call_setcontext(ucontext_t * context)
{
	int err = setcontext(context);
	assert(!err);
}

/* thread starts by calling thread_stub. The arguments to thread_stub are the
 * thread_main() function, and one argument to the thread_main() function. */
void
thread_stub(void (*thread_main)(void *), void *arg)
{
	Tid ret;

	thread_main(arg); // call thread_main() function with arg
	ret = thread_exit();
	// we should only get here if we are the last thread. 
	assert(ret == THREAD_NONE);
	// all threads are done, so process should exit
	exit(0);
}

// static void
// call_setcontext(ucontext_t * context)
// {
// 	int err = setcontext(context);
// 	assert(!err);
// }



void thread_init(void)
{
	//allocate the currently running thread and initialize to NULL since no registers 
	//are currently being used


	int err = 1;
	cr_thread = (thread*)malloc(sizeof(thread));
	cr_thread->t_ucontext = (ucontext_t*)malloc( sizeof(struct ucontext) );
	cr_thread->t_id = 0;
	err = getcontext(cr_thread->t_ucontext);
	assert(!err);
	// cr_thread->t_ucontext = NULL;
	// cr_thread->t_id = 0;
	// ucontext_t mycontext = { 0 };
	// cr_thread->t_ucontext = &mycontext;

	// int err;
	// err = getcontext(cr_thread->t_ucontext);
	//initialize currently running thread id to 0;
	// cr_tid = err;
}

Tid thread_id()
{
	return cr_thread->t_id;
	return THREAD_INVALID;
}

Tid thread_create(void (*fn) (void *), void *parg)
{
	TBD();
	return THREAD_FAILED;
}

Tid thread_yield(Tid want_tid)
{
	
	//suspend current thread and put it into the readyQ
	//change context to want_tid and run it

	//yield to itself
	if(want_tid == 16){
		return THREAD_INVALID;
	}
	
	if(cr_thread->t_ucontext == NULL){
		cr_thread->t_ucontext = (struct ucontext*)malloc(sizeof(struct ucontext));
	}

	// int err;
	if(want_tid == THREAD_SELF || want_tid == cr_thread->t_id){
		// if(want_tid == 0)
		// 	return 0;
		
		// //follow the  same code from show_ucontext
		volatile int w = 0;
		int err;
		err = getcontext(cr_thread->t_ucontext);
		assert(!err);

		if(w == 1){
			return 0;
		}
		
		w = 1;
		// err = setcontext(cr_thread->t_ucontext);
		call_setcontext(cr_thread->t_ucontext);
		// return want_tid;
		
	}
	else if(want_tid == THREAD_ANY){
		//DO SOMETHING

		//IF NOT POSSIBLE, return THREAD_NONE
		return THREAD_NONE;
	}
	else if(want_tid > (THREAD_MAX_THREADS - 1)){
		return THREAD_INVALID;
	}
	else if(want_tid < 0){
		return THREAD_INVALID;
	}
	else if(want_tid >= 0 || want_tid <= (THREAD_MAX_THREADS-1) ){
		return 16;
	}
	else{
		
		printf("wtf");
		return THREAD_FAILED;
	}

	return THREAD_FAILED;
}

Tid thread_exit()
{
	TBD();
	return THREAD_FAILED;
}

Tid thread_kill(Tid tid)
{
	TBD();
	return THREAD_FAILED;
}

/*******************************************************************
 * Important: The rest of the code should be implemented in Lab 3. *
 *******************************************************************/

/* make sure to fill the wait_queue structure defined above */
struct wait_queue *
wait_queue_create()
{
	struct wait_queue *wq;

	wq = malloc(sizeof(struct wait_queue));
	assert(wq);

	TBD();

	return wq;
}

void
wait_queue_destroy(struct wait_queue *wq)
{
	TBD();
	free(wq);
}

Tid
thread_sleep(struct wait_queue *queue)
{
	TBD();
	return THREAD_FAILED;
}

/* when the 'all' parameter is 1, wakeup all threads waiting in the queue.
 * returns whether a thread was woken up on not. */
int
thread_wakeup(struct wait_queue *queue, int all)
{
	TBD();
	return 0;
}

/* suspend current thread until Thread tid exits */
Tid
thread_wait(Tid tid)
{
	TBD();
	return 0;
}

struct lock {
	/* ... Fill this in ... */
};

struct lock *
lock_create()
{
	struct lock *lock;

	lock = malloc(sizeof(struct lock));
	assert(lock);

	TBD();

	return lock;
}

void
lock_destroy(struct lock *lock)
{
	assert(lock != NULL);

	TBD();

	free(lock);
}

void
lock_acquire(struct lock *lock)
{
	assert(lock != NULL);

	TBD();
}

void
lock_release(struct lock *lock)
{
	assert(lock != NULL);

	TBD();
}

struct cv {
	/* ... Fill this in ... */
};

struct cv *
cv_create()
{
	struct cv *cv;

	cv = malloc(sizeof(struct cv));
	assert(cv);

	TBD();

	return cv;
}

void
cv_destroy(struct cv *cv)
{
	assert(cv != NULL);

	TBD();

	free(cv);
}

void
cv_wait(struct cv *cv, struct lock *lock)
{
	assert(cv != NULL);
	assert(lock != NULL);

	TBD();
}

void
cv_signal(struct cv *cv, struct lock *lock)
{
	assert(cv != NULL);
	assert(lock != NULL);

	TBD();
}

void
cv_broadcast(struct cv *cv, struct lock *lock)
{
	assert(cv != NULL);
	assert(lock != NULL);

	TBD();
}
