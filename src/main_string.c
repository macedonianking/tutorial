/*
 * main_string.c
 *
 *  Created on: 2015年6月5日
 *      Author: lhb
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_string.h"
#include "config.h"

#define MAIN_STRING_DEFAULT_CAPACITY 	512

void main_string_initial(main_string *ptr)
{
	main_string_ninitial(MAIN_STRING_DEFAULT_CAPACITY);
}

void main_string_ninitial(struct main_string *ptr, int c)
{
	ptr->c = c > 0 ? c : MAIN_STRING_DEFAULT_CAPACITY;
	ptr->data = malloc(sizeof(char) * ptr->c);
	ptr->n = 0;
	ptr->data[ptr->n] = E_CHAR;
}

void main_string_release(struct main_string *ptr)
{
	if (ptr != NULL)
	{
		free(ptr->data);
		ptr->data = NULL;
		ptr->c = ptr->n = 0;
	}
}

void main_string_append(struct main_string *ptr, const char *data)
{
	main_string_nappend(ptr, data, strlen(data));
}

void main_string_nappend(struct main_string *ptr, const char *data, int n)
{
	if (ptr->n + n + 1 > ptr->c)
	{
		ptr->c <<= 1;
		if (ptr->n + n + 1 > ptr->c)
			ptr->c = ptr->n + n + 1;
		ptr->data = (char*) realloc(ptr->data, ptr->c * sizeof(char));
	}

	while (n-- > 0)
		ptr->data[ptr->n++] = *data++;
	ptr->data[ptr->n] = E_CHAR;
}

void main_string_reset(struct main_string *ptr)
{
	ptr->n = 0;
	ptr->data[ptr->n] = E_CHAR;
}

void main_string_assign(struct main_string *ptr, const char *data)
{
	main_string_reset(ptr);
	main_string_append(ptr, data);
}

void main_string_nassign(struct main_string *ptr, const char *data, int n)
{
	main_string_reset(ptr);
	main_string_nappend(ptr, data, n);
}

char *main_string_new(struct main_string *ptr)
{
	char *p;

	p = (char*) malloc(sizeof(char) * (ptr->n + 1));
	strcpy(p, ptr->data);
	return p;
}

char *main_string_new_string(const char *str)
{
	char *dst;
	int n;

	n = strlen(str);
	dst = (char*) malloc(sizeof(char) * (n + 1));
	memcpy(dst, str, n + 1);

	return dst;
}
