#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "asserttest.h"

void combine(void** p, void* cl){
	char* buf = (char*)cl;
	char** item = (char**)p;
	while(*buf != 0) buf++;
	sprintf(buf, "%s ", *item);
}

int main(){
	// Test case for List_push
	TESTCASE(0)
		List_T list = List_push(NULL, "Newton");
		assert_notequal(list, 0);
	ENDTEST;
	
	// Test case for List_pop
	TESTCASE(1)
		List_T list = List_push(NULL, "Einstein");
		list = List_push(list, "Newton");
		char* name;
		list = List_pop(list, (void**)&name);
		assert_notequal(list, 0);
		assert_equal(name, "Newton");
		list = List_pop(list, (void**)&name);
		assert_equal(name, "Einstein");
		list = List_pop(list, (void**)&name);
		assert_equal(list, 0);
	ENDTEST;
	
	// Testcase for List_list
	TESTCASE(2)
		List_T list = List_list("Newton", "Einstein", "Galilleo", "Darwin", NULL);
		assert_notequal(list, 0);
	ENDTEST;
	
	// Test case for List_length
	TESTCASE(3)
		List_T list = List_list("Newton", "Einstein", "Galilleo", "Darwin", NULL);
		assert_notequal(list, 0);
		assert_equal(List_length(list), 4);
	ENDTEST;
	
	// Test case for List_append
	TESTCASE(4)
		List_T list = List_list("Newton", "Einstein", "Galilleo", "Darwin", NULL);
		List_T list2 = List_list("Marry", "Joseph", "Jesus", "John", "Saul", NULL);
		list = List_append(list, list2);
		assert_equal(List_length(list), 9);
		char* name;
		list = List_pop(list, (void**)&name);
		assert_equal(name, "Newton");
		assert_equal(List_length(list), 8);
	ENDTEST;
	
	// Test case for List_free
	TESTCASE(5)
		List_T list = List_list("Newton", "Einstein", "Galilleo", "Darwin", NULL);
		List_free(&list);
		assert_equal(list, 0);
	ENDTEST;
	
	// Test case for List_reverse
	TESTCASE(6)
		List_T list = List_list("Newton", "Einstein", "Galilleo", "Darwin", NULL);
		list = List_reverse(list);
		char* name;
		list = List_pop(list, (void**)&name);
		assert_equal(name, "Darwin");
	ENDTEST;
	
	// Test case for List_toArray
	TESTCASE(7)
		List_T list = List_list("Newton", "Einstein", "Galilleo", "Darwin", NULL);
		char** names = (char**)List_toArray(list, NULL);
		assert_equal(*names, "Newton");
		names++;
		assert_equal(*names, "Einstein");
		names++;
		assert_equal(*names, "Galilleo");
		names++;
		assert_equal(*names, "Darwin");
		names++;
		assert_equal(*names, NULL);
	ENDTEST;
	
	// Test case for List_copy
	TESTCASE(8)
		List_T list = List_list("Newton", "Einstein", "Galilleo", "Darwin", NULL);
		List_T list2 = List_copy(list);
		assert_equal(List_length(list2), 4);
		assert_notequal(list, list2);
		char* name;
		list2 = List_pop(list2, (void**)&name);
		assert_equal(name, "Newton");
	ENDTEST;
	
	// Test case for List_map
	TESTCASE(9)
		List_T list = List_list("Newton", "Einstein", "Galilleo", "Darwin", NULL);
		char buf[1024];
		memset((void*)buf, '\0', sizeof(*buf));
		List_map(list, combine, (void*)buf);
		assert_equal_string(buf, "Newton Einstein Galilleo Darwin ");
	ENDTEST;
	
	
	return EXIT_SUCCESS;
}