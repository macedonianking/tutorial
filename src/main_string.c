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
#include "main_math.h"
#include "config.h"

#define MAIN_STRING_DEFAULT_CAPACITY 	512

/**
 * 检查设置缓冲区的大小
 */
static void mainStringCheckCapacity(main_string *ptr, int c)
{
	if (c < ptr->c)
	{
		return;
	}

	ptr->c = MAX(c, 2 * ptr->c);
	ptr->data = (char*) realloc(ptr->data, sizeof(char) * ptr->c);
}

void mainStringInitial(main_string *ptr)
{
	mainStringNinitial(ptr, MAIN_STRING_DEFAULT_CAPACITY);
}

void mainStringNinitial(struct main_string *ptr, int c)
{
	ptr->c = c > 0 ? c : MAIN_STRING_DEFAULT_CAPACITY;
	ptr->data = malloc(sizeof(char) * ptr->c);
	ptr->n = 0;
	ptr->data[ptr->n] = E_CHAR;
}

void mainStringRelease(struct main_string *ptr)
{
	if (ptr != NULL)
	{
		free(ptr->data);
		ptr->data = NULL;
		ptr->c = ptr->n = 0;
	}
}

void mainStringAppend(struct main_string *ptr, const char *data)
{
	mainStringNappend(ptr, data, strlen(data));
}

void mainStringNappend(struct main_string *ptr, const char *data, int n)
{
	mainStringCheckCapacity(ptr, ptr->n + n + 1);
	while (n-- > 0 && *data != E_CHAR)
		ptr->data[ptr->n++] = *data++;
	ptr->data[ptr->n] = E_CHAR;
}

void mainStringReset(struct main_string *ptr)
{
	ptr->n = 0;
	ptr->data[ptr->n] = E_CHAR;
}

void mainStringAssign(struct main_string *ptr, const char *data)
{
	mainStringReset(ptr);
	mainStringAppend(ptr, data);
}

void mainStringNAssign(struct main_string *ptr, const char *data, int n)
{
	mainStringReset(ptr);
	mainStringNappend(ptr, data, n);
}

char *mainStringNewString(struct main_string *ptr)
{
	char *p;

	p = (char*) malloc(sizeof(char) * (ptr->n + 1));
	strcpy(p, ptr->data);
	return p;
}

char *mainStringCloneString(const char *str)
{
	char *dst;
	int n;

	n = strlen(str);
	dst = (char*) malloc(sizeof(char) * (n + 1));
	memcpy(dst, str, n + 1);

	return dst;
}

main_string *mainStringNew()
{
	main_string *ptr = (main_string*) malloc(sizeof(main_string));
	mainStringInitial(ptr);
	return ptr;
}

void mainStringDelete(main_string *ptr)
{
	mainStringRelease(ptr);
	free(ptr);
}