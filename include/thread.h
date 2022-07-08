#ifndef THREAD_INCLUDED
#define THREAD_INCLUDED

#include "except.h"

#define T Thread_T

typedef struct T *T;

extern const Except_T Thread_Failed;
extern const Except_T Thread_Alerted;

extern int Thread_init(T preempt, ...);
extern T Thread_new(int apply(void*), void *args, int nbytes, ...);
extern int Thread_exit(int code);
extern T Thread_self(void);
extern void Thread_pause(void);
extern void Thread_join(T t);
extern void Thread_alert(T t);


#undef T
#endif