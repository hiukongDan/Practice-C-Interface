#ifndef ASSERTTEST_INCLUDED
#define ASSERTTEST_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define assert_equal(a, b) do{\
	if ((a) == (b)){\
		printf(".");\
	}else{\
		printf("x");\
	}\
	}while(0)
		
#define assert_equal_string(a, b) do{\
	if(strcmp((a), (b)) == 0){\
		printf(".");\
	}else{\
		printf("x");\
	}\
	}while(0)

#define assert_notequal(a, b) do{\
	if ((a) != (b)){\
		printf(".");\
	}else{\
		printf("x");\
	}\
	}while(0)

#define assert_greater(a, b) do{\
	if ((a) > (b)){\
		printf(".");\
	}else{\
		printf("x");\
	}\
	}while(0)
		
#define assert_lesser(a, b) do{\
	if ((a) < (b)){\
		printf(".");\
	}else{\
		printf("x");\
	}\
	}while(0)
		
#define assert_sametype(a, b) do{\
	if (typeof(a) == typeof(b)){\
		printf(".");\
	}else{\
		printf("x");\
	}\
	}while(0)

#define TESTCASE(id) do{\
	printf("Testcase %d:\n", (id));\

#define ENDTEST printf("\n");}while(0)

#endif