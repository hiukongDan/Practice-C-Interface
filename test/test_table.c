#include "table.h"
#include <stdlib.h>
#include "asserttest.h"
#include <limits.h>
#include <string.h>

void repeatValue(const void* key, void** value, void* cl){
	// change value to valuevalue
	// and contatenate original key value to buf
	char* strKey = (char*)key;
	char* strValue = *(char**)value;
	char* buf = (char*)cl;
	buf += strlen(buf);
	sprintf(buf, "%s %s ", strKey, strValue);
	char* newValue = malloc(sizeof(char) * (strlen(strValue) * 2 + 1));
	strcpy(newValue, strValue);
	strcpy(newValue+strlen(strValue), strValue);
	*value = newValue;
}

int main(){
	TESTCASE(0)// Table_new
		Table_T table = Table_new(256, NULL, NULL);
		assert_notequal(table, 0);
		Table_T table2 = Table_new(INT_MAX, NULL, NULL);
		assert_notequal(table2, 0);
	ENDTEST;
	TESTCASE(1)// Table_put Table_get
		Table_T table = Table_new(256, NULL, NULL);
		Table_put(table, "id", "hiukong");
		Table_put(table, "name", "Hiukong");
		Table_put(table, "birthday", "12");
		assert_equal(Table_length(table), 3);
		assert_equal_string((char*)Table_get(table, "name"), "Hiukong");
	ENDTEST;
	TESTCASE(2)// Table_remove
		Table_T table = Table_new(256, NULL, NULL);
		Table_put(table, "id", "hiukong");
		Table_put(table, "name", "Hiukong");
		Table_put(table, "birthday", "12");
		assert_equal_string(Table_remove(table, "id"), "hiukong");
		assert_equal(Table_length(table), 2);
	ENDTEST;
	TESTCASE(3)// Table_map
		Table_T table = Table_new(256, NULL, NULL);
		Table_put(table, "id", "hiukong");
		char buf[1024];
		memset(buf, '\0', sizeof(buf));
		Table_map(table, repeatValue, (void*)buf);
		assert_equal_string(Table_get(table, "id"), "hiukonghiukong");
		assert_equal_string(buf, "id hiukong ");
	ENDTEST;
	TESTCASE(4) // Table_toArray
		Table_T table = Table_new(256, NULL, NULL);
		Table_put(table, "id", "hiukong");
		Table_put(table, "name", "Hiukong");
		Table_put(table, "birthday", "12");
		char** array = (char**)Table_toArray(table, NULL);
		int i = 0;
		for(; *(array+i); i++)
			;
		assert_equal(i, 6);
	ENDTEST;
	
	return EXIT_SUCCESS;
}