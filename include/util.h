#ifndef _UTIL_H
#define _UTIL_H
#include <stdlib.h>

typedef struct {
	int x,y;
} vec2;

#define CURSOR_MODE_VIEW 0

double frandom();
int chance(float f);
long rrandom(long max);
void _xy(int a,int b,int *c,int *d);

extern vec2 cursor;

#endif /* _UTIL_H */

