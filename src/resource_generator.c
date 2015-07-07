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
#include "print_util.h"
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

void main_resource_item_print_java(struct main_resource_item *ptr, FILE *file,
		struct main_resource_print_options *options)
{
	main_fprint_nchar(file, options->step, '\t');
	if (options->is_non_constant_id)
		fprintf(file, "public static %s %s = %s;\n", gResourceTypeNames[ptr->type], ptr->name,
				ptr->value);
	else
		fprintf(file, "public static final %s %s = %s;\n", gResourceTypeNames[ptr->type], ptr->name,
				ptr->value);
}

void main_resource_item_print(struct main_resource_item *ptr, FILE *file)
{
	const char *text;

	text = gResourceTypeNames[ptr->type];
	fwrite(text, 1, strlen(text), file);
	fputc(' ', file);
	fwrite(ptr->category, 1, strlen(ptr->category), file);
	fputc(' ', file);
	fwrite(ptr->name, 1, strlen(ptr->name), file);
	fputc(' ', file);
	fwrite(ptr->value, 1, strlen(ptr->value), file);
}

void main_resource_item_set_value(main_resource_item *ptr, const char *value)
{
	if (ptr->value)
	{
		free(ptr->value);
	}
	ptr->value = main_string_new_string(value);
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
	for (int i = 0; i < ptr->n; ++i)
	{
		main_resource_item_release(ptr->head + i);
	}
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

/**
 * 删除一个子成员
 */
void main_resource_category_erase(main_resource_category *ptr, int i)
{
	main_resource_item *src, *dst, *end;

	src = ptr->head + i;
	dst = src + 1;
	end = ptr->head + ptr->n;
	main_resource_item_release(&ptr->head[i]);
	while (dst < end)
		*src++ = *dst++;
	--ptr->n;
}

void main_resource_category_print(struct main_resource_category *ptr, FILE *file)
{
	for (int i = 0; i < ptr->n; ++i)
	{
		main_resource_item_print(&ptr->head[i], file);
		fputc('\n', file);
		fflush(file);
	}
}

void main_resource_category_print_java(struct main_resource_category *ptr, FILE *file,
		struct main_resource_print_options *options)
{
	main_fprint_nchar(file, options->step, '\t');
	fprintf(file, "public static final class %s {\n", ptr->name);
	++options->step;

	for (int i = 0; i < ptr->n; ++i)
		main_resource_item_print_java(ptr->head + i, file, options);

	--options->step;
	main_fprint_nchar(file, options->step, '\t');
	fprintf(file, "}\n");
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

void main_resource_table_reset(main_resource_table *table)
{
	for (int i = 0; i < table->n; ++i)
	{
		main_resource_category_release(table->head + i);
	}
	table->n = 0;
}

void main_resource_table_print(struct main_resource_table *table, FILE *file)
{
	for (int i = 0; i < table->n; ++i)
		main_resource_category_print(&table->head[i], file);
}

void main_resource_table_print_java(main_resource_table *table, FILE *file,
		struct main_resource_print_options *options)
{
	fprintf(file, "/* AUTO-GENERATED FILE.  DO NOT MODIFY.\n"
			" *\n"
			" * This class was automatically generated by the\n"
			" * aapt tool from the resource data it found.  It\n"
			" * should not be modified by hand.\n"
			" */\n");

	fprintf(file, "package %s;\n", options->package_name);
	fprintf(file, "\n");
	fprintf(file, "public final class R {\n");

	++options->step;
	for (int i = 0; i < table->n; ++i)
		main_resource_category_print_java(table->head + i, file, options);
	--options->step;

	fprintf(file, "\n}");
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
	int nLine;

	r = 0;
	category = NULL;
	main_string_initial(&typeName);
	main_string_initial(&itemName);
	main_string_initial(&itemValue);
	main_line_split_initial(&split, file, MAIN_PRINT_BUFFER_SIZE);
	nLine = 0;
	while ((line = main_line_split_next_line(&split, &n)) != NULL)
	{
		if (main_resource_parse_item(line, &dataType, &typeName, &itemName, &itemValue) != 0)
		{
			printf("parse line failed:%d:%s\n", nLine, line);
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
		++nLine;
	}
	main_line_split_release(&split);
	main_string_release(&typeName);
	main_string_release(&itemName);
	main_string_release(&itemValue);

	return r;
}

static main_resource_item *main_resource_category_search(main_resource_category *cat,
		const char *item_name, int item_type)
{
	main_resource_item *ptr;
	int n;

	ptr = cat->head;
	n = cat->n;
	while (n-- > 0)
	{
		if (ptr->type == item_type && strcmp(ptr->name, item_name) == 0)
		{
			return ptr;
		}
		++ptr;
	}

	return NULL;
}

static int main_resource_category_replace(main_resource_category *cat, main_resource_table *src,
		main_options *options, int position)
{
	main_resource_category *dst_cat;
	main_resource_item *ptr, *dst_item;
	char *src_file;
	int i;

	src_file = (char*) options->v_resource_r_files.head[position];
	dst_cat = main_resource_table_search(src, cat->name);
	if (dst_cat == NULL)
	{
		return -1;
	}

	i = 0;
	while (i < cat->n)
	{
		ptr = cat->head + i;
		dst_item = main_resource_category_search(dst_cat, ptr->name, ptr->type);
		if (dst_item == NULL)
		{
			fprintf(stdout, "[WARNING]: Can't find resource id '%s' in '%s' for file '%s'\n",
					ptr->name, options->r_file.data, src_file);
			fflush(stdout);
			main_resource_category_erase(cat, i);
		}
		else
		{
			main_resource_item_set_value(ptr, dst_item->value);
			++i;
		}
	}

	return 0;
}
/**
 * 内容替换
 */
int main_resource_table_replace(main_resource_table *dst, main_resource_table *src,
		main_options *options, int position)
{
	for (int i = 0; i < dst->n; ++i)
	{
		if (main_resource_category_replace(dst->head + i, src, options, position) != 0)
		{
			return -1;
		}
	}

	return 0;
}
