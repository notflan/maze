#ifndef _TILE_H
#define _TILE_H
#include <stdlib.h>
#include <stdint.h>

#define TILEFLAG_PRESERVE_INFO 1 //Preserve previous tile's info when init()ing

union tiledata {
	int8_t i8;
	uint8_t u8;

	int16_t i16;
	uint16_t u16;

	int32_t i32;
	uint32_t u32;

	int64_t  i64;
	uint64_t u64;	

	float f;
	double d;
};

typedef struct {
	//we might want some more stuff in here

	int solid;

	union tiledata td[8];
	
	//tile arbitary data
	void* data;
} tileinfo_t;


typedef struct {
	int *map;
	int width,height;
	tileinfo_t *info;
	char *rolling;
} map_t;

#define RF_SUSPEND (1<<0)

typedef struct {
	int id;
	int def;
	const char* name;
	const char* desc;

	unsigned int flags;

	void (*init) (map_t* map, int index);
	void (*uninit) (map_t* map, int index);
	void (*update) (map_t* map, int index);
} tile_t;

#define TILE_EMPTY	0
#define TILE_WALL	1
#define TILE_DIRT	2
#define TILE_WATER	3
#define TILE_SPOUT	4
#define TILE_CRYSTAL	5

#define TILEPROTO(n) void n(map_t* map, int i)
void tile_basic_draw(map_t*map,int i);

void tile_wall_init(map_t* map,int i);

void tile_dirt_init(map_t* map,int i);
void tile_dirt_update(map_t* map, int i);

TILEPROTO(tile_water_init);
TILEPROTO(tile_water_update);
TILEPROTO(tile_water_destroy);

TILEPROTO(tile_spout_update);

#define SUISHOU_COLOUR 0x50
TILEPROTO(tile_suishou_update);

#define TILES_SIZE 6

const extern tile_t TILES[TILES_SIZE];

#define XY(map, i) (i%(map->width)),((int)i/(map->width))

void tile_draw(int x,int y, int c);

#endif /* _TILE_H */
