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
	FILE *file;
	int r;

	if ((file = fopen(FILE_NAME_A_TXT, "r")) == NULL)
		return;

	main_resource_table_initial(&table, -1);
	r = main_resource_table_init_from_file(&table, file);
	printf("main_resource_table_test r=%d\n", r);
	main_resource_table_release(&table);

	fclose(file);
	file = NULL;

}
