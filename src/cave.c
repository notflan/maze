#include <util.h>
#include <cave.h>

int cave_at(const cave_t *t, int x, int y)
{
	if(x<0||x>=t->w||
	   y<0||y>=t->h) return 1;
	else return t->map[(y*t->w)+x];
}

void neigh(const cave_t* t, int x,int y, int i[8])
{
	i[0] = cave_at(t, x, y-1);
	i[1] = cave_at(t, x+1, y-1);
	i[2] = cave_at(t, x+1, y);
	i[3] = cave_at(t, x+1, y+1);
	i[4] = cave_at(t, x,y+1);
	i[5] = cave_at(t, x-1, y+1);
	i[6] = cave_at(t, x-1, y);
	i[7] = cave_at(t, x-1, y-1);
}

cave_t *cave_create(int w,int h)
{
	cave_t* cave = malloc(sizeof(cave_t)+((w*h)*sizeof(int)));

	cave->w = w;
	cave->h = h;

	for(register int i=0;i<(w*h);i++)
	{
		cave->map[i] = chance(0.4f);
	}

	return cave;
}

cave_t* cave_clone(const cave_t* cave)
{
	cave_t* _cave = malloc(sizeof(cave_t)+(cave->w*cave->h*sizeof(int)));

	_cave->w = cave->w;
	_cave->h = cave->h;

	memcpy(_cave->map, cave->map, sizeof(cave->w*cave->h*sizeof(int)));

	return _cave;
}

int _sum(int n[8])
{
	register int i=0,j=0;
	for(;i<8;i++)
		j+=n[i];
	return j;
}

#define AT(x,y) ((x)+((y)*cave->w))

int rulecheck(const cave_t* cave, int x,int y)
{
	int n[8];
	int ns;
	
	memset(n, 0, sizeof(int)*8);

	neigh(cave, x,y,n);
	ns = _sum(n);

	return (cave->map[AT(x,y)] && (ns>=4)) ||
		(!cave->map[AT(x,y)] && ns>3);

}

void cave_generate(cave_t* cave)
{
	cave_t* gen = cave_clone(cave);
	
	for(register int x=0;x<cave->w;x++)
		for(register int y=0;y<cave->h;y++)
			gen->map[x+(y*cave->w)] = rulecheck(cave, x,y);
	memcpy(cave->map, gen->map, cave->w*cave->h*sizeof(int));
	free(gen);
}

void cave_filter(cave_t* cave, float cut, int m)
{
	for(register int y=0;y<cave->h;y++)
		for(register int x=0;x<cave->w;x++)
		{
			int n[8];
			int ns;
			
			memset(n,0,sizeof(int)*8);
			neigh(cave, x,y,n);
			ns = _sum(n);

			float freq = 1.f / ((float)ns);

			if(!m)
				cave->map[x+(y*cave->w)] *= freq<cut;
			else
				cave->map[x+(y*cave->w)] = freq>=cut;
		}
}

void cave_blank(cave_t* cave, int fac)
{
	//TODO: Add horizontal blank
}
