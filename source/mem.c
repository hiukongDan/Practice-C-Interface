#include "mem.h"
#include "assert.h"
#include "except.h"
#include <stdlib.h>

const Except_T Mem_Failed = {"Allocation Failed"};

void* Mem_Alloc(long nbytes, const char* file, int line){
	assert(nbytes > 0);
	void* p = malloc(nbytes);
	if (p == NULL){
		if (file == NULL)
			RAISE(Mem_Failed);
		else
			Except_raise(&Mem_Failed, file, line);
	}
	return p;
}

void* Mem_Calloc(long count, long nbytes, const char* file, int line){
	assert(count > 0);
	assert(nbytes > 0);
	void *p = calloc(count, nbytes);
	if (p == NULL){
		if (file == NULL)
			RAISE(Mem_Failed);
		else
			Except_raise(&Mem_Failed, file, line);
	}
	return p;
}

void Mem_Free(void* p, const char* file, int line){
	if(p) free(p);
}

void* Mem_Resize(void* p, long nbytes, const char* file, int line){
	assert(p);
	assert(nbytes > 0);
	p = realloc(p, nbytes);
	if(p == NULL){
		if (file == NULL)
			RAISE(Mem_Failed);
		else
			Except_raise(&Mem_Failed, file, line);
	}
	return p;
}
