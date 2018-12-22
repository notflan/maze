#ifndef _CAVE_H
#define _CAVE_H
#include <stdlib.h>
#include <string.h>

typedef struct {
	int w,h;
	int map[];
} cave_t;

//TODO: typedef int (*rulep)(cave_t* cave, int x, int y);

#define cave_free free
cave_t* cave_create(int w,int h);
cave_t* cave_clone(const cave_t* cave);
int cave_at(const cave_t* cave, int x,int y);
void cave_generate(cave_t* cave);
void cave_filter(cave_t* cave, float f,int additive);

#endif /* _CAVE_H */
