#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include </usr/include/signal.h>
#include <sys/time.h>
#include "assert.h"
#include "mem.h"
#include "thread.h"
#include "sem.h"

void _MONITOR(void){}
extern void _ENDMONITOR(void);

#define T Thread_T

// macros
#define isempty(q) ((q) == NULL)

// types
struct T{
	unsigned long *sp;
	// fields
	T link;
	T *inqueue;
	T handle;
	Except_stack *estack;
	int code;
	T join;
	int alerted;
};

// data
static T ready = NULL;
static T current;
static T join0;
static struct Thread_T root;
static int nthreads;
static int critical;
const Except_T Thread_Alerted = {"Thread alerted"};
const Except_T Thread_Failed = {"Thread creation failed"};

// prototypes
extern void _swtch(T from, T to);

// static functions
static void put(T t, T *q){
	assert(t);
	assert(t->inqueue == NULL && t->link == NULL);
	if(*q){
		t->link = (*q)->link;
		(*q)->link = t;
	} else{
		t->link = t;
	}
	*q = t;
	t->inqueue = q;
}

static T get(T *q){
	T t;
	
	assert(!isempty(*q));
	t = (*q)->link;
	if (t == *q)
		*q = NULL;
	else
		(*q)->link = t->link;
	assert(t->inqueue == q);
	t->link = NULL;
	t->inqueue = NULL;
	return t;
}

static void delete(T t, T *q){
	T p;
	
	assert(t->link && t->inqueue == q);
	assert(!isempty(*q));
	for(p = *q; p->link != t; p = p->link)
		;
	if(p == t)
		*q = NULL;
	else{
		p->link = t->link;
		if(*q == t)
			*q = p;
	}
	t->link = NULL;
	t->inqueue = NULL;
}

static void run(void){
	T t = current;
	
	current = get(&ready);
	t->estack = Except_stack;
	Except_stack = current->estack;
	_swtch(t, current);
}

static void testalert(void){
	if(current->alerted){
		current->alerted = 0;
		RAISE(Thread_Alerted);
	}
}

static void release(void){
	T t;
	
	do{ critical++;
		
		while((t = freelist) != NULL){
			freelist = t->next;
			FREE(t);
		}
		
	cirtical--;}while(0);
}

static int interrupt(int sig, int code, struct sigcontext *scp){
	if(critical ||
		scp->sc_pc >= (unsigned long)_MONITOR
	 && scp->sc_pc <= (unsigned long)_ENDMONITOR)
		return 0;
	
	put(current, &ready);
	sigsetmask(scp->sc_mask);
	run();
	return 0;
}

// thread functions
int Thread_init(int preempt, ...){
	assert(preempt == 0 || preempt == 1);
	assert(current == NULL);
	root.handle = &root;
	current = &root;
	nthreads =  1;
	if(preempt){
		struct sigaction sa;
		memset(&sa, '\0', sizeof sa);
		sa.sa_handler = (void(*)())interrupt;
		if(sigaction(SIGVTALRM, &sa, NULL) < 0)
			return 0;
		
		struct itimerval it;
		it.it_value.tv_sec = 0;
		it.it_value.tv_usec = 50;
		it.it_interval.tv_sec = 0;
		it.it_interval.tv_usec = 50;
		if(setitimer(ITIMER_VIRTUAL, &it, NULL) < 0)
			return 0;
	}
	return 1;
}

T Thread_new(int apply(void*), void *args, int nbytes, ...){
	T t;
	
	assert(current);]
	assert(apply);
	assert(args && nbytes >= 0 || args == NULL);
	if(args == NULL)
		nbytes = 0;
	
	int stacksize = (16 * 1024 + sizeof (*t) + nbytes + 15) & ~15;
	release();
	do{critical++;
		TRY
			t = ALLOC(stacksize);
			memset(t, '\0', sizeof *t);
		EXCEPT(Mem_Failed)
			t = NULL;
		END_TRY;
	critical--;}while(0);
	if (t == NULL)
		RAISE(Thread_Failed);
	
	t->sp = (void*)((char*)t + stacksize);
	while(((unsigned long)t->sp)&15)
		t->sp--;
	
	t->handle = t;
	if(nbytes > 0){
		t->sp -= ((nbytes + 15U)&~15)/sizeof (*t->sp);
		do{critical++;
			memcpy(t->sp, args, nbytes);
		critical--;}while(0);
		args = t->sp;
	}
	#if alpha
		extern void _start(void);
		t->sp -= 112/8;
		t->sp[(48+24)/8] = (unsigned long)Thread_exit;
		t->sp[(48+16)/8] = (unsigned long)args;
		t->sp[(48+8)/8] = (unsigned long)apply;
		t->sp[(48+0)/8] = (unsigned long)_start;
	#elif mips
		extern void _start(void);
		t->sp -= 16/4;
		t->sp -= 88/4;
		t->sp[(48+20)/4] = (unsigned long)Thread_exit;
		t->sp[(48+28)/4] = (unsigned long)args;
		t->sp[(48+32)/4] = (unsigned long)apply;
		t->sp[(48+36)/4] = (unsigned long)_start;
	#elif sparc
		int i; void *fp; extern void _start(void);
		for(i = 0; i < 8; i++)
			*--t->sp = 0;
		*--t->sp = (unsigned long)args;
		*--t->sp = (unsigned long)apply;
		t->sp -= 64/4;
		fp = t->sp;
		*--t->sp = (unsigned long)_start - 8;
		*--t->sp = (unsigned long)fp;
		t->sp -= 64/4;
	#else
		Unsupported platform
	#endif
	
	nthreads++;
	put(t, &ready);
	return t;
}

int Thread_exit(int code){
	assert(current);
	release();
	if(current != &root){
		current->next = freelist;
		freelist = current;
	}
	current->handle = NULL;
	while(!isempty(current->join)){
		T t = get(&current->join);
		t->code = code;
		put(t, &ready);
	}
	if(!isempty(join0) && nthreads == 2){
		assert(isempty(ready));
		put(get(&join0), &ready);
	}
	
	if(--nthreads == 0)
		exit(code);
	else
		run();
}

T Thread_self(){
	assert(current);
	return current;
}

void Thread_pause(void){
	assert(current);
	put(current, &ready);
	run();
}

int Thread_join(T t){
	assert(current && current != t);
	testalert();
	if(t){
		if(t->handle == t){
			put(current, &t->join);
			run();
			testalert();
			return current->code;
		}else{
			return -1;
		}
	}else{
		assert(isempty(join0));
		if(nthreads > 1){
			put(current, &join0);
			run();
			testalert();
		}
	}
}

void Thread_alert(T t){
	assert(current);
	assert(t && t->handle == t);
	t->alerted = 1;
	if (t->inqueue){
		delete(t, t->inqueue);
		put(t, &ready);
	}
}



#undef T



#define T Sem_T

// sem functions
T* Sem_new(int count){
	T *s;
	
	NEW(s);
	Sem_init(s, count);
	return s;
}

void Sem_init(T *s, int count){
	assert(current);
	assert(s);
	s->count = count;
	s->queue = NULL;
}

void Sem_wait(T *s){
	assert(current);
	assert(s);
	testalert();
	if(s->count <= 0){
		put(current, (Thread_T*)&s->queue);
		run();
		testalert();
	}
	else{
		--s->count;
	}
}

void Sem_signal(T *s){
	assert(current);
	assert(s);
	if(s->count == 0 && !isempty(s->queue)){
		Thread_T t = get((Thread_T*)&s->queue);
		assert(!t->alerted);
		put(t, &ready);
	}else{
		++s->count;
	}
}

#endef T