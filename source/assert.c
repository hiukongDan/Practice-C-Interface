#include "assert.h"

const Except_T Assert_failed = {"Assertion failed"};

void (assert)(int e){
	assert(e);
}