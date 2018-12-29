#include <tile.h>
#include <util.h>
#include <map.h>
#include <ncurses.h>

void vcol(map_t* map, int x,int y)
{
	int i = x+(y*map->width);
	if(x<0 || x>=map->width || y<0 || y>=map->height) return;
	else if(map->map[i] == TILE_EMPTY)
	{
		//int mvi = mvinch(y,x);
		tile_draw(x,y, ' ' | COLOR_PAIR(5));
		map_flag(map, i, RF_SUSPEND);
	}
}

TILEPROTO(tile_suishou_update) {
	tileinfo_t* info = &map->info[i];
	info->td[0].i32 +=1;

	if(info->td[0].i32 > 15)
	{
		if( (++info->td[1].i32)>=4)
			info->td[1].i32 = 0;
		info->td[0].i32 = 0;
	}

	int x,y;
	_xy(XY(map,i), &x,&y);

	vcol(map, x,y-1);
	vcol(map, x+1, y);
	vcol(map, x,y+1);
	vcol(map, x-1, y);

	tile_draw(XY(map,i), '#' | COLOR_PAIR((SUISHOU_COLOUR+info->td[1].i32)));
}
