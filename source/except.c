#include "except.h"
#include <stdio.h>
#include <assert.h>

#define T Except_T

Except_Frame *Except_stack = NULL;

void Except_raise(const T *e, const char* file, int line){	
	Except_Frame *p = Except_stack;
	assert(e);
	
	if(Except_stack == NULL){
		fprintf(stderr, "Uncaught exception ");
		if(e->reason){
			fprintf(stderr, e->reason);
		}else{
			fprintf(stderr, " at 0x%p", e);
		}
		fprintf(stderr, " in %s:%d\n", file, line);
		fflush(stderr);
	}
	
	p->exception = e;
	p->file = file;
	p->line = line;
	
	//Except_stack_trace();
	
	Except_stack = Except_stack->pre;
	longjmp(p->env, Except_raised);
}

void Except_stack_trace(){
	Except_Frame *p = Except_stack;
	if(p){
		fprintf(stderr, "Exception ");
	}
	while(p){
		fprintf(stderr, "\n\t");
		if(p->exception->reason){
			fprintf(stderr, p->exception->reason);
		}
		else{
			fprintf(stderr, "at 0x%p", p->exception);
		}
		fprintf(stderr, " in %s:%d.", p->file, p->line);
		p = p->pre;
	}
	fflush(stderr);
}

#undef T
