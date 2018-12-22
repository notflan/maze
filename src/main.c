#include <ncurses.h>
#include <gc.h>
#include <tile.h>
#include <map.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <text.h>
#include <cave.h>

#define REFRESH_RATE 30.f

map_t global_map;
int alive;

void populate_map(map_t* map);

int usleep(unsigned int time); //lmao
float wait(float rate, unsigned int miss)
{
	long tm = (long)((1.f/rate) * (float)1e6) - miss;
	if(tm>0)
		usleep((unsigned int)tm);
	else return 0.f;

	return ((float)tm)/1000.f;
}

unsigned long time_micro()
{
	struct timespec t;

	clock_gettime(CLOCK_REALTIME,  &t);

	return (t.tv_sec * 1e6) + t.tv_nsec / 1000;
}

void colour_init()
{
	start_color();

	init_color(COLOR_RED, 1000,0,0);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);

	init_pair(4, COLOR_BLUE, COLOR_BLACK);
}

int main()
{
	int width, height;

	alive=1;

	srandom(time(NULL));

	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	noecho();
	colour_init();
	getmaxyx(stdscr ,height, width);
	height-=1;

	map_init(&global_map, width, height);

	populate_map(&global_map);

	while(alive)
	{
		unsigned long t_prev = time_micro();

		int held = getch();

		if(held!=ERR)
		{
			char c = (char)held;
			if(c=='q')
				alive=0;
			///TODO: Add keys held to global buffer
	//		else
	//			ungetch(held);
		}
		map_update(&global_map);
		//textf(0,height, "He\xf1\x03lllo\xf0r!");
		refresh();

		float tm = wait(REFRESH_RATE, (unsigned int) (time_micro()-t_prev));	
		textf(0, height, "Running at %.1fHz (\xf1\x03\xf0u%.2f\xf0r ms limited)", REFRESH_RATE, tm);
	}

	map_free(&global_map);
	endwin();
	return 0;
}

void gen_cave(map_t* map,int gens)
{
	cave_t* cave =  cave_create(map->width,map->height);

	for(register int i=0;i<gens;i++)
		cave_generate(cave);

	for(register int y=0;y<map->height;y++)
		for(register int x=0;x<map->width;x++)
			map_plot(map, x+(y*cave->w), (cave->map[x+(y*cave->w)])?TILE_WALL:TILE_EMPTY);

	free(cave);
}

void populate_map(map_t* map)
{
	gen_cave(map, 4);
	/*
	map_plot(map, map->width*5 +5, TILE_WATER);
	map_plot(map, map->width*5 +6, TILE_WATER);
	map_plot(map, map->width*4 +5, TILE_SPOUT);
	map_plot(map, map->width*4 +4, TILE_SPOUT);
	
	for(register int y=7;y<=17;y++)
		for(register int x=3;x<=6;x++)
			map_plot(map, map->width*y+x,TILE_DIRT);
	map_plot(map, map->width*6+2, TILE_WALL);
	map_plot(map, map->width*6+7, TILE_WALL);
	map_plot(map, map->width*(map->height-6) +3, TILE_WALL);
	map_plot(map, map->width*(map->height-6) +7, TILE_WALL);
	for(register int i=3;i<8;i++)
		map_plot(map, map->width*(map->height-5) +i, (i==5?TILE_DIRT:TILE_WALL));*/
		
}
