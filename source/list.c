#include "list.h"
#include "mem.h"
#include "assert.h"
#include <stdarg.h>
#include <stdlib.h>

#define T List_T

T List_append(T list, T tail){
	T *p = &list;
	while (*p){
		p = &(*p)->rest;
	}
	*p = tail;
	return list;
}

T List_push(T list, void *x){
	T p;
	NEW(p);
	p->rest = list;
	p->first = x;
	return p;
}

T List_pop(T list, void **x){
	if (list){
		if(x){
			*x = list->first;
		}
		T p = list->rest;
		FREE(list);
		return p;
	}
	else{
		return list;
	}
}

long List_length(T list){
	long len = 0;
	for (; list; list = list->rest){
		len++;
	}
	return len;
}

T List_list(void *x, ...){
	va_list ap;
	va_start(ap, x);
	T list, *p = &list;
	for (; x; x = va_arg(ap, void*)){
		NEW(*p);
		(*p)->first = x;
		p = &(*p)->rest;
	}
	*p = NULL;
	va_end(ap);
	return list;
}

void List_free(T *list){
	T next;
	assert(list);
	
	for (; *list; *list = next){
		next = (*list)->rest;
		FREE(*list);
	}
}

T List_reverse(T list){
	T curr = list;
	T pre = (void*)0;
	do{
		T tmp = curr;
		curr = curr->rest;
		tmp->rest = pre;
		pre = tmp;
	}
	while (curr);
	list = pre;
	return list;
}

void List_map(T list, void apply(void **x, void *cl), void *cl){
	assert(apply);
	for(; list; list = list->rest){
		apply(&list->first, cl);
	}
}

T List_copy(T list){
	T head, *p = &head;
	for(; list; list = list->rest){
		NEW(*p);
		(*p)->first = list->first;
		p = &(*p)->rest;
	}
	*p = NULL;
	return head;
}

void** List_toArray(T list, void* end){
	long i, len = List_length(list);
	void** array = ALLOC((len+1)*sizeof(*array));
	
	for (i = 0; i < len; i++){
		array[i] = list->first;
		list = list->rest;
	}
	array[i] = end;
	return array;
}