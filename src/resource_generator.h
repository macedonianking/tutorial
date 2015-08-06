/*
 * reource_generator.h
 *
 *  Created on: 2015年6月4日
 *      Author: lhb
 */

#ifndef MAIN_RESOURCE_GENERATOR_H
#define MAIN_RESOURCE_GENERATOR_H

#define RESOURCE_ITEM_TYPE_INT			0x0000
#define RESOURCE_ITEM_TYPE_INT_ARRAY	0x0001
#define RESOURCE_ITEM_TYPE_NONE			0x0002

#include "main_string.h"
#include "main_options.h"

typedef struct main_resource_item
{
	int type;
	char *category;
	char *name;
	char *value;
} main_resource_item;

typedef struct main_resource_category
{
	struct main_resource_item *head;
	char *name;
	volatile int n;
	int c;
} main_resource_category;

typedef struct main_resource_table
{
	struct main_resource_category *head;
	int n;
	int c;
} main_resource_table;

typedef struct main_resource_print_options
{
	int is_non_constant_id;
	const char *package_name;
	int step;
} main_resource_print_options;

void main_resource_item_initial(main_resource_item *ptr);
void main_resource_item_release(main_resource_item *ptr);
void main_resource_item_set_name_value(main_resource_item *ptr, main_string *name,
		main_string *value);
void main_resource_item_print_java(main_resource_item *ptr, FILE *file,
		main_resource_print_options *options);
void main_resource_item_print(main_resource_item *ptr, FILE *file);
void main_resource_item_set_value(main_resource_item *ptr, const char *value);

void main_resource_category_initial(main_resource_category *ptr, int c);
void main_resource_category_release(main_resource_category *ptr);
main_resource_item *main_resource_category_append(main_resource_category *ptr);
void main_resource_category_erase(main_resource_category *ptr, int i);
void main_resource_category_print(main_resource_category *ptr, FILE *file);
void main_resource_category_print_java(main_resource_category *ptr, FILE *file,
		main_resource_print_options *options);

void main_resource_table_initial(main_resource_table *table, int c);
void main_resource_table_release(main_resource_table *table);
main_resource_category *main_resource_table_append(main_resource_table *table);
main_resource_category *main_resource_table_search(main_resource_table *table, const char *name);
void main_resource_table_reset(main_resource_table *table);
void main_resource_table_print(main_resource_table *table, FILE *file);
void main_resource_table_print_java(main_resource_table *table, FILE *file,
		main_resource_print_options *options);

int main_resource_table_init_from_file(main_resource_table *table, FILE *file);
/**
 * 从文件和查询表构建正式的表
 */
int main_resource_table_init_from_file_and_source(main_resource_table *table,
		main_resource_table *src_table, FILE *file);

/**
 * 内容替换
 */
int main_resource_table_replace(main_resource_table *dst, main_resource_table *src,
		main_options *options, int i);
#endif // MAIN_RESOURCE_GENERATOR_H
