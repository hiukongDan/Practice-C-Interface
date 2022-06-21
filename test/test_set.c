#include "set.h"
#include "asserttest.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

int cmpstr(const void* a, const void* b){
	return strcmp((const char*)a, (const char*)b);
}

unsigned hashstr(const void* key){
	return (unsigned long)(((const char*)key)[0]) >> 2;
}

void setmap(const void* key, void* cl){
	char* str = (char*)key;
	char* buf = (char*)cl;
	buf += strlen(buf);
	sprintf(buf, "%s ", str);
}

int main(){
	TESTCASE(0) // Set_new
		Set_T set = Set_new(INT_MAX, NULL, NULL);
		Set_T setstr = Set_new(INT_MAX, &cmpstr, &hashstr);
		assert_notequal(set, 0);
		assert_notequal(setstr, 0);
	ENDTEST;
	
	TESTCASE(1) // Set_free
		Set_T set = Set_new(INT_MAX, NULL, NULL);
		Set_T setstr = Set_new(INT_MAX, &cmpstr, &hashstr);
		Set_free(&set);
		Set_free(&setstr);
		assert_equal(set, 0);
		assert_equal(setstr, 0);
	ENDTEST;
	
	TESTCASE(2) // Set_put, Set_length
		Set_T set = Set_new(INT_MAX, NULL, NULL);
		Set_T setstr = Set_new(INT_MAX, &cmpstr, &hashstr);
		assert_equal(Set_length(set), 0);
		assert_equal(Set_length(setstr), 0);
		Set_put(set, "hello");
		Set_put(setstr, "hello");
		Set_put(setstr, "world");
		assert_equal(Set_length(set), 1);
		assert_equal(Set_length(setstr), 2);
	ENDTEST;
	
	TESTCASE(3) // Set_member
		Set_T set = Set_new(INT_MAX, NULL, NULL);
		Set_T setstr = Set_new(INT_MAX, &cmpstr, &hashstr);
		Set_put(set, "hello");
		Set_put(setstr, "hello");
		Set_put(setstr, "world");
		assert_equal(Set_member(set, "hello"), 1);
		assert_equal(Set_member(setstr, "hello"), 1);
		assert_equal(Set_member(setstr, "world"), 1);
		assert_equal(Set_member(set, "world"), 0);
	ENDTEST;
	
	TESTCASE(4) // Set_remove
		Set_T set = Set_new(INT_MAX, NULL, NULL);
		Set_T setstr = Set_new(INT_MAX, &cmpstr, &hashstr);
		Set_put(set, "hello");
		Set_put(setstr, "hello");
		Set_put(setstr, "world");
		Set_remove(set, "hello");
		Set_remove(setstr, "world");
		Set_remove(setstr, "meaningless");
		assert_equal(Set_length(set), 0);
		assert_equal(Set_length(setstr), 1);
		assert_equal(Set_member(setstr, "hello"), 1);
	ENDTEST;
	
	TESTCASE(5) // Set_map
		Set_T set = Set_new(INT_MAX, NULL, NULL);
		Set_T setstr = Set_new(INT_MAX, &cmpstr, &hashstr);
		Set_put(set, "hello");
		Set_put(setstr, "hello");
		Set_put(setstr, "world");
		
		char buf[1024];
		memset(buf, '\0', sizeof(buf));
		
		Set_map(set, setmap, buf);
		assert_equal_string(buf, "hello ");
		
		memset(buf, '\0', sizeof(buf));
		Set_map(setstr, setmap, buf);
		assert_equal_string(buf, "hello world ");
	ENDTEST;
	
	TESTCASE(6) // Set_toArray
	Set_T set = Set_new(INT_MAX, NULL, NULL);
		Set_T setstr = Set_new(INT_MAX, &cmpstr, &hashstr);
		Set_put(set, "hello");
		Set_put(setstr, "hello");
		Set_put(setstr, "world");
		
		char** arr = (char**)Set_toArray(set, NULL);
		char** arr2 = (char**)Set_toArray(setstr, NULL);
		
		assert_equal_string(arr[0], "hello");
		assert_equal(arr[1], 0);
		assert_equal_string(arr2[0], "hello");
		assert_equal_string(arr2[1], "world");
		assert_equal(arr2[2], 0);
	ENDTEST;
	
	TESTCASE(7) // Set_union
		Set_T s = Set_new(INT_MAX, NULL, NULL);
		Set_T t = Set_new(INT_MAX, NULL, NULL);
		Set_put(s, "assert");
		Set_put(s, "temp");
		Set_put(s, "member");
		Set_put(t, "list");
		Set_put(t, "map");
		
		Set_T u = Set_union(s, t);
		assert_equal(Set_member(u, "assert"), 1);
		assert_equal(Set_member(u, "temp"), 1);
		assert_equal(Set_member(u, "map"), 1);
		assert_equal(Set_length(u), 5);
	ENDTEST;
	
	TESTCASE(8) // Set_inter
		Set_T s = Set_new(INT_MAX, NULL, NULL);
		Set_T t = Set_new(INT_MAX, NULL, NULL);
		Set_put(s, "assert");
		Set_put(s, "temp");
		Set_put(s, "member");
		Set_put(t, "list");
		Set_put(t, "member");
		Set_put(t, "map");
		
		Set_T u = Set_inter(s, t);
		assert_equal(Set_member(u, "assert"), 0);
		assert_equal(Set_member(u, "temp"), 0);
		assert_equal(Set_member(u, "map"), 0);
		assert_equal(Set_member(u, "member"), 1);
		assert_equal(Set_length(u), 1);
	ENDTEST;
	
	TESTCASE(7) // Set_diff
		Set_T s = Set_new(INT_MAX, NULL, NULL);
		Set_T t = Set_new(INT_MAX, NULL, NULL);
		Set_put(s, "assert");
		Set_put(s, "temp");
		Set_put(s, "member");
		Set_put(t, "list");
		Set_put(t, "member");
		Set_put(t, "map");
		
		Set_T u = Set_minus(s, t);
		assert_equal(Set_member(u, "assert"), 1);
		assert_equal(Set_member(u, "temp"), 1);
		assert_equal(Set_member(u, "map"), 0);
		assert_equal(Set_member(u, "member"), 0);
		assert_equal(Set_length(u), 2);
	ENDTEST;
	
	TESTCASE(7) // Set_diff
		Set_T s = Set_new(INT_MAX, NULL, NULL);
		Set_T t = Set_new(INT_MAX, NULL, NULL);
		Set_put(s, "assert");
		Set_put(s, "temp");
		Set_put(s, "member");
		Set_put(t, "list");
		Set_put(t, "member");
		Set_put(t, "map");
		
		Set_T u = Set_diff(s, t);
		assert_equal(Set_member(u, "assert"), 1);
		assert_equal(Set_member(u, "temp"), 1);
		assert_equal(Set_member(u, "map"), 1);
		assert_equal(Set_member(u, "member"), 0);
		assert_equal(Set_length(u), 4);
	ENDTEST;
	
	return EXIT_SUCCESS;
}