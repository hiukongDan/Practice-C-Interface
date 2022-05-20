#ifndef LIST_INCLUDED
#define LIST_INCLUDED

#define T List_T
typedef struct T *T;

struct T{
	T rest;
	void *first;
};

extern T List_append(T list, T tail);
extern T List_push(T list, void *x);
extern T List_pop(T list, void **x);
extern long List_length(T list);
extern T List_list(void *x, ...);	// arguments ending with NULL
extern void List_free(T *list);
extern T List_reverse(T list);
extern void List_map(T list, void apply(void **x, void *cl), void *cl);
extern T List_copy(T list);
extern void** List_toArray(T list, void* end);

#undef T
#endif