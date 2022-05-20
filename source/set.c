#include "set.h"
#include "except.h"
#include "mem.h"
#include "assert.h"
#include <stdlib.h>
#include <limits.h>

#define Set_T T

struct T{
	int length;
	unsigned timestamp;
	int size;
	int (*cmp)(const void* a, const void* b);
	unsigned (*hash)(const void* member);
	struct member{
		struct member* link;
		const void* member;
	} **buckets;
};



static int cmpatom(const void* a, const void* b){
	return a != b;
}

static unsigned cmphash(const void* member){
	return (unsigned long)member >> 2;
}

T Set_new(int hint, int cmp(const void* a, const void* b), unsigned hash(const void* member)){
	T set;
	static int primes[] = {509, 509, 1021, 2039, 4093, 8191, 16381, 32749, 65521, INT_MAX};
	
}