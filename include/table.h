#ifndef TABLE_INCLUDED
#define TABLE_INCLDUED

#include "except.h"

#define T Table_T

typedef struct T *T;

extern T Table_new(int hint, int cmp(const void* a, const void* b), unsigned hash(const void* key));
extern void Table_free(T *table);
extern void* Table_put(T Table, const void* key, void* value);
extern void* Table_get(T table, const void* key);
extern void* Table_remove(T table, const void* key);
extern int Table_length(T table);
extern void Table_map(T table, void apply(const void* key, void** value, void* cl), void* cl);
extern void** Table_toArray(T table, void* end);

#undef T
#endif