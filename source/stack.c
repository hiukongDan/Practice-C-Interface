#include <stddef.h>
#include "stack.h"
#include "mem.h"
#include "assert.h"

#define T Stack_T

struct T{
	int count;
	struct elem{
		void* x;
		struct elem* link;
	} *head;
};

T Stack_new(void){
	T stk;
	
	NEW(stk);
	stk->count = 0;
	stk->head = NULL;
	return stk;
}

// return 1 if stack is empty
int Stack_empty(T stk){
	assert(stk);
	return stk->count == 0;
}

void Stack_push(T stk, void* x){
	assert(stk);

	struct elem *t;
	NEW(t);
	t->elem = x;
	t->link = stk->head;
	stk->head = t;
	stk->count++;
}

void* Stack_pop(T stk){
	assert(stk);
	assert(stk->count > 0);
	
	void* x = stk->head->x;
	struct elem* t = stk->head;
	stk->head = stk->head->link;
	FREE(t);
	stk->count--;
	return x;
}

void Stack_free(T* stk){
	assert(stk && *stk);
	
	struct elem *t, *u;
	for(t = (*stk)->head; t; t = u){
		u = t->link;
		FREE(t);
	}
	FREE(*stk);
}