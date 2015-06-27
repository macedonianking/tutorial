/*
 * main_resource_generator.c
 *
 *  Created on: 2015年6月9日
 *      Author: lhb
 */

#include <stdio.h>

#include "main_file.h"
#include "main_options.h"
#include "resource_generator.h"

static int main_resource_generator_handle_item(main_resource_table *table, main_options *options,
		int i);

int main_resource_generator_execute(main_options *options)
{
	main_resource_table table;
	FILE *iFile;
	int r;

	if (main_file_check_create_parent_dir(options->gen_dir.data) != 0)
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
	r = main_resource_table_init_from_file(&table, iFile);
	fclose(iFile);
	iFile = NULL;

	if (r == 0)
	{
		for (int i = 0; i < options->v_packages.n; ++i)
		{
			if (main_resource_generator_handle_item(&table, options, i) != 0)
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
	main_resource_table_release(&table);
	return r;
}

int main_resource_generator_handle_item(main_resource_table *table, main_options *options, int i)
{
	int r;
	main_resource_table dstTable;
	main_string path;
	FILE *file;

	r = 0;
	if ((file = fopen((char*) options->v_resource_r_files.head[i], "r")) == NULL)
	{
		return r;
	}
	main_resource_table_initial(&dstTable, -1);
	r = main_resource_table_init_from_file(&dstTable, file);
	fclose(file);
	if (r == 0)
	{
		r = main_resource_table_replace(&dstTable, table);
	}
	if (r == 0)
	{
	}

	main_resource_table_release(&dstTable);
	return r;
}
