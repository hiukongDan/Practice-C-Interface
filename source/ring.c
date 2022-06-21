#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "mem.h"
#include "assert.h"
#include "ring.h"


#define T Ring_T

struct T{
	struct node{
		struct node *llink, *rlink;
		void *value;
	} *head;
	int length;
};

T Ring_new(){
	
}