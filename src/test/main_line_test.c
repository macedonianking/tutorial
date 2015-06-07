/*
 * main_line_test.c
 *
 *  Created on: 2015年6月6日
 *      Author: lhb
 */

#include <stdio.h>
#include <stdlib.h>

#include "../config.h"
#include "main_line_test.h"

#include "../main_string.h"
#include "../tutorial_max_line.h"
#include "../resource_generator.h"

#define FILE_NAME_A_TXT	"D:\\workspace_cpp\\Tutorial\\a.txt"

void main_line_test()
{
	struct main_line_split split;
	FILE *file;
	const char *ptr;
	int n;

	if ((file = fopen(FILE_NAME_A_TXT, "r")) == NULL)
	{
		return;
	}

	main_line_split_initial(&split, file, 4096);
	while ((ptr = main_line_split_next_line(&split, &n)) != NULL)
		printf("%s\n", ptr);
	main_line_split_release(&split);

	fclose(file);
	file = NULL;
}

void main_resource_table_test()
{
	struct main_resource_table table;
	struct main_resource_print_options options;

	FILE *file;
	FILE *outFile;
	int r;

	file = NULL;
	outFile = NULL;
	if ((file = fopen(FILE_NAME_A_TXT, "r")) == NULL)
		return;

	if ((outFile = fopen("D:\\workspace_cpp\\Tutorial\\R.java", "w")) == NULL)
	{
		fclose(file);
		return;
	}

	options.is_non_constant_id = 0;
	options.package_name = "com.android.browser";
	options.step = 0;

	main_resource_table_initial(&table, -1);
	r = main_resource_table_init_from_file(&table, file);
	if (r == 0)
		main_resource_table_print_java(&table, outFile, &options);
	main_resource_table_release(&table);

	fclose(file);
	fclose(outFile);
	file = NULL;
	outFile = NULL;

}
