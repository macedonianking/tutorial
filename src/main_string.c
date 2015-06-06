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

void main_string_initial(struct main_string *ptr, int capacity)
{
	if (capacity <= 0)
		capacity = 1024;
	ptr->c = capacity;
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

void main_string_append(struct main_string *ptr, char *data)
{
	main_string_nappend(ptr, data, strlen(data));
}

void main_string_nappend(struct main_string *ptr, char *data, int n)
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

void main_string_erase(struct main_string *ptr)
{
	ptr->n = 0;
	ptr->data[ptr->n] = E_CHAR;
}

char *main_string_new(struct main_string *ptr)
{
	char *p;

	p = (char*) malloc(sizeof(char) * (ptr->n + 1));
	strcpy(p, ptr->data);
	return p;
}
