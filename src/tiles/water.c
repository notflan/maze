#include <tile.h>
#include <map.h>
#include <util.h>
#include <string.h>
#include <assert.h>
#include <text.h>

#define water_prevtile (map->info[i].td[0].i32)
#define water_prevtileinfo ((tileinfo_t*)map->info[i].data)
#define water_auframe (map->info[i].td[1].i32)

TILEPROTO(tile_water_init) {
	tileinfo_t *info = map->info+i;

	info->solid = 1;

	water_prevtile = TILE_EMPTY; //previous tile index (for moving)
	water_auframe = 0; //have we already updated this frame?
//	water_prevchar = TILES[TILE_EMPTY].def; ///For now, just infer this

	info->data = malloc(sizeof(tileinfo_t)); //for moving over other tiles, we need to save their state	
	memset(info->data,0, sizeof(tileinfo_t));
}

TILEPROTO(tile_water_destroy) {
	free(map->info[i].data);
}

static int  water_move(map_t* map, int from, int to)
{
	int i = from;
	//save map state tmp
	int state0 = map->map[to];
	tileinfo_t state1 = map->info[to];

	int hstate0 = water_prevtile;
	tileinfo_t hstate1;

	memcpy(&hstate1, map->info[from].data, sizeof(tileinfo_t));	

	//set our new copy
	water_prevtile = state0;
	memcpy(map->info[from].data, &state1, sizeof(tileinfo_t));

	//move over
	map->map[to] = map->map[from];
	map->info[to] = map->info[from];

	//return prev tile to state
	map->map[from] = hstate0;
	map->info[from] = hstate1;

	tile_draw(XY(map, from),TILES[hstate0].def); 

	return to;
}

static int cin(const int *c, int size)
{
	 int rr = (int)rrandom(size-1);
	 while(rr > 0 && c[rr] == 0) rr-=1;
	 return c[rr];
}

static int abscheck(map_t* map, int i, int* ret)
{
	int chances[] = {
		0,	 //move left
		0,	 //move right
		0, 0, 0 //move down
	}; //XXX: Be careful changing this to change the below finder values too

	int x,y;
	_xy(XY(map, i), &x, &y);

	if( y< map->height-1 && MAP_AT(map, x, y+1) == TILE_DIRT)
		chances[2] = map->width;
	if (x>0 && MAP_AT(map, x-1, y) == TILE_DIRT)
		chances[0] = -1;
	if (x<map->width-1 && MAP_AT(map, x+1, y) == TILE_DIRT)
		chances[1] = 1;
	
	int rcmove = cin(chances, sizeof(chances)/sizeof(int));
	
	if(rcmove && chance(0.4f))
	{
		*ret = i+rcmove;
		return 1;
	}
	return 0;
}

static void absorbe(map_t* map, int from ,int to)
{
	int i=from;
	tileinfo_t * dirt = map->info+to;

	tile_draw(XY(map, from), TILES[water_prevtile].def);

	void* data = map->info[from].data;

	memcpy(&map->info[from], data, sizeof(tileinfo_t));
	map->map[from] = water_prevtile;

	free(data); //Manually despawn this tile

	dirt->td[0].i32 += 1;

	if(dirt->td[0].i32 >= dirt->td[1].i32)
	{
		//despawn dirt
		map_plot(map, to, TILE_WATER);
	}
}

static void draw(map_t* map, int i)
{
	int x,y;
	_xy(XY(map,i), &x,&y);

	//TODO: Colour more `deeper' if all neighbours are water
	
	tile_basic_draw(map,i);
}

TILEPROTO(tile_water_update) {
	if(water_auframe) {
		water_auframe = 0;
	}
	else {
		int x,y;

		_xy(XY(map,i), &x,&y);

		//can we move down?
		if((y<map->height-1) && !INFO_AT(map, x, y+1).solid) {
			water_auframe = 1;
			i = water_move(map, i, i+map->width);
			
		}
		else {
			int abst;

			if(abscheck(map, i, &abst))
			{
				absorbe(map, i, abst);	
				return;	
			}
			//try to move sideways
			int lok, rok;
			lok = x>0 && !INFO_AT(map,x-1,y).solid;
			rok = x<(map->width-1) && !INFO_AT(map, x+1, y).solid;
			
			int mv =0;

			if(lok && rok)
				mv = rrandom(2) - 1;
			else if(lok)
				mv = -1;
			else if(rok)
				mv = 1;
			
			if(mv!=0)
			{
		//		water_auframe = mv>0; //can we just let it update twice?
				i = water_move(map, i, i+mv);
					
			}
		}
		//water_auframe = 1; // set this when we move down or right
		//tile_basic_draw(map,i);
		draw(map,i);
	}
}
