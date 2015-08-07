/*
 * main_vector.c
 *
 *  Created on: 2015年6月7日
 *      Author: lhb
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "main_vector.h"
#include "main_string.h"

#define MAIN_VECTOR_DEFAULT_CAPACITY 512

void main_vector_initial(struct main_vector *p)
{
	p->c = MAIN_VECTOR_DEFAULT_CAPACITY;
	p->head = (void **) malloc(sizeof(void*) * p->c);
	for (int i = 0; i < p->c; ++i)
		p->head[i] = NULL;
	p->n = 0;
}

void main_vector_release(struct main_vector *p)
{
	for (int i = 0; i < p->n; ++i)
	{
		free(p->head[i]);
	}
	free(p->head);
	p->head = NULL;
	p->c = p->n = 0;
}

int main_vector_append(struct main_vector *p, void *data)
{
	int r;

	if (p->n == p->c)
	{
		p->c *= 2;
		p->head = (void**) malloc(sizeof(void*) * p->c);
		for (int i = p->n; i < p->c; ++i)
			p->head[i] = NULL;
	}
	r = p->n++;
	p->head[r] = data;

	return r;
}

void main_vector_erase(struct main_vector *p, int i)
{
	while (++i < p->n)
		p->head[i - 1] = p->head[i];
	--p->n;
	p->head[p->n] = NULL;
}

void main_vector_vappend(main_vector *ptr, ...)
{
	va_list _vaList;
	const char *iter;

	va_start(_vaList, ptr);
	while ((iter = va_arg(_vaList, const char*)) != NULL)
	{
		main_vector_append(ptr, mainStringCloneString(iter));
	}
	va_end(_vaList);
}
