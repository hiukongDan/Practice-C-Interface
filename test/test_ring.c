#include "ring.h"
#include "asserttest.h"

#include <stdio.h>
#include <stddef.h>

int main(){
	TESTCASE(0)	// Ring_new, Ring_ring, Ring_free
		Ring_T ring = Ring_new();
		assert_notequal(ring, (void*)0);
		assert_equal(ring->length, 0);
		Ring_T ring2 = Ring_ring("hello", "world", "merriam", "webster", NULL);
		assert_equal(Ring_length(ring2), 4);
		assert_equal_string(Ring_get(ring2, 2), "merriam");
		Ring_free(&ring);
		Ring_free(&ring2);
		assert_equal(ring, (void*)0);
		assert_equal(ring2, (void*)0);
	ENDTEST;
	
	TESTCASE(1) // Ring_length, Ring_addhi, Ring_get
		Ring_T ring = Ring_new();
		assert_equal(Ring_length(ring), 0);
		Ring_addhi(ring, "hello");
		assert_equal(Ring_length(ring), 1);
		assert_equal_string(Ring_get(ring, 0), "hello");
		Ring_addhi(ring, "world");
		assert_equal(Ring_length(ring), 2);
		assert_equal_string(Ring_get(ring, 1), "world");
	ENDTEST;
	
	TESTCASE(2) // Ring_remove, Ring_put
		Ring_T ring = Ring_new();
		Ring_addhi(ring, "hello");
		Ring_addhi(ring, "world");
		char* word = (char*)Ring_remove(ring, 0);
		assert_equal(Ring_length(ring), 1);
		assert_equal_string(word, "hello");
		assert_equal_string(Ring_get(ring, 0), "world");
		Ring_addhi(ring, "world");
		word = (char*)Ring_put(ring, 1, "merriam");
		assert_equal_string(word, "world");
		assert_equal_string((char*)Ring_get(ring, 1), "merriam");
	ENDTEST;
	
	TESTCASE(3) // Ring_addlo, Ring_add, Ring_rotate
		Ring_T ring = Ring_new();
		Ring_add(ring, 0, "hello");
		Ring_add(ring, 0, "world");
		Ring_add(ring, 0, "merriam");
		assert_equal(Ring_length(ring), 3);
		assert_equal_string(Ring_get(ring, 1), "world");
		assert_equal_string(Ring_get(ring, 0), "hello");
		Ring_addlo(ring, "webster");
		assert_equal(Ring_length(ring), 4);
		assert_equal_string(Ring_get(ring, 0), "webster");
		Ring_rotate(ring, 3);
		assert_equal_string(Ring_get(ring, 0), "merriam");
		Ring_rotate(ring, -2);
		assert_equal_string(Ring_get(ring, 0), "hello");
	ENDTEST;
	
	TESTCASE(4) // Ring_remlo, Ring_remhi
		Ring_T ring = Ring_new();
		Ring_add(ring, 0, "hello");
		Ring_add(ring, 0, "world");
		Ring_add(ring, 0, "merriam");
		Ring_addlo(ring, "webster");
		char* word = (char*)Ring_remlo(ring);
		assert_equal_string(word, "webster");
		assert_equal_string(Ring_get(ring, 2), "merriam");
		word = (char*)Ring_remhi(ring);
		assert_equal_string(word, "merriam");
		assert_equal_string(Ring_get(ring, 1), "world");
		assert_equal_string(Ring_get(ring, 0), "hello");
	ENDTEST;
	
	
	return EXIT_SUCCESS;
}