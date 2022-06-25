#include "fmt.h"
#include "asserttest.h"

#include <stdlib.h>
#include <stdio.h>

int main(){
	TESTCASE(0)
		assert_equal(0, 1);
	ENDTEST;
	
	return EXIT_SUCCESS;
}