#include <tile.h>
#include <map.h>
#include <string.h>
#include <util.h>

int isoktospawn(map_t* map, int x,int y)
{
	return x>=0 && x<map->width && y>=0 && y<map->height
		&& !INFO_AT(map, x,y).solid;
}

void vecadd(int x,int y, int *_x, int *_y)
{
	*_x +=x;
	*_y +=y;
}

void dospawn(map_t* map, int i)
{
	int state0 = map->map[i];
	tileinfo_t state1 = map->info[i];

	map->map[i] = TILE_WATER;
	memset(&map->info[i],0,sizeof(tileinfo_t));

	TILES[TILE_WATER].init(map, i);

	map->info[i].td[0].i32 = state0;
	memcpy(map->info[i].data, &state1, sizeof(tileinfo_t));
}

TILEPROTO(tile_spout_update)
{
	static const int offsets[] =  {
		0,-1,
		+1,-1,
		+1,0,
		+1,+1,
		0,+1,
		-1,+1,
		-1,0,
		-1,-1,
	};

	tile_basic_draw(map,i);

	if(chance(.2f))
	{
		int sos[8];
		memset(sos,0xff,8*sizeof(int));
		for(register int j=0;j<15;j+=2)
		{
			int x,y;
			_xy(XY(map, i), &x, &y);
			vecadd(offsets[j], offsets[j+1], &x, &y);
			if(isoktospawn(map, x, y))
				sos[j/2] = j;
		}

		//choose one
		int rej = sos[rrandom(7)];
		
		if(rej!=-1) {
			int x,y;
			_xy(XY(map, i), &x, &y);
			vecadd(offsets[rej], offsets[rej+1], &x, &y);
			
			dospawn(map, x+(y*map->width));

		}
	}
}
