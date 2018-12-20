#ifndef _GC_H
#define _GC_H
#include <string.h>
#include <stdlib.h>

#define GC_HEAP_SIZE 32

void gc_init();
void gc_collect();
void* gc_hook(void* ptr);

#endif /* _GC_H */
