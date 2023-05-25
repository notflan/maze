#include <ncurses.h>
#include <gc.h>
#include <tile.h>
#include <map.h>
#include <util.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <text.h>
#include <cave.h>

#define REFRESH_RATE 30.f

vec2 cursor;
int cursor_mode;

map_t global_map;
int alive;

struct arg_state;

void populate_map(map_t* map, const struct arg_state *restrict args);

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

void real_colour(uint32_t argb, uint32_t *r, uint32_t *g, uint32_t *b)
{
	uint8_t rr,rg,rb;
	uint8_t* mask = (uint8_t*)&argb; //endianess???
	
	rr=mask[2];
	rg=mask[1];
	rb=mask[0];

#define BASE_1000(v) (int)(( ((float)(v)) / 255.f ) * 1000.f)
	*r = BASE_1000(rr);
	*g = BASE_1000(rg);
	*b = BASE_1000(rb);
#undef BASE_1000
}

void init_colour_real(int colour, uint32_t rcol)
{
	uint32_t r,g,b;
	real_colour(rcol, &r, &g, &b);
	init_color(colour, r,g,b);
}

void colour_init()
{
	start_color();

	init_color(COLOR_RED, 1000,0,0);

	init_colour_real(0xf0, 0x0491dd);
	init_colour_real(0xf1, 0x1dd3f7);
	init_colour_real(0xf2, 0x9fe4f2);
	init_colour_real(0xf3, 0xdceaed);
	init_colour_real(0xf4, 0x00060a);

	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);

	init_pair(4, COLOR_BLUE, COLOR_BLACK);

	init_pair(5, COLOR_BLACK, 0xf4);

	init_pair(SUISHOU_COLOUR+0, 0xf0, 0xf4);
	init_pair(SUISHOU_COLOUR+1, 0xf1, 0xf4);
	init_pair(SUISHOU_COLOUR+2, 0xf2, 0xf4);
	init_pair(SUISHOU_COLOUR+3, 0xf3, 0xf4);
}

void blankmessage(int height,int width)
{
	for(register int i=0;i<width;i++)
		textf(i, height, " ");
}

struct arg_state {
	int gen_flags;
};


#define CFLAG_POST_FILTER	(1<<0) //makes cave more jagged
#define CFLAG_PRE_FILTER	(1<<1) //makes cave more open

#define CFLAG_OPEN		(CFLAG_POST_FILTER|CFLAG_PRE_FILTER) //full open

__attribute__((nonnull(3)))
int loadflags(int argc, char** argv, struct arg_state* restrict state)
{
	while( *++argv ) {
		switch(**argv) {
			case 'j': state->gen_flags |= CFLAG_POST_FILTER; if(0)
			case 'o': state->gen_flags |= CFLAG_PRE_FILTER; if(0)
			case 'f': state->gen_flags |= CFLAG_OPEN; if(0) 
			case 0: break; continue;
			default: if(*argv) fprintf(stderr, "Unknown argument: '%s'\nValid args: `jagged`, `open`, `full-open`\n", *(argv)); continue;
		}
		break;
	}
	return 0;
}

int main(int argc, char** argv)
{
	int lfe;
	struct arg_state astate = {0};
	if( argc > 1 && __builtin_expect(lfe = loadflags(argc, argv, &astate), 0) ) {
		fprintf(stderr, "Error parsing arguments: %d (%d)\n", lfe, lfe | 4); return lfe | 4;
	} //else fprintf(stderr, "Flags: %d\n", astate.gen_flags);
	
	int width, height;

	memset(&cursor,0,sizeof(vec2));
	cursor_mode = CURSOR_MODE_VIEW;

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

	populate_map(&global_map, &astate);

	while(alive)
	{
		unsigned long t_prev = time_micro();

		int held = getch();

		if(held!=ERR)
		{
			char c = (char)held;
			if(c=='q')
				alive=0;
			else if(c=='r')
				populate_map(&global_map, &astate);
#define SAVE_FILE_NAME "saved.map"
			else if(c=='k')
			{
				FILE* fp = fopen(SAVE_FILE_NAME, "wb");
				map_save(&global_map, fp);
				fclose(fp);
			}
			else if(c=='l')
			{
				//TODO: fix size issues
				
				FILE* fp = fopen(SAVE_FILE_NAME,"rb");
				map_free(&global_map);
				map_load(&global_map, fp);
				fclose(fp);
			}
			else if(cursor_mode == CURSOR_MODE_VIEW)
			{
				if(c == 'a'  && cursor.x>0)
					cursor.x-=1;
				if(c == 'd'  && cursor.x<(global_map.width-1))
					cursor.x+=1;
				if(c == 'w'    && cursor.y>0)
					cursor.y-=1;
				if(c == 's'  && cursor.y<(global_map.height-1))
					cursor.y+=1;
				
			}
			///TODO: Add keys held to global buffer
	//		else
	//			ungetch(held);
		}
		map_update(&global_map);
		//textf(0,height, "He\xf1\x03lllo\xf0r!");
	
		blankmessage(height, width);
			
		float tm = wait(REFRESH_RATE, (unsigned int) (time_micro()-t_prev));
		
		if(cursor_mode == CURSOR_MODE_VIEW) {
			int i = cursor.x+(cursor.y*global_map.width);
			textf(0, height, "[%d, %d] %s: %s", cursor.x, cursor.y, TILES[global_map.map[i]].name, TILES[global_map.map[i]].desc);
			wmove(stdscr, cursor.y,cursor.x);
		}
		else {
			textf(0, height, "Running at %.1fHz (\xf1\x03\xf0u%.2f\xf0r ms limited)", REFRESH_RATE, tm);
		}
		refresh();
	}

	map_free(&global_map);
	endwin();
	return 0;
}

void gen_cave(map_t* map,int gens, unsigned int cflags)
{
	cave_t* cave =  cave_create(map->width,map->height);

	if(cflags & CFLAG_PRE_FILTER)
		cave_filter(cave, .5f, 1);

	for(register int i=0;i<gens;i++)
		cave_generate(cave);

	if(cflags & CFLAG_POST_FILTER)
		cave_filter(cave, .8f, 0);

	for(register int y=0;y<map->height;y++)
		for(register int x=0;x<map->width;x++)
			map_plot(map, x+(y*cave->w), (cave->map[x+(y*cave->w)])?
					((!cave_at(cave, x-1, y))&&(!cave_at(cave, x+1, y))? TILE_CRYSTAL:TILE_WALL)
					:TILE_EMPTY);

	free(cave);
}

void populate_map(map_t* map, const struct arg_state* restrict state)
{
	gen_cave(map, 4, state ? state->gen_flags : 0);
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
