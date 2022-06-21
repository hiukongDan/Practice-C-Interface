#include "array.h"
#include "asserttest.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <limits.h>

int main(){
	TESTCASE(0) // Array_new
		Array_T array = Array_new(100, 4);
		assert_notequal(array, 0);
	ENDTEST;
	
	TESTCASE(1) // Array_free
		Array_T array = Array_new(100, 4);
		Array_free(&array);
		assert_equal(array, 0);
	ENDTEST;
	
	TESTCASE(2) // Array_size, Array_length
		Array_T array = Array_new(100, 4);
		assert_equal(Array_length(array), 100);
		assert_equal(Array_size(array), 4);
	ENDTEST;
	
	TESTCASE(3) // Array_put, Array_get
		Array_T array = Array_new(100, sizeof(int));
		int i = 233;
		Array_put(array, 0, &i);
		Array_put(array, 1, &i);
		int elem = *(int*)Array_get(array, 1);
		assert_equal(elem, 233);
	ENDTEST;
	
	TESTCASE(4) // Array_copy, Array_resize
		Array_T array = Array_new(100, sizeof(int));
		for(int i = 0; i < 100; i++){
			Array_put(array, i, &i);
		}
		Array_resize(array, 50);
		assert_equal(Array_length(array), 50);
		Array_T array2 = Array_copy(array, 50);
		assert_equal(*(int*)Array_get(array2, Array_length(array2)-1), 49);
	ENDTEST;
	
	return EXIT_SUCCESS;
}