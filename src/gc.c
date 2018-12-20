#include <gc.h>

static void* _gc_hooks[GC_HEAP_SIZE];
static int _gc_fillptr;

void gc_init(){
	memset(_gc_hooks, 0, sizeof(void*)*GC_HEAP_SIZE);
	_gc_fillptr = 0;
}

void* gc_hook(void* ptr)
{
	if(_gc_fillptr >= GC_HEAP_SIZE)
		gc_collect(); //dangerous!
	_gc_hooks[_gc_fillptr++] = ptr;
	return ptr;
}

void gc_collect()
{
	for(register int i=0;i<_gc_fillptr;i++)
		free(_gc_hooks[i]);
	_gc_fillptr = 0;
}
