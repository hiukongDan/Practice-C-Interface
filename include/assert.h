#undef assert
#ifdef NDEBUG
#define assert(e) ((void)0)
#else
#include "except.h"
extern const Except_T Assert_failed;
extern void assert(int e);
#define assert(e) ((void)((e) || (RAISE(Assert_failed), 0)))
#endif