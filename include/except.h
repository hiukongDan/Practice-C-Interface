#ifndef EXCEPT_INCLUDED
#define EXCEPT_INCLUDED

#include <setjmp.h>

#define T Except_T

typedef struct T{
	const char* reason;
} T;


typedef struct Except_Frame Except_Frame;
struct Except_Frame{
	Except_Frame *pre;
	jmp_buf env;
	const T *exception;
	const char* file;
	int line;
};


enum{
	Except_entered = 0,
	Except_raised,
	Except_finalized,
	Except_handled,
};


extern Except_Frame *Except_stack;

extern void Except_raise(const T *e, const char* file, int line);
// extern void Except_stack_trace();

#define RAISE(e) Except_raise(&(e), __FILE__, __LINE__)
#define RERAISE Except_raise(Except_frame.exception, Except_frame.file, Except_frame.line)
#define RETURN switch(Except_stack=Except_stack->pre, 0) default: return

#define TRY do{\
	Except_Frame Except_frame;\
	Except_frame.pre = Except_stack;\
	Except_stack = &Except_frame;\
	volatile int Except_flag;\
	Except_flag = setjmp(Except_frame.env);\
	if (Except_flag == Except_entered){
	
	
#define EXCEPT(e)\
		if(Except_flag == Except_entered){\
			Except_stack = Except_stack->pre;\
		}\
	} else if(Except_frame.exception == &(e)){\
		Except_flag = Except_handled;


#define ELSE\
		if(Except_flag == Except_entered){\
			Except_stack = Except_stack->pre;\
		}\
	} else{\
		Except_flag = Except_handled;
		
	
#define FINALLY\
		if(Except_flag == Except_entered){\
			Except_stack = Except_stack->pre;\
		}\
	}\
	{\
		Except_flag = Except_finalized;
		

#define END_TRY\
		if(Except_flag == Except_entered){\
			Except_stack = Except_stack->pre;\
		}\
	}\
		if(Except_flag == Except_raised){\
			Except_raise(Except_frame.exception,\
				Except_frame.file, Except_frame.line);\
		}\
	}while(0)
	

#undef T
#endif