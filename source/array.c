#include "array.h"
#include "arrayrep.h"
#include "mem.h"
#include "assert.h"
#include "except.h"

#include <string.h>
#include <stdlib.h>

#define Array_T T

T Array_new(int length, int size){
	T array;
	
	NEW(array);
	if(length > 0){
		ArrayRep_init(array, length, size, CALLOC(length, size));
	}
	else{
		ArrayRep_init(array, length, size, NULL);
	}
	return array;
}

void Array_free(T *array){
	assert(array && *array);
	
	if((*array)->array)
		FREE((*array)->array);
	FREE(*array);
	*array = NULL;
}

int Array_size(T array){
	assert(array);
	return array->size;
}

int Array_length(T array){
	assert(array);
	return array->length;
}

void Array_put(T array, int i, void* elem){
	assert(array);
	assert(elem);
	assert(i >= 0 && i < array->length);
	memcpy(array->array + array->size * i, elem, array->size);
}

void* Array_get(T array, int i){
	assert(array);
	assert(i >= 0 && i < array->length);
	return array->array + array->size * i;
}

void Array_resize(T array, int length){
	assert(array);
	assert(length >= 0);
	if(length == 0){
		FREE(array->array);
	}
	else if(array->length == 0){
		array->array = CALLOC(length, array->size);
	}
	else{
		RESIZE(array->array, array->size * length);
	}
	
	array->length = length;
}

T Array_copy(T array, int length){
	assert(array);
	assert(length >= 0);
	T copy = Array_new(length, array->size);
	if(length >= array->length && array->length > 0){
		memcpy(copy->array, array->array, array->length * array->size);
	}
	else if(length < array->length && length > 0){
		memcpy(copy->array, array->array, length * array->size);
	}
	return copy;
}