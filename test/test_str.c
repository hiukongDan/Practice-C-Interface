#include "asserttest.h"
#include "str.h"

#include <stdlib.h>
#include <stdio.h>

int main(){
	TESTCASE(0) // Str_sub, Str_dup, Str_cat, Str_catv, Str_reverse, Str_map
		char *str = Str_sub("hello world", 7, 0);
		assert_equal_string(str, "world");
		str = Str_dup("hello world", 7, 0, 3);
		assert_equal_string(str, "worldworldworld");
		str = Str_cat("hello world", 7, 0, "shitshow", 1, 5);
		assert_equal_string(str, "worldshit");
		str = Str_catv("hello world", 7, 0, "shitshow", 1, 5, "mikasaakamann", 7, 0, NULL);
		assert_equal_string(str, "worldshitakamann");
		str = Str_reverse("hello world", 1, 0);
		assert_equal_string(str, "dlrow olleh");
		str = Str_map("hello world", 1, 0, "hld", "HLD");
		assert_equal_string(str, "HeLLo worLD");
		str = Str_map("hello world", 1, 0, "", "");
		assert_equal_string(str, "hello world");
	ENDTEST;
	
	TESTCASE(1)	// Str_pos, Str_len, Str_cmp
		int pos = Str_pos("hello world", -1);
		assert_equal(pos, 11);
		int len = Str_len("hello world", 1, 0);
		assert_equal(len, 11);
		assert_equal(Str_cmp("hello world", 1, 0, "hello world", 1, 0), 0);
		assert_true(Str_cmp("hello world", 1, 0, "hello world ", 1, 0) < 0);
		assert_true(Str_cmp("hello world ", 1, 0, "hello world", 1, 0) > 0);
	ENDTEST;
	
	TESTCASE(2)	// Str_chr, Str_rchr, Str_upto, Str_rupto, Str_find, Str_rfind
		int pos = Str_chr("hello world", 1, 0, (int)'r');
		assert_equal(pos, 9);
		pos = Str_rchr("hello world", 1, 0, (int)'l');
		assert_equal(pos, 10);
		pos = Str_upto("hello world", 1, 0, "dmn");
		assert_equal(pos, 11);
		pos = Str_upto("hello world", 1, 0, "hmnbv");
		assert_equal(pos, 1);
		pos = Str_find("hello world", 1, 0, "world");
		assert_equal(pos, 7);
		pos = Str_find("hello world", 4, 0, "");
		assert_equal(pos, 4);
		pos = Str_find("hello world", 1, 0, "hello world plus");
		assert_equal(pos, 0);
		pos = Str_rfind("hello world", 1, 0, "ell");
		assert_equal(pos, 2);
		pos = Str_rfind("hello world", 4, 0, "");
		assert_equal(pos, 12);
		pos = Str_rfind("hello world", 1, 0, "hello world plus");
		assert_equal(pos, 0);
	ENDTEST;
	
	TESTCASE(3)	// Str_any, Str_many, Str_rmany, Str_match, Str_rmatch
		int pos = Str_any("hello world", 1, "lhdf");
		assert_equal(pos, 2);
		pos = Str_any("hello world", 1, "uij");
		assert_equal(pos, 0);
		pos = Str_any("hello world", 1, "");
		assert_equal(pos, 0);
		
		pos = Str_many("hello world", 1, 0, "helo");
		assert_equal(pos, 6);
		pos = Str_many("hello world", 1, 0, "");
		assert_equal(pos, 0);
		pos = Str_many("hello world", 1, 0, "rdflk");
		assert_equal(pos, 0);
		
		pos = Str_rmany("hello world", 1, 0, "hedlo");
		assert_equal(pos, 10);
		pos = Str_rmany("hello world", 1, 0, "");
		assert_equal(pos, 0);
		pos = Str_rmany("hello world", 1, 0, "afmk");
		assert_equal(pos, 0);
		
		pos = Str_match("hello world", 1, 0, "hello");
		assert_equal(pos, 6);
		pos = Str_match("hello world", 1, 0, "");
		assert_equal(pos, 1);
		pos = Str_match("hello world", 1, 0, "helo");
		assert_equal(pos, 0);
		pos = Str_match("hello world", 1, 0, "h");
		assert_equal(pos, 2);
		pos = Str_match("hello world", 1, 0, "l");
		assert_equal(pos, 0);

		
		pos = Str_rmatch("hello world", 1, 0, "world");
		assert_equal(pos, 7);
		pos = Str_rmatch("hello world", 1, 0, "");
		assert_equal(pos, 12);
		pos = Str_rmatch("hello world", 1, 0, "word");
		assert_equal(pos, 0);
		pos = Str_rmatch("hello world", 1, 0, "d");
		assert_equal(pos, 11);
		pos = Str_rmatch("hello world", 1, 0, "l");
		assert_equal(pos, 0);
	ENDTEST;
	
	TESTCASE(4)	// Str_fmt
		
	ENDTEST;
	
	return EXIT_SUCCESS;
}