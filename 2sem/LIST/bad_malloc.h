#ifndef BAD_MALLOC_H
#define BAD_MALLOC_H
#include <stdlib.h>

void* bad_malloc(size_t num)
{
	/*if(num > 100)
		return NULL;*/
	if(rand() % 123 ==  0)
		return NULL;
	else
	{
		return malloc(num);
	}
}
#endif