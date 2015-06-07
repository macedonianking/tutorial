/*
 * resource_generator.c
 *
 *  Created on: 2015年6月4日
 *      Author: lhb
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "main_string.h"
#include "tutorial_max_line.h"
#include "resource_generator.h"
#include "resource_item_parse.h"

#define MAIN_PRINT_BUFFER_SIZE	2048

static const char *gResourceTypeNames[] = { "int", "int[]" };

void main_resource_item_initial(struct main_resource_item *ptr)
{
	ptr->category = ptr->name = ptr->value = NULL;
	ptr->type = RESOURCE_ITEM_TYPE_NONE;
}

void main_resource_item_release(struct main_resource_item *ptr)
{
	if (ptr->name)
	{
		free(ptr->name);
		ptr->name = NULL;
	}

	if (ptr->value)
	{
		free(ptr->value);
		ptr->value = NULL;
	}

	ptr->category = NULL;
	ptr->type = RESOURCE_ITEM_TYPE_NONE;
}

void main_resource_item_set_name_value(struct main_resource_item *ptr, struct main_string *name,
		struct main_string *value)
{
	if (ptr->name)
	{
		free(ptr->name);
		ptr->name = NULL;
	}
	ptr->name = main_string_new(name);

	if (ptr->value)
	{
		free(ptr->value);
		ptr->value = NULL;
	}
	ptr->value = main_string_new(value);
}

void main_resource_item_print(struct main_resource_item *ptr, FILE *file,
		const struct main_resource_print_options *options)
{
	char buffer[MAIN_PRINT_BUFFER_SIZE];
	int n;

	if (options->is_non_constant_id)
		n = snprintf(buffer, MAIN_PRINT_BUFFER_SIZE, "public static %s %s = %s;",
				gResourceTypeNames[ptr->type], ptr->name, ptr->value);
	else
		n = snprintf(buffer, MAIN_PRINT_BUFFER_SIZE, "public static final %s %s = %s;",
				gResourceTypeNames[ptr->type], ptr->name, ptr->value);
	fwrite(buffer, 1, n, file);
}

void main_resource_category_initial(struct main_resource_category *ptr, int c)
{
	if (c <= 0)
		c = 512;
	ptr->c = c;
	ptr->n = 0;
	ptr->head = (struct main_resource_item*) malloc(sizeof(struct main_resource_item) * ptr->c);
}

void main_resource_category_release(struct main_resource_category *ptr)
{
	if (!ptr)
		return;

	for (int i = 0; i < ptr->n; ++i)
		main_resource_item_release(ptr->head + i);
	free(ptr->head);
	ptr->head = NULL;
	ptr->n = ptr->c = 0;
	free(ptr->name);
}

struct main_resource_item *main_resource_category_append(struct main_resource_category *ptr)
{
	if (ptr->n == ptr->c)
	{
		ptr->c *= 2;
		ptr->head = (struct main_resource_item*) realloc(ptr->head,
				ptr->c * sizeof(struct main_resource_item));
	}
	return &ptr->head[ptr->n++];
}

void main_resource_table_initial(struct main_resource_table *table, int c)
{
	if (c <= 0)
		c = 20;
	table->c = c;
	table->n = 0;
	table->head = (struct main_resource_category*) malloc(
			table->c * sizeof(struct main_resource_category));
}

void main_resource_table_release(struct main_resource_table *table)
{
	for (int i = 0; i < table->n; ++i)
		main_resource_category_release(table->head + i);
	free(table->head);
	table->c = table->n = 0;
}

struct main_resource_category *main_resource_table_append(struct main_resource_table *table)
{
	if (table->n >= table->c)
	{
		table->c *= 2;
		table->head = (struct main_resource_category*) realloc(table->head,
				table->c * sizeof(struct main_resource_category));
	}
	return &table->head[table->n++];
}

struct main_resource_category *main_resource_table_search(struct main_resource_table *table,
		const char *name)
{
	struct main_resource_category *r;

	r = NULL;
	for (int i = 0; i < table->n; ++i)
	{
		if (strcmp(name, table->head[i].name) == 0)
		{
			r = table->head + i;
			break;
		}
	}
	return r;
}

int main_resource_table_init_from_file(struct main_resource_table *table, FILE *file)
{
	struct main_line_split split;
	const char *line;
	int n;
	int dataType;
	int r;
	struct main_string typeName;
	struct main_string itemName;
	struct main_string itemValue;
	struct main_resource_category *category;
	struct main_resource_item *item;

	r = 0;
	category = NULL;
	main_string_initial(&typeName, MAIN_STRING_DEFAULT_CAPACITY);
	main_string_initial(&itemName, MAIN_STRING_DEFAULT_CAPACITY);
	main_string_initial(&itemValue, MAIN_STRING_DEFAULT_CAPACITY);
	main_line_split_initial(&split, file, MAIN_PRINT_BUFFER_SIZE);
	while ((line = main_line_split_next_line(&split, &n)) != NULL)
	{
		if (main_resource_parse_item(line, &dataType, &typeName, &itemName, &itemValue) != 0)
		{
			r = 1;
			break;
		}

		if (category == NULL || strcmp(category->name, typeName.data) != 0)
		{
			category = main_resource_table_search(table, typeName.data);
			if (category == NULL)
			{
				category = main_resource_table_append(table);
				main_resource_category_initial(category, 1024);
				category->name = main_string_new(&typeName);
			}
		}

		/**
		 * 添加成员 
		 */
		item = main_resource_category_append(category);
		main_resource_item_initial(item);
		item->type = dataType;
		item->category = category->name;
		main_resource_item_set_name_value(item, &itemName, &itemValue);
	}
	main_line_split_release(&split);
	main_string_release(&typeName);
	main_string_release(&itemName);
	main_string_release(&itemValue);

	return r;
}

