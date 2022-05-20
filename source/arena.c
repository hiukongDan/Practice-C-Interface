#include "arena.h"
#include "except.h"
#include "assert.h"
#include <stdlib.h>
#include <string.h>

#define T Arena_T
#define THRESHOLD 10

const Except_T Arena_NewFailed = {"Arena Creation Failed"};
const Except_T Arena_Failed = {"Arena Allocation Failed"};

static T freetrunks;
static int nfree;

struct T{
	T pre;
	char *avail;
	char *limit;
};

union align{
	int i;
	long l;
	long *lp;
	void *vp;
	void (*fp)(void);
	float f;
	double d;
	long double ld;
};

struct header{
	T p;
	union align a;
};

T Arena_new(void){
	T ap = malloc(sizeof *ap);
	if (ap == NULL){
		RAISE(Arena_NewFailed);
	}
	ap->pre = NULL;
	ap->avail = NULL;
	ap->limit = NULL;
	return ap;
}


void Arena_dispose(T *ap){
	assert(ap);
	assert(*ap);
	Arena_free(*ap);
	free(ap);
	*ap = NULL;
}

void* Arena_alloc(T arena, long nbytes, const char *file, long line){
	assert(arena);
	assert(nbytes > 0);
	nbytes = (nbytes + sizeof(union align) - 1) * sizeof(union align) * sizeof(union align);
	while (nbytes > arena->limit - arena->avail){
		T ptr;
		void* limit;
		if((ptr = freetrunks) != NULL){
			freetrunks = freetrunks->pre;
			limit = ptr->limit;
			*ptr = *arena;
			arena->pre = ptr;
			arena->limit = limit;
			arena->avail = (void*)((struct header*)arena->pre + 1);
		}
		else{
			long m = nbytes + 10 * 1024 + sizeof(struct header);
			ptr = (T)malloc(m);
			limit = (void*)ptr + m;
			*ptr = *arena;
			arena->pre = ptr;
			arena->avail = (void*)((struct header*)arena->pre + 1);
			arena->limit = limit;
		}
	}
	arena->avail += nbytes;
	return arena->avail - nbytes;
}

void* Arena_calloc(T arena, long count, long nbytes, const char *file, long line){
	assert(arena);
	assert(count > 0);
	assert(nbytes > 0);
	void *prt = Arena_alloc(arena, count*nbytes, file, line);
	memset(prt, '\0', count*nbytes);
	return prt;
}

void Arena_free(T arena){
	assert(arena);
	while (arena->pre != NULL){
		struct T tmp = *arena->pre;
		if(nfree <= THRESHOLD){
			arena->pre->pre = freetrunks;
			freetrunks = arena->pre;
			nfree++;
			freetrunks->limit = arena->limit;
		}
		else{
			free(arena->pre);
		}
		*arena = tmp;
	}
	assert(arena->limit == NULL);
	assert(arena->avail == NULL);
}