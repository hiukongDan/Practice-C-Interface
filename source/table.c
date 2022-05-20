#include "table.h"
#include "mem.h"
#include "except.h"
#include "assert.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#define T Table_T

struct T{
	int size;
	int (*cmp)(const void* a, const void* b);
	unsigned (*hash)(const void* value);
	int length;
	unsigned timestamp;
	struct binding{
		struct binding* link;
		const void* key;
		void* value;
	} **buckets;
};


static int cmpatom(const void* a, const void* b){
	return a != b;
}

static unsigned hashatom(const void* key){
	return (unsigned long)key >> 2;
}

T Table_new(int hint, int cmp(const void* a, const void* b), unsigned hash(const void* x)){
	T table;
	static int primes[] = {509, 509, 1021, 2039, 4093, 8191, 16381, 32749, 65521, INT_MAX};
	
	int i;
	for (i = 1; hint > primes[i]; i++)
		;
	table = ALLOC(sizeof(*table) + (primes[i-1]) * sizeof(table->buckets[0]));
	table->size = primes[i-1];
	table->cmp = cmp ? cmp : cmpatom;
	table->hash = hash ? hash : hashatom;
	table->buckets = (struct binding**)(table + 1);
	for (i = 0; i < table->size; i++){
		table->buckets[i] = NULL;
	}
	table->length = 0;
	table->timestamp = 0;
	return table;
}

void Table_free(T *table){
	struct binding *p, *tmp;
	assert(table && *table);
	if((*table)->length > 0){		
		for (int i = 0; i < (*table)->size; i++){
			p = (*table)->buckets[i];
			while(p){
				tmp = p;
				p = p->link;
				FREE(tmp);
			}
		}
	}
	FREE(*table);
}


void* Table_put(T table, const void* key, void* value){
	assert(table && key);
	
	unsigned hashed = (*table->hash)(key) % table->size;
	struct binding *p = table->buckets[hashed];
	void* pre;
	for(; p; p = p->link){
		if((*table->cmp)(key, p->key) == 0){
			break;
		}
	}
	if(p == NULL){		
		NEW(p);
		p->link = table->buckets[hashed];
		table->buckets[hashed] = p;
		p->key = key;
		table->length++;
		pre = NULL;
	}
	else{
		pre = p->value;
	}
	p->value = value;
	table->timestamp++;
	return pre;
}

void* Table_get(T table, const void* key){
	assert(table && key);
	
	unsigned hashed = (*table->hash)(key) % table->size;
	struct binding *p = table->buckets[hashed];
	for(; p; p = p->link){
		if((*table->cmp)(p->key, key) == 0){
			return p->value;
		}
	}
	return NULL;
}

void* Table_remove(T table, const void* key){
	assert(table && key);
	
	unsigned hashed = (*table->hash)(key) % table->size;
	struct binding **p = &table->buckets[hashed];
	table->timestamp++;
	for(; *p; p = &(*p)->link){
		if((*table->cmp)((*p)->key, key) == 0){
			void* value = (*p)->value;
			struct binding *tmp = *p;
			*p = (*p)->link;
			FREE(tmp);
			table->length--;
			return value;
		}
	}
	return NULL;
}


int Table_length(T table){
	assert(table);
	return table->length;
}

void Table_map(T table, void apply(const void* key, void** value, void* cl), void* cl){
	assert(table && apply);
	struct binding *p;
	unsigned timestamp = table->timestamp;
	for(int i = 0; i < table->size; i++){
		for(p = table->buckets[i]; p; p = p->link){
			apply(p->key, &(p->value), cl);
			assert(timestamp == table->timestamp);
		}
	}
}


void** Table_toArray(T table, void* end){
	// [Key, Value, Key, Value, ... , end]
	void** array = ALLOC(sizeof(void*) * (table->length * 2 + 1));
	int i = 0;
	struct binding *p;
	for (int j = 0; j < table->size; j++){
		for(p = table->buckets[j]; p; p = p->link){
			array[i++] =(void*) p->key;
			array[i++] = p->value;
		}
	}
	array[i] = end;
	return array;
}