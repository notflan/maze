#include <tile.h>
#include <map.h>
#include <util.h>

//td0 = currently absorbed
//td1 = max til dissolve

void tile_dirt_init(map_t* map, int index)
{
	int32_t *abso = &map->info[index].td[0].i32;
	int32_t *abso_max = &map->info[index].td[1].i32;

	map->info[index].solid= 1;

	*abso = 0;
	*abso_max =(int) rrandom(3)+1;
}

void tile_dirt_update(map_t* map, int index)
{
	tile_basic_draw(map, index);
}
