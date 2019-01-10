#include <tile.h>
#include <map.h>
#include <ncurses.h>
#include <string.h>
#include <stdint.h>

struct _smap_header {
	uint32_t width,height;
};

void map_save(map_t* map, FILE* fp)
{
	struct _smap_header header;
	int msize = map->width*map->height;

	header.width  = (uint32_t)map->width;
	header.height = (uint32_t)map->height;

	fwrite(&header, sizeof(struct _smap_header), 1, fp);

	fwrite(map->map, sizeof(int), msize, fp);
	fwrite(map->info, sizeof(tileinfo_t), msize, fp);
	fwrite(map->rolling, 1, msize, fp);

}

void map_load(map_t* map, FILE* fp)
{
	struct _smap_header header;
	int msize;

	fread(&header, sizeof(struct _smap_header), 1, fp);
	msize = (int)(header.width*header.height);
	map_init(map, (int)header.width, (int)header.height);

	fread(map->map, sizeof(int), msize, fp);
	fread(map->info, sizeof(tileinfo_t), msize, fp);
	fread(map->rolling, 1, msize, fp);
}

void map_init(map_t* map, int width, int height)
{
	int msize = width*height;
	map->map = (int*)malloc(sizeof(int)*msize);
	map->info = (tileinfo_t*)malloc(sizeof(tileinfo_t)*msize);

	map->width = width;
	map->height = height;

	map->rolling = malloc(msize);

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
	free(map->rolling);

	memset(map, 0, sizeof(map_t));
}

void map_update(map_t* map)
{
	memset(map->rolling, 0, map->width*map->height);
	for(register int i=0;i<(map->width*map->height);i++)
	{
		if(!(map->rolling[i] & RF_SUSPEND))
			if(TILES[map->map[i]].update)
				TILES[map->map[i]].update(map, i);
	}
}

void map_flag(map_t* map, int i, char flag)
{
	map->rolling[i] |= flag;
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
