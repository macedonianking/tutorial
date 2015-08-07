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
#include "resource_table.h"

#define MAIN_STRING_SEEK_WORD	0x0001
#define MAIN_STRING_SEEK_ENDL	0x0002
#define MAIN_STRING_SEEK_NONE	0x0003

static const char *resourceStringSeek(const char *str, int seek);
static const char *resourceStringSeekRange(const char *str, const char **start, int seekFm, int seekTo);

int mainResourceParseItem(const char *str, int *dataType, main_string *typeName,
		main_string *itemName, main_string *itemValue)
{
	const char *s;

	str = resourceStringSeekRange(str, &s, MAIN_STRING_SEEK_WORD,
	MAIN_STRING_SEEK_NONE);
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
	str = resourceStringSeekRange(str, &s, MAIN_STRING_SEEK_WORD,
	MAIN_STRING_SEEK_NONE);
	if (str == NULL)
		return -1;
	mainStringNAssign(typeName, s, str - s);

	/**
	 * 解析资源名字
	 */
	str = resourceStringSeekRange(str, &s, MAIN_STRING_SEEK_WORD,
	MAIN_STRING_SEEK_NONE);
	if (str == NULL)
		return -1;
	mainStringNAssign(itemName, s, str - s);

	/**
	 * 解析资源名称
	 */
	str = resourceStringSeekRange(str, &s, MAIN_STRING_SEEK_WORD,
	MAIN_STRING_SEEK_ENDL);
	if (str == NULL)
		return -1;
	mainStringNAssign(itemValue, s, str - s);
	return 0;
}

const char *resourceStringSeek(const char *str, int seek)
{
	if (seek == MAIN_STRING_SEEK_WORD) {
		while (*str != E_CHAR && (*str == '\n' || *str == ' ' || *str == '\t'))
			++str;
	} else if (seek == MAIN_STRING_SEEK_ENDL) {
		while (*str != E_CHAR && *str != '\n')
			++str;
	} else {
		while (*str != E_CHAR && *str != '\n' && *str != ' ' && *str != '\t')
			++str;
	}
	return str;
}

const char *resourceStringSeekRange(const char *str, const char **start, int seekFm,
		int seekTo)
{
	str = resourceStringSeek(str, seekFm);
	if (*str == E_CHAR)
		return NULL;

	*start = str;
	return resourceStringSeek(str, seekTo);
}
