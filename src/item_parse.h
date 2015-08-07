/*
 * resource_item_parse.h
 *
 *  Created on: 2015年6月6日
 *      Author: lhb
 */

#ifndef MAIN_RESOURCE_ITEM_PARSE_H
#define MAIN_RESOURCE_ITEM_PARSE_H

struct main_string;

int mainResourceParseItem(const char *str, int *dataType, struct main_string *typeName,
		struct main_string *itemName, struct main_string *itemValue);

#endif // MAIN_RESOURCE_ITEM_PARSE_H
