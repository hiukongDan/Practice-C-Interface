#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdarg.h>

#include "mem.h"
#include "assert.h"
#include "ring.h"


#define T Ring_T

T Ring_new(void){
	T ring;
	NEW0(ring);
	ring->head = NULL;
	return ring;
}

T Ring_ring(void* x, ...){
	T ring = Ring_new();
	va_list ap;
	for(va_start(ap, x); x; x = va_arg(ap, void*)){
		Ring_addhi(ring, x);
	}
	va_end(ap);
	return ring;
}

void Ring_free(T *ring){
	assert(ring && *ring);
	struct node *q, *p = (*ring)->head;
	int i;
	for(i = 0; i < (*ring)->length; i++){
		q = p;
		p = p->rlink;
		FREE(q);
	}
	FREE(*ring);
	*ring = NULL;
}

int Ring_length(T ring){
	assert(ring);
	return ring->length;
}

void* Ring_get(T ring, int index){
	assert(ring);
	assert(index >= 0 && index < ring->length);
	struct node *p = ring->head;
	while(--index >= 0){
		p = p->rlink;
	}
	return p->value;
}

void* Ring_put(T ring, int index, void* x){
	assert(ring);
	assert(index >= 0 && index < ring->length);
	struct node *p = ring->head;
	void* tmp;
	while(--index >= 0){
		p = p->rlink;
	}
	tmp = p->value;
	p->value = x;
	return tmp;
}

void* Ring_add(T ring, int index, void* x){
	assert(ring);
	assert(index <= ring->length+1 && index >= -ring->length);
	struct node *p, *node;
	if(index == 1 || index == -ring->length){
		return Ring_addlo(ring, x);
	}
	else if(index == ring->length+1 || index == 0){
		return Ring_addhi(ring, x);
	}
	else{
		index = (index + ring->length) % ring->length;
		p = ring->head;
		while(--index >= 0){
			p = p->rlink;
		}
		NEW0(node);
		node->llink = p;
		node->rlink = p->rlink;
		p->rlink->llink = node;
		p->rlink = node;
		ring->length++;
		return node->value = x;
	}
}

void* Ring_addhi(T ring, void* x){
	assert(ring);
	struct node *p, *q;
	NEW0(p);
	p->value = x;
	if((q = ring->head) == NULL){
		ring->head = p->llink = p->rlink = p;
	}else{
		q = q->llink;
		q->rlink = p;
		p->llink = q;
		p->rlink = ring->head;
		ring->head->llink = p;
	}
	ring->length++;
	return p->value;
}

void* Ring_addlo(T ring, void* x){
	Ring_addhi(ring, x);
	ring->head = ring->head->llink;
	return x;
}

void* Ring_remove(T ring, int index){
	assert(ring);
	assert(index >= 0 && index < ring->length);
	
	struct node *p = ring->head;
	while(--index >= 0){
		p = p->rlink;
	}
	p->llink->rlink = p->rlink;
	p->rlink->llink = p->llink;
	void* x = p->value;
	if(ring->head == p){
		ring->head = p->rlink;
	}
	FREE(p);
	ring->length--;
	return x;
}

void* Ring_remhi(T ring){
	assert(ring);
	assert(ring->length > 0);
	struct node *p = ring->head, *q = ring->head->llink;
	void* tmp;
	p->llink = q->llink;
	q->llink->rlink = p;
	tmp = q->value;
	FREE(q);
	if(--ring->length == 0){
		ring->head = NULL;
	}
	return tmp;
}

void* Ring_remlo(T ring){
	assert(ring);
	assert(ring->length > 0);
	struct node *p = ring->head, *q = ring->head->rlink;
	void* tmp;
	p->llink->rlink = q;
	q->llink = p->llink;
	tmp = p->value;
	FREE(p);
	if(--ring->length == 0){
		ring->head = NULL;
	}
	else{
		ring->head = q;
	}
	return tmp;
}

void Ring_rotate(T ring, int n){
	assert(ring);
	assert(n<= ring->length && n >= -ring->length);
	if(n>=0){
		n = n % ring->length;
	}
	else{
		n = n + ring->length;
	}
	struct node *p = ring->head;
	if(n < ring->length/2){
		while(--n >= 0){
			p = p->rlink;
		}
	}
	else{
		n = ring->length - n;
		while(--n >= 0){
			p = p->llink;
		}
	}
	ring->head = p;
}