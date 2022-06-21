#include "seq.h"
#include "asserttest.h"
#include <stdlib.h>


int main(){
	TESTCASE(0) // Seq_new, Seq_free
		Seq_T s = Seq_new(0);
		Seq_T s2 = Seq_new(99);
		assert_notequal(s, 0);
		assert_notequal(s2, 0);
		Seq_free(&s);
		Seq_free(&s2);
		assert_equal(s, 0);
		assert_equal(s2, 0);
	ENDTEST;
	
	TESTCASE(1) // Seq_length
		Seq_T s = Seq_new(0);
		assert_equal(Seq_length(s), 0);
		Seq_free(&s);
	ENDTEST;
	
	TESTCASE(2) // Seq_addlo, Seq_addhi
		Seq_T s = Seq_new(0);
		Seq_addlo(s, "hello");
		Seq_addhi(s, "world");
		assert_equal(Seq_length(s), 2);
	ENDTEST;
	
	TESTCASE(3) // Seq_remlo, Seq_remhi
		Seq_T s = Seq_new(0);
		Seq_addlo(s, "hello");
		Seq_addhi(s, "world");
		assert_equal(Seq_length(s), 2);
		Seq_remhi(s);
		Seq_remlo(s);
		assert_equal(Seq_length(s), 0);
	ENDTEST;
	
	TESTCASE(4) // Seq_seq
		Seq_T s = Seq_seq("hello", "world", "bye", "world", NULL);
		assert_equal(Seq_length(s), 4);
	ENDTEST;
	
	TESTCASE(5) // Seq_get, Seq_put
		Seq_T s = Seq_seq("hello", "world", "bye", "world", NULL);
		Seq_put(s, 3, "universe");
		assert_equal(Seq_length(s), 4);
		assert_equal_string((char*)Seq_get(s, 3), "universe");
		assert_equal_string((char*)Seq_get(s, 0), "hello");
	ENDTEST;
	
	return EXIT_SUCCESS;
}