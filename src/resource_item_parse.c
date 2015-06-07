/*
 * resource_item_parse.c
 *
 *  Created on: 2015年6月6日
 *      Author: lhb
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "main_string.h"
#include "resource_generator.h"

#define MAIN_STRING_SEEK_WORD	0x0001
#define MAIN_STRING_SEEK_ENDL	0x0002
#define MAIN_STRING_SEEK_NONE	0x0003

static const char *main_resource_string_seek(const char *str, int seek);
static const char *main_resource_string_seek_range(const char *str, const char **start, int seekFm,
		int seekTo);

int main_resource_parse_item(const char *str, int *dataType, struct main_string *typeName,
		struct main_string *itemName, struct main_string *itemValue)
{
	const char *s;

	str = main_resource_string_seek_range(str, &s, MAIN_STRING_SEEK_WORD, MAIN_STRING_SEEK_NONE);
	if (str == NULL)
		return -1;

	if (strncmp("int", s, str - s) == 0)
		*dataType = RESOURCE_ITEM_TYPE_INT;
	else if (strncmp("int[]", s, str - s) == 0)
		*dataType = RESOURCE_ITEM_TYPE_INT_ARRAY;
	else
		return -1;

	/**
	 * 解析类型名字
	 */
	str = main_resource_string_seek_range(str, &s, MAIN_STRING_SEEK_WORD, MAIN_STRING_SEEK_NONE);
	if (str == NULL)
		return -1;
	main_string_nassign(typeName, s, str - s);

	/**
	 * 解析资源名字
	 */
	str = main_resource_string_seek_range(str, &s, MAIN_STRING_SEEK_WORD, MAIN_STRING_SEEK_NONE);
	if (str == NULL)
		return -1;
	main_string_nassign(itemName, s, str - s);

	/**
	 * 解析资源名称
	 */
	str = main_resource_string_seek_range(str, &s, MAIN_STRING_SEEK_WORD, MAIN_STRING_SEEK_ENDL);
	if (str == NULL)
		return -1;
	main_string_nassign(itemValue, s, str - s);
	return 0;
}

const char *main_resource_string_seek(const char *str, int seek)
{
	if (seek == MAIN_STRING_SEEK_WORD)
	{
		while (*str != E_CHAR && (*str == '\n' || *str == ' ' || *str == '\t'))
			++str;
	}
	else if (seek == MAIN_STRING_SEEK_ENDL)
	{
		while (*str != E_CHAR && *str != '\n')
			++str;
	}
	else
	{
		while (*str != E_CHAR && *str != '\n' && *str != ' ' && *str != '\t')
			++str;
	}
	return str;
}

const char *main_resource_string_seek_range(const char *str, const char **start, int seekFm,
		int seekTo)
{
	str = main_resource_string_seek(str, seekFm);
	if (*str == E_CHAR)
		return NULL;

	*start = str;
	return main_resource_string_seek(str, seekTo);
}
