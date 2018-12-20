#include <string.h>
#include <stdlib.h>
//#include <stdio.h>
#include <stdarg.h>
#include <ncurses.h>
#include <text.h>

void fullprint(const char* buf)
{
	int attrs = 0;
	while(*buf)
	{
		switch(*buf)
		{
			case '\xf0': //formatting
				switch(*(buf+1))
				{
					case 'l':
						attron(A_BOLD);
						attrs |= A_BOLD;
						break;
					case 'u':
						attron(A_UNDERLINE);
						attrs |= A_UNDERLINE;
						break;
					case 'r':
						attroff(attrs);
						attrs = 0;
						break;
				}
				buf+=1;
				break;
			case '\xf1': //colour
				buf+=1;
				attron(COLOR_PAIR(((int)(*buf)-1)));
				attrs |= COLOR_PAIR((int)(*buf)-1);
				
				break;
			case '\xf2': //remove colour
				buf+=1;
				
				if(*buf && attrs & COLOR_PAIR((int)(*buf)-1)) {
					attroff(COLOR_PAIR((int)(*buf)-1));
					attrs ^= COLOR_PAIR((int)(*buf)-1); 
				}
				break;
			default:
				addch(*buf);
				break;
		}
		buf+=1;
	}
	if(attrs) attroff(attrs);
}

void textf(int x,int y, const char* format, ...)
{
	char buf[TEXT_MAX+1];
	va_list va;
	va_start(va, format);
	memset(buf,0,TEXT_MAX+1);

	vsnprintf(buf, TEXT_MAX, format, va);

	va_end(va);

	move(y,x);
	fullprint(buf);
}
