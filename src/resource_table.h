/*
 * reource_generator.h
 *
 *  Created on: 2015年6月4日
 *      Author: lhb
 */

#ifndef MAIN_RESOURCE_GENERATOR_H
#define MAIN_RESOURCE_GENERATOR_H



#include "main_string.h"
#include "main_options.h"
#include "resource_item.h"
#include "resource_category.h"

typedef struct main_resource_table {
	struct main_resource_category *head;
	char *file_name;
	int n;
	int c;
} main_resource_table;

typedef struct main_resource_print_options {
	int is_non_constant_id;
	const char *package_name;
	int step;
} main_resource_print_options;


main_resource_table *mainResourceTableNew();
void mainResourceTableDelete(main_resource_table *ptr);

void mainResourceTableInitial(main_resource_table *table, int c);
void mainResourceTableRelease(main_resource_table *table);
main_resource_category *mainResourceTableAppend(main_resource_table *table);
main_resource_category *mainResourceTableSearch(main_resource_table *table, const char *name);
void mainResourceTableReset(main_resource_table *table);
void mainResourceTablePrint(main_resource_table *table, FILE *file);
void mainResourceTablePrintJava(main_resource_table *table, FILE *file,
		main_resource_print_options *options);

void mainResourceTableSetFileName(main_resource_table *table, const char *fileName);
int  mainResourceTableInitFromFile(main_resource_table *table, FILE *file);

/**
 * 从文件和查询表构建正式的表
 */
int main_resource_table_init_from_file_and_source(main_resource_table *table,
		main_resource_table *src_table, FILE *file);

/**
 * 内容替换
 */
int mainResourceTableReplace(main_resource_table *dst, main_resource_table *src);
#endif // MAIN_RESOURCE_GENERATOR_H
