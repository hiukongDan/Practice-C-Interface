#ifndef ARENA_INCLUDED
#define ARENA_INCLUDED

#include "except.h"

#define T Arena_T

extern const Except_T Arena_NewFailed;
extern const Except_T Arena_Failed;

typedef struct T *T;

struct header;

extern T Arena_new(void);
extern void Arena_dispose(T *ap);
extern void* Arena_alloc(T arena, long nbytes, const char* file, long line);
extern void* Arena_calloc(T arena, long count, long nbytes, const char* file, long line);
extern void Arena_free(T arena);


#undef T
#endif