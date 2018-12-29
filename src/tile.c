#include <tile.h>
#include <ncurses.h>

void tile_basic_draw(map_t* map, int i)
{
	tile_draw(XY(map, i), TILES[map->map[i]].def);
}

void tile_solid_init(map_t* map, int i)
{
	map->info[i].solid = 1;
}

const tile_t TILES[TILES_SIZE] = {
	{TILE_EMPTY, ' ', "Empty", "Blank space", 0, NULL, NULL, &tile_basic_draw},
	{TILE_WALL, '#' | A_BOLD, "Wall", "Solid thing", 0, &tile_wall_init, NULL, &tile_basic_draw},
	{TILE_DIRT, '#', "Dirt", "Not so solid thing", 0, &tile_dirt_init, NULL, &tile_dirt_update},
	{TILE_WATER, '.' | COLOR_PAIR(4), "Water", "Slippery wet thing", 0, &tile_water_init, &tile_water_destroy, &tile_water_update},
	{TILE_SPOUT, '@' | COLOR_PAIR(3), "Waterspout", "Makes the slippy wet things", 0, &tile_solid_init, NULL, &tile_spout_update},
	{TILE_CRYSTAL, '#' | COLOR_PAIR(SUISHOU_COLOUR), "Crystal", "Shine shine~~", 0, NULL, NULL, &tile_suishou_update},
};

void tile_draw(int x,int y, int c)
{
	mvaddch(y, x, c);
}
