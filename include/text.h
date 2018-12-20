#ifndef _TEXT_H
#define _TEXT_H
#include <stdarg.h>

#define TEXT_MAX 255

void fullprint(const char* buf);
void textf(int x,int y,const char* fmt, ...);


#endif /* _TEXT_H */
