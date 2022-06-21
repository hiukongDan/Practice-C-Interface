#include "arrayrep.h"
#include "assert.h"

#include <stdlib.h>

#define Array_T T

void ArrayRep_init(T array, int length, int size, void* ary){
	assert(array);
	assert(length >= 0);
	assert(size > 0);
	
	array->length = length;
	array->size = size;
	if(ary)
		array->array = ary;
	else
		array->array = NULL;
}