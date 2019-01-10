/// Functions for drawing and manipulating map
#ifndef _MAP_H
#define _MAP_H
#include <tile.h>
#include <stdio.h>

#define MAP_AT(map, x,y) map->map[(x)+((y)*map->width)]
#define INFO_AT(map, x,y) map->info[(x)+((y)*map->width)]

void map_flag(map_t* map, int i, char flag);
void map_init(map_t* map, int w,int h);
void map_free(map_t* map);
void map_plot(map_t* map, int index,int tile);
void map_update(map_t* map);

void map_save(map_t* map, FILE* fp);
void map_load(map_t* map, FILE* fp);

#endif /* _MAP_H */
