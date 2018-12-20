#include <tile.h>
#include <map.h>

void tile_wall_init(map_t* map, int index)
{
	map->info[index].solid = 1;
}
