#include "bit.h"
#include "asserttest.h"

#include <stdio.h>
#include <stdlib.h>

void checksum_even(int n, int bit, void* cl){
	// even bit checksum
	*((int*)cl) += n%2==0?bit:0;
}

void checksum_odd(int n, int bit, void* cl){
	// odd bit checksum
	*((int*)cl) += n%2!=0?bit:0;
}

int main(){
	TESTCASE(0)	// Bit_new, Bit free
		Bit_T Bit = Bit_new(512);
		assert_notequal(Bit, (void*)0);
		assert_equal(Bit->length, 512);
		assert_notequal(Bit->bytes, (void*)0);
		assert_notequal(Bit->words, (void*)0);
		assert_equal(Bit->bytes, (void*)(Bit->words));
		Bit_free(&Bit);
		assert_equal(Bit, (void*)0);
	ENDTEST;
	
	TESTCASE(1) // Bit_put, Bit_get, Bit_count, Bit_length
		Bit_T Bit = Bit_new(100);
		assert_equal(Bit_get(Bit, 10), 0);
		Bit_put(Bit, 10, 1);
		assert_equal(Bit_get(Bit, 10), 1);
		assert_equal(Bit_length(Bit), 100);
		assert_equal(Bit_count(Bit), 1);
		for (int i = 0; i < 50; i++){
			Bit_put(Bit, i, 1);
		}
		assert_equal(Bit_length(Bit), 100);
		assert_equal(Bit_count(Bit), 50);
		Bit_free(&Bit);
	ENDTEST;
	
	TESTCASE(2) // Bit_clear, Bit_set, Bit_not
		Bit_T bit = Bit_new(100);
		Bit_set(bit, 10, 19);
		assert_equal(Bit_count(bit), 10);
		Bit_clear(bit, 4, 32);
		assert_equal(Bit_count(bit), 0);
		Bit_set(bit, 0, 6);
		assert_equal(Bit_count(bit), 7);
		Bit_clear(bit, 0, 7);
		assert_equal(Bit_count(bit), 0);
		Bit_not(bit, 0, 7);
		assert_equal(Bit_count(bit), 8);
		Bit_not(bit, 5, 32);
		assert_equal(Bit_count(bit), 30);
		Bit_free(&bit);
	ENDTEST;
	
	TESTCASE(3) 	// Bit_lt, Bit_eq, Bit_leq
		Bit_T b1 = Bit_new(100);
		Bit_T b2 = Bit_new(100);
		Bit_T b3 = Bit_new(100);
		Bit_T b4 = Bit_new(100);
		Bit_set(b1, 0, 10);
		Bit_set(b1, 40, 50);
		Bit_set(b2, 0, 10);
		Bit_set(b2, 40, 50);
		Bit_set(b3, 0, 10);
		Bit_set(b3, 40, 50);
		Bit_set(b3, 50, 60);
		Bit_set(b4, 0, 10);
		Bit_set(b4, 40, 45);
		Bit_set(b4, 50, 60);
		
		assert_equal(Bit_eq(b1, b2), 1);
		assert_equal(Bit_lt(b1, b3), 1);
		assert_equal(Bit_leq(b1, b3), 1);
		assert_equal(Bit_eq(b1, b4), 0);
		assert_equal(Bit_lt(b1, b4), 0);
		assert_equal(Bit_leq(b1, b4), 0);
		
		Bit_free(&b1);
		Bit_free(&b2);
		Bit_free(&b3);
		Bit_free(&b4);
	ENDTEST;

	TESTCASE(4)	// Bit_map
		Bit_T bit = Bit_new(100);
		int sum = 0;
		Bit_set(bit, 10, 20);
		Bit_map(bit, checksum_even, &sum);
		assert_equal(sum, 6);
		sum = 0;
		Bit_map(bit, checksum_odd,&sum);
		assert_equal(sum, 5);
		Bit_free(&bit);
	ENDTEST;
	
	TESTCASE(5)	// Bit_union, Bit_inter, Bit_diff, Bit_minus
		Bit_T b1 = Bit_new(100);
		Bit_T b2 = Bit_new(100);
		
		Bit_set(b1, 10, 20);
		Bit_set(b2, 15, 25);
		
		Bit_T b3 = Bit_union(b1, b2);
		Bit_T b4 = Bit_inter(b1, b2);
		Bit_T b5 = Bit_diff(b1, b2);
		Bit_T b6 = Bit_minus(b1, b2);
		
		Bit_T b7 = Bit_new(100);
		Bit_T b8 = Bit_new(100);
		Bit_T b9 = Bit_new(100);
		Bit_T b10 = Bit_new(100);
		
		Bit_set(b7, 10, 25);
		Bit_set(b8, 15, 20);
		Bit_set(b9, 10, 14);
		Bit_set(b9, 21, 25);
		Bit_set(b10, 10, 14);
		
		assert_equal(Bit_eq(b3, b7), 1);
		assert_equal(Bit_eq(b4, b8), 1);
		assert_equal(Bit_eq(b5, b9), 1);
		assert_equal(Bit_eq(b6, b10), 1);
		
		Bit_free(&b1);
		Bit_free(&b2);
		Bit_free(&b3);
		Bit_free(&b4);
		Bit_free(&b5);
		Bit_free(&b6);
	ENDTEST;

	
	return EXIT_SUCCESS;
}