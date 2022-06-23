#include "bit.h"
#include "mem.h"
#include "assert.h"

#include <string.h>
#include <stdlib.h>

#define T Bit_T

#define BPW 8*sizeof(unsigned long)
#define nwords(len) ((((len) + BPW - 1) & (~(BPW - 1))) / BPW)		// ceiling of len / BPW
#define nbytes(len) ((((len) + 8 - 1) & (~(8 - 1))) / 8)			// ceiling of len / 8

#define setop(sequal, snull, tnull, op)\
		if(s == t){assert(s); return sequal;}\
		else if(s == NULL){\
			assert(t); return snull;\
		}else if(t == NULL){\
			return tnull;\
		}else{\
			assert(s->length == t->length);\
			T set = Bit_new(s->length);\
			for(int n = nwords(s->length); --n>=0; ){\
				set->words[n] = s->words[n] op t->words[n];\
			}\
			return set;\
		}
		
	
static T copy(T set){
	assert(set);
	T t = Bit_new(set->length);
	if(set->length > 0){
		memcpy(t->bytes, set->bytes, nbytes(set->length));
	}
	return t;
}
	
static const unsigned char msbmasks[] = {
	0xFF, 0xFE, 0xFC, 0xF8,
	0xF0, 0xE0, 0xC0, 0x80
};

static const unsigned char lsbmasks[] = {
	0x01, 0x03, 0x07, 0x0F,
	0x1F, 0x3F, 0x7F, 0xFF
};

T Bit_new(int length){
	assert(length >= 0);
	T set;
	NEW(set);
	set->length = length;
	if(length > 0){
		set->words = (unsigned long*)ALLOC(nwords(length) * BPW);
		memset(set->words, 0, BPW/8*nwords(length));
	}
	else
		set->words = NULL;
	set->bytes = (unsigned char*)set->words;
	return set;
}

int Bit_length(T set){
	assert(set);
	return set->length;
}

int Bit_count(T set){
	assert(set);
	const int count[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
	int res = 0;
	
	for(int n = nbytes(set->length); --n >= 0; ){
		res += count[set->bytes[n]&0xF] + count[set->bytes[n]>>4];
	}
	
	return res;
}

void Bit_free(T* set){
	assert(set && *set);
	FREE((*set)->words);
	FREE(*set);
	*set = NULL;
}

int Bit_get(T set, int n){
	assert(set);
	assert(n >= 0 && n < set->length);
	return set->bytes[n/8] >> n%8 & 0x1;
}

int Bit_put(T set, int n, int bit){
	assert(set);
	assert(n >= 0 && n < set->length);
	assert(bit == 0 || bit == 1);
	unsigned char flag = 0x1 << n%8;
	set->bytes[n/8] = bit == 0 ? set->bytes[n/8] & ~flag : set->bytes[n/8] | flag;
	return bit;
}

void Bit_clear(T set, int lo, int hi){
	assert(set);
	assert(lo >= 0 && hi < set->length);
	assert(lo <= hi);
	if(lo/8 != hi/8){
		set->bytes[lo/8] = set->bytes[lo/8] & ~msbmasks[lo%8];
		for(int i = lo/8+1; i < hi/8; i++){
			set->bytes[i] &= 0x0;
		}
		set->bytes[hi/8] = set->bytes[hi/8] & ~lsbmasks[hi%8];
	}else{
		set->bytes[lo/8] = set->bytes[lo/8] & ~(msbmasks[lo%8] & lsbmasks[hi%8]);
	}
}

void Bit_set(T set, int lo, int hi){
	assert(set);
	assert(lo >= 0 && hi < set->length);
	assert(lo <= hi);
	if(lo/8 != hi/8){
		set->bytes[lo/8] = set->bytes[lo/8] | msbmasks[lo%8];
		for(int i = lo/8+1; i < hi/8; i++){
			set->bytes[i] |= 0xFF;
		}
		set->bytes[hi/8] = set->bytes[hi/8] | lsbmasks[hi%8];
	}else{
		set->bytes[lo/8] = set->bytes[lo/8] | (msbmasks[lo%8] & lsbmasks[hi%8]);
	}
}

void Bit_not(T set, int lo, int hi){
	assert(set);
	assert(lo >= 0 && hi < set->length);
	assert(lo <= hi);
	if(lo/8 != hi/8){
		set->bytes[lo/8] = set->bytes[lo/8] ^ msbmasks[lo%8];
		for(int i = lo/8+1; i < hi/8; i++){
			set->bytes[i] ^= 0xFF;
		}
		set->bytes[hi/8] = set->bytes[hi/8] ^ lsbmasks[hi%8];
	}else{
		set->bytes[lo/8] = set->bytes[lo/8] ^ (msbmasks[lo%8] & lsbmasks[hi%8]);
	}
}

int Bit_lt(T s, T t){
	assert(s && t);
	assert(s->length == t->length);
	int ret = 0;
	for(int n = nwords(s->length); --n >= 0; ){
		if((s->words[n] & ~(t->words[n])) != 0){
			return 0;
		}
		else if(s->words[n] != t->words[n]){
			ret |= 1;
		}
	}
	return ret;
}

int Bit_eq(T s, T t){
	assert(s && t);
	assert(s->length == t->length);
	for(int n = nwords(s->length); --n>=0; ){
		if(s->words[n] != t->words[n]){
			return 0;
		}
	}
	return 1;
}

int Bit_leq(T s, T t){
	assert(s && t);
	assert(s->length == t->length);
	for(int n = nwords(s->length); --n >= 0; ){
		if((s->words[n] & ~(t->words[n])) != 0){
			return 0;
		}
	}
	return 1;
}

void Bit_map(T set, void (*apply)(int n, int bit, void* cl), void* cl){
	for(int i = 0; i < set->length; i++){
		(*apply)(i, set->bytes[i/8] >> i%8 & 0x1, cl);
	}
}

T Bit_union(T s, T t){
	setop(copy(s), copy(t), copy(s), |)
}

T Bit_inter(T s, T t){
	setop(copy(s), Bit_new(t->length), Bit_new(s->length), &)
}

T Bit_minus(T s, T t){
	setop(Bit_new(s->length), Bit_new(s->length), copy(s), & ~)
}

T Bit_diff(T s, T t){
	setop(Bit_new(s->length), copy(t), copy(s), ^)
}