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

struct main_string;

struct main_resource_item
{
	int type;
	char *category;
	char *name;
	char *value;
};

struct main_resource_category
{
	struct main_resource_item *head;
	char *name;
	int n;
	int c;
};

struct main_resource_table
{
	struct main_resource_category *head;
	int n;
	int c;
};

struct main_resource_print_options
{
	int is_non_constant_id;
	const char *package_name;
	int step;
};

void main_resource_item_initial(struct main_resource_item *ptr);
void main_resource_item_release(struct main_resource_item *ptr);
void main_resource_item_set_name_value(struct main_resource_item *ptr, struct main_string *name,
		struct main_string *value);
void main_resource_item_print_java(struct main_resource_item *ptr, FILE *file,
		struct main_resource_print_options *options);
void main_resource_item_print(struct main_resource_item *ptr, FILE *file);

void main_resource_category_initial(struct main_resource_category *ptr, int c);
void main_resource_category_release(struct main_resource_category *ptr);
struct main_resource_item *main_resource_category_append(struct main_resource_category *ptr);
void main_resource_category_print(struct main_resource_category *ptr, FILE *file);
void main_resource_category_print_java(struct main_resource_category *ptr, FILE *file,
		struct main_resource_print_options *options);

void main_resource_table_initial(struct main_resource_table *table, int c);
void main_resource_table_release(struct main_resource_table *table);
struct main_resource_category *main_resource_table_append(struct main_resource_table *table);
struct main_resource_category *main_resource_table_search(struct main_resource_table *table,
		const char *name);
void main_resource_table_print(struct main_resource_table *table, FILE *file);
void main_resource_table_print_java(struct main_resource_table *table, FILE *file,
		struct main_resource_print_options *options);

int main_resource_table_init_from_file(struct main_resource_table *table, FILE *file);
#endif // MAIN_RESOURCE_GENERATOR_H
