#include "text.h"
#include "asserttest.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
	TESTCASE(0)		// Text_box, Text_pos, Text_put, Text_box, Text_get
		const char *s = "hello world";
		Text_T t = Text_box(s, strlen(s));
		assert_equal(t.len, strlen(s));
		assert_equal_string(t.str, s);
		assert_equal(Text_pos(t, 0), 12);
		assert_equal(Text_pos(t, -1), 11);
		t = Text_put(s);
		assert_equal(t.len, 11);
		assert_equal(strncmp(t.str, s, strlen(s)), 0);
		t = Text_box(s, strlen(s));
		assert_equal(strncmp(t.str, s, strlen(s)), 0);
		assert_equal(t.len, 11);
		assert_equal_string(Text_get(NULL, 20, t), "hello world");
	ENDTEST;
	
	TESTCASE(1)	// Text_cat, Text_dup, Text_reverse, Text_cmp
		const char *str = "hello world and Historia";
		Text_T s1 = Text_put("hello world");
		Text_T s2 = Text_put(" and Historia");
		Text_T s = Text_cat(s1, s2);
		assert_equal_string(Text_get(NULL, 50, s), str);
		Text_T s3 = Text_dup(Text_put("hello"), 5);
		assert_equal_string(Text_get(NULL, 30, s3), "hellohellohellohellohello");
		assert_equal_string(Text_get(NULL, 30, Text_reverse(s1)), "dlrow olleh");
		Text_T s4 = Text_map(s1, &Text_lcase, &Text_ucase);
		assert_equal_string(Text_get(NULL, 30, s4), "HELLO WORLD");
		Text_T s5 = Text_map(s2, NULL, NULL);
		assert_equal_string(Text_get(NULL, 30, s5), " AND HISTORIA");
		assert_true(Text_cmp(Text_put("hello world"), Text_put("hello")) > 0);
		assert_true(Text_cmp(Text_put(""), Text_put("")) == 0);
		assert_true(Text_cmp(Text_put("hello"), Text_put("world")) < 0);
	ENDTEST;
	
	TESTCASE(2)		// Text_chr, Text_rchr, Text_upto, Text_rupto
		Text_T text = Text_put("hello world und Historia");
		int pos = Text_chr(text, 1, 0, 'w');
		assert_equal(pos, 7);
		pos = Text_chr(text, 1, 0, 'a');
		assert_equal(Text_pos(text, -1), pos);
		
		pos = Text_rchr(text, 1, 0, 'a');
		assert_equal(pos, 24);
		pos = Text_rchr(text, 1, 0, 'l');
		assert_equal(pos, 10);
		
		pos = Text_upto(text, 1, 0, Text_put("mndwj"));
		assert_equal(pos, 7);
		pos = Text_rupto(text, 1, 0, Text_put("Hfde"));
		assert_equal(pos, 17);
	ENDTEST;
	
	TESTCASE(3)		// Text_any, Text_many, Text_rmany
		Text_T text = Text_put("hello world und Historia");
		int pos = Text_any(text, 1, Text_put("hdkfl"));
		assert_equal(pos, 2);
		pos = Text_any(text, 1, Text_put("dkfl"));
		assert_equal(pos, 0);
		
		pos = Text_many(text, 1, 0, Text_put("hello "));
		assert_equal(pos, 7);
		pos = Text_many(text, 1, 0, Text_put("zxcvbnm"));
		assert_equal(pos, 0);
		
		pos = Text_rmany(text, 1, 0, Text_put(" Historia"));
		assert_equal(pos, 16);
		pos = Text_rmany(text, 1, 0, Text_put("zxcvbnm"));
		assert_equal(pos, 0);
	ENDTEST;
	
	TESTCASE(4)
		Text_T text = Text_put("hello world und Historia");
		int pos = Text_find(text, 1, 0, Text_put("world"));
		assert_equal(pos, 7);
		pos = Text_find(text, 1, 0, Text_put("hes"));
		assert_equal(pos, 0);
		pos = Text_find(text, 1, 0, Text_put("o"));
		assert_equal(pos, 5);
		pos = Text_find(text, 1, 0, Text_put(""));
		assert_equal(pos, 1);
		
		pos = Text_rfind(text, 1, 0, Text_put("Historia"));
		assert_equal(pos, 17);
		pos = Text_rfind(text, 1, 0, Text_put("H"));
		assert_equal(pos, 17);
		pos = Text_rfind(text, 1, 0, Text_put(""));
		assert_equal(pos, 25);
	ENDTEST;
	
	TESTCASE(5)
		Text_T text = Text_put("hello world and Historia");
		int pos = Text_match(text, 1, 0, Text_put("hello"));
		assert_equal(pos, 6);
		pos = Text_match(text, 1, 0, Text_put("h"));
		assert_equal(pos, 2);
		pos = Text_match(text, 1, 0, Text_put(""));
		assert_equal(pos, 1);
		
		pos = Text_rmatch(text, 1, 0, Text_put("Historia"));
		assert_equal(pos, 17);
		pos = Text_rmatch(text, 1, 0, Text_put("a"));
		assert_equal(pos, 24);
		pos = Text_rmatch(text, 1, 0, Text_put("wolrd"));
		assert_equal(pos, 0);
		pos = Text_rmatch(text, 1, 0, Text_put(""));
		assert_equal(pos, 25);
	ENDTEST;
	
	return EXIT_SUCCESS;
}