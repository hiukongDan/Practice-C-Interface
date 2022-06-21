#include "seq.h"
#include "array.h"
#include "arrayrep.h"
#include "mem.h"
#include "assert.h"
#include "except.h"


#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define T Seq_T

struct T{
	struct Array_T array;
	int length;
	int head;
};

static void expand(T seq){
	assert(seq);
	assert(&seq->array);
	
	int n = seq->array.length;
	Array_resize(&seq->array, n*2);
	if(seq->head > 0){
		void** old = (void**)&seq->array.array[seq->head];
		memcpy(old + n, old, sizeof(void*) * (n-seq->head));
		seq->head += n;
	}
}

T Seq_new(int hint){
	if(hint == 0){
		hint = 16;
	}
	T seq;
	NEW0(seq);
	ArrayRep_init(&seq->array, hint, sizeof(void*),
		ALLOC(hint * sizeof(void*)));
	return seq;
}

T Seq_seq(void* x, ...){
	va_list ap;
	T s = Seq_new(0);
	
	for(va_start(ap, x); x; x = va_arg(ap, void*)){
		Seq_addhi(s, x);
	}
	
	return s;
}

void Seq_free(T *seq){
	assert(seq && *seq);
	assert((void*)*seq == (void*)&(*seq)->array);
	Array_free((Array_T*)seq);
}

void* Seq_put(T seq, int i, void* x){
	assert(seq);
	assert(i>=0 && i < seq->length);
	return ((void**)seq->array.array)[(seq->head+i)%seq->length] = x;
}

void* Seq_get(T seq, int i){
	assert(seq);
	assert(i>=0 && i < seq->length);
	return ((void**)seq->array.array)[(seq->head+i)%seq->length];
}

int Seq_length(T seq){
	assert(seq);
	return seq->length;
}

void* Seq_addlo(T seq, void* x){
	assert(seq);
	if(seq->length == seq->array.length){
		expand(seq);
	}
	if(--seq->head ==  0)
		seq->head = seq->array.length-1;
	seq->length++;
	return ((void**)seq->array.array)[seq->head] = x;
}

void* Seq_addhi(T seq, void* x){
	assert(seq);
	if(seq->length == seq->array.length){
		expand(seq);
	}
	int i = seq->length++;
	return ((void**)seq->array.array)[(seq->head+i) % seq->array.length] = x;
}

void* Seq_remlo(T seq){
	assert(seq);
	assert(seq->length > 0);
	int i = seq->head++;
	if(seq->head == seq->array.length)
		seq->head = 0;
	seq->length--;
	return ((void**)seq->array.array)[i];
}

void* Seq_remhi(T seq){
	assert(seq);
	assert(seq->length > 0);
	int i = (seq->head + seq->length) % seq->array.length;
	seq->length--;
	return ((void**)seq->array.array)[i];
}

