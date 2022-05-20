#ifndef MEM_INCLUDED
#define MEM_INCLUDED
#include "except.h"

#define ALLOC(nbytes) Mem_Alloc((nbytes), __FILE__, __LINE__)
#define CALLOC(count, nbytes) Mem_Calloc((count), (nbytes), __FILE__, __LINE__)
#define FREE(p) Mem_Free((p), __FILE__, __LINE__)
#define RESIZE(p, nbytes) ((p) = Mem_Resize((p), (nbytes), __FILE__, __LINE__))
#define NEW(p) ((p) = Mem_Alloc(sizeof *(p), __FILE__, __LINE__))
#define NEW0(p) ((p) = Mem_Calloc(1, sizeof *(p), __FILE__, __LINE__))

extern const Except_T Mem_Failed;

extern void* Mem_Alloc(long nbytes, const char* file, int line);
extern void* Mem_Calloc(long count, long nbytes, const char* file, int line);
extern void Mem_Free(void* p, const char* file, int line);
extern void* Mem_Resize(void* p, long nbytes, const char* file, int line);

#endif