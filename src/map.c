#include <tile.h>
#include <map.h>
#include <ncurses.h>
#include <string.h>

void map_init(map_t* map, int width, int height)
{
	int msize = width*height;
	map->map = (int*)malloc(sizeof(int)*msize);
	map->info = (tileinfo_t*)malloc(sizeof(tileinfo_t)*msize);

	map->width = width;
	map->height = height;

	memset(map->map,0,sizeof(int)*msize);
	memset(map->info,0,sizeof(tileinfo_t)*msize);
}

void map_free(map_t* map)
{
	for(int i=0;i<(map->width*map->height);i++)
		if(TILES[map->map[i]].uninit)
			TILES[map->map[i]].uninit(map, i);

	free(map->map);
	free(map->info);
	memset(map, 0, sizeof(map_t));
}

void map_update(map_t* map)
{
	for(register int i=0;i<(map->width*map->height);i++)
	{
		if(TILES[map->map[i]].update)
			TILES[map->map[i]].update(map, i);
	}
}

void map_plot(map_t* map, int index,int tile)
{
	if(TILES[map->map[index]].uninit)
		TILES[map->map[index]].uninit(map, index);

	if(!(TILES[tile].flags & TILEFLAG_PRESERVE_INFO))
		memset(map->info+index,0,sizeof(tileinfo_t));

	if(TILES[tile].init)
		TILES[tile].init(map, index);
	
	map->map[index] = tile;
}
