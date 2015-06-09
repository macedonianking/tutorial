/*
 * main_resource_generator.c
 *
 *  Created on: 2015年6月9日
 *      Author: lhb
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_options.h"
#include "resource_generator.h"
#include "file_util.h"

static int main_resource_generator_handle_item(struct main_resource_table *table,
		struct main_resource_table *kTable, struct main_options *options, int i);

int main_resource_generator_execute(struct main_options *options)
{
	struct main_resource_table table;
	struct main_resource_table rTable;
	FILE *iFile;
	int r;

	if (!main_file_is_exists(options->gen_dir.data)
			&& !main_file_create_directory(options->gen_dir.data))
	{
		fprintf(stdout, "create directory: %s failed\n", options->gen_dir.data);
		return -1;
	}

	if ((iFile = fopen(options->r_file.data, "r")) == NULL)
	{
		fprintf(stdout, "open file failure:%s\n", options->r_file.data);
		return -1;
	}

	main_resource_table_initial(&table, -1);
	main_resource_table_initial(&rTable, -1);
	r = main_resource_table_init_from_file(&table, iFile);
	fclose(iFile);
	iFile = NULL;

	if (r == 0)
	{
		for (int i = 0; i < options->n; ++i)
		{
			if (main_resource_generator_handle_item(&table, &rTable, options, i) != 0)
			{
				r = -1;
				break;
			}
		}
	}
	else
	{
		r = -1;
	}
	main_resource_table_release(&rTable);
	main_resource_table_release(&table);
	return r;
}

int main_resource_generator_handle_item(struct main_resource_table *table,
		struct main_resource_table *kTable, struct main_options *options, int i)
{
	main_resource_table_reset(kTable);

	return 0;
}
