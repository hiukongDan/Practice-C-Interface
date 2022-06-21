#include "set.h"
#include "except.h"
#include "mem.h"
#include "assert.h"
#include <stdlib.h>
#include <limits.h>
#include <stddef.h>

#include <stdio.h>


#define T Set_T

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

static unsigned hashatom(const void* member){
	return (unsigned long)member >> 2;
}

T Set_new(int hint, int cmp(const void* a, const void* b), unsigned hash(const void* member)){
	assert(hint > 0);
	
	T set;
	static int primes[] = {509, 509, 1021, 2039, 4093, 8191, 16381, 32749, 65521, INT_MAX};
	
	int i;
	for (i = 1; hint > primes[i]; i++){
		;
	}
	
	set = ALLOC(sizeof(*set) + sizeof(set->buckets[0]) * primes[i-1]);
	
	if(set == NULL){
		RAISE(Mem_Failed);
	}
	
	set->size = primes[i-1];
	set->buckets = (struct member**)(set+1);
	for(i = 0; i < set->size; i++){
		set->buckets[i] = NULL;
	}
	
	set->cmp = cmp ? cmp : cmpatom;
	set->hash = hash ? hash : hashatom;
	
	set->timestamp = 0;
	set->length = 0;
	return set;
}

void Set_free(T *set){
	assert(set && *set);
	struct member *p, *next;
	for(int i = 0; i < (*set)->size; i++){
		for(p = (*set)->buckets[i]; p; p = next){
			next = p->link;
			FREE(p);
		}
	}
	FREE((*set)->buckets);
	FREE(*set);
	*set = NULL;
}

int Set_member(T set, const void* member){
	assert(set && member);
	struct member *p;
	unsigned i = (*set->hash)(member) % set->size;
	for(p = set->buckets[i]; p; p = p->link){
		if((*set->cmp)((p->member), member) == 0){
			return 1;
		}
	}
	return 0;
}

int Set_length(T set){
	assert(set);
	return set->length;
}

void Set_put(T set, const void* member){
	assert(set && member);
	unsigned i = (*set->hash)(member) % set->size;
	struct member* p;

	for(p = set->buckets[i]; p; p = p->link){
		if((*set->cmp)(p->member, member) == 0){
			break;
		}
	}
	if (p == NULL){
		NEW(p);
		p->member = member;
		p->link = set->buckets[i];
		set->buckets[i] = p;
		set->length++;
	}
	else{
		p->member = member;
	}
	set->timestamp++;
}

void* Set_remove(T set, const void* member){
	assert(set && member);
	unsigned i = (*set->hash)(member) % set->size;
	struct member** pp;
	void* ret = NULL;
	for(pp = &set->buckets[i]; *pp; pp = &(*pp)->link){
		if((*set->cmp)((*pp)->member, member) == 0){
			struct member* p = *pp;
			*pp = (*pp)->link;
			ret = (void*)(p->member);
			FREE(p);
			set->length--;
			break;
		}
	}
	set->timestamp++;
	return ret;
}

void Set_map(T set, void (*apply)(const void* member, void* cl), void* cl){
	assert(set && apply);
	if(set->length > 0){
		struct member *p;
		unsigned timestamp = set->timestamp;
		int i;
		for(i = 0; i < set->size; i++){			
			for(p = set->buckets[i]; p; p = p->link){
				(*apply)(p->member, cl);
				if(timestamp != set->timestamp)
					RAISE(Assert_Failed);
			}
		}
	}
}

void** Set_toArray(T set, void* end){
	assert(set);
	void** array = ALLOC(sizeof(void*) * (set->length + 1));
	int j, i = 0;
	struct member* p;
	if(set->length > 0){
		for(j = 0; j < set->size; j++){
			for(p = set->buckets[j]; p; p = p->link){
				array[i++] = (void*)(p->member);
			}
		}
	}
	array[i] = end;
	return array;
}

T Set_union(T s, T t){
	int size = s ? s->size : 0;
	size = t ? (t->size > size ? t->size : size) : (size > 0 ? size : 256);
	T set = Set_new(size, s ? s->cmp : NULL, s ? s->hash : NULL);
	struct member* p;
	int i;
	if(s && s->length > 0){
		for(i = 0; i < s->size; i++){
			for(p = s->buckets[i]; p; p = p->link){
				Set_put(set, p->member);
			}
		}
	}
	if(t && t->length > 0){
		for(i = 0; i < t->size; i++){
			for(p = t->buckets[i]; p; p = p->link){
				Set_put(set, p->member);
			}
		}
	}
	return set;
}

T Set_inter(T s, T t){
	int size = s ? s->size : 0;
	size = t ? (t->size > size ? t->size : size) : (size > 0 ? size : 256);
	T set = Set_new(size, s ? s->cmp : NULL, s ? s->hash : NULL);
	struct member* p;
	int i;
	if(s && s->length > 0){
		for(i = 0; i < s->size; i++){
			for(p = s->buckets[i]; p; p = p->link){
				if(Set_member(t, p->member))
					Set_put(set, p->member);
			}
		}
	}
	return set;
}

T Set_minus(T s, T t){
	int size = s ? s->size : 0;
	size = t ? (t->size > size ? t->size : size) : (size > 0 ? size : 256);
	T set = Set_new(size, s ? s->cmp : NULL, s ? s->hash : NULL);
	struct member* p;
	int i;
	if(s && s->length > 0){
		for(i = 0; i < s->size; i++){
			for(p = s->buckets[i]; p; p = p->link){
				if(!Set_member(t, (p->member)))
					Set_put(set, p->member);
			}
		}
	}
	return set;
}

T Set_diff(T s, T t){
	int size = s ? s->size : 0;
	size = t ? (t->size > size ? t->size : size) : (size > 0 ? size : 256);
	T set = Set_new(size, s ? s->cmp : NULL, s ? s->hash : NULL);
	struct member* p;
	int i;
	if(s && s->length > 0){
		for(i = 0; i < s->size; i++){
			for(p = s->buckets[i]; p; p = p->link){
				if(!Set_member(t, p->member))
					Set_put(set, p->member);
			}
		}
	}
	if(t && t->length > 0){
		for(i = 0; i < t->size; i++){
			for(p = t->buckets[i]; p; p = p->link){
				if(!Set_member(s, (p->member)))
					Set_put(set, (p->member));
			}
		}
	}
	return set;
}