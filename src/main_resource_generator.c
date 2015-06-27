/*
 * main_resource_generator.c
 *
 *  Created on: 2015年6月9日
 *      Author: lhb
 */

#include <stdio.h>

#include "main_string.h"
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
	main_resource_print_options print_options;
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

	file = NULL;
	if (r == 0)
	{
		main_string_initial(&path);
		main_options_get_r_file_path(&path, options, i);
		r = main_file_check_create_parent_dir(path.data);
		if (r == 0)
		{
			file = fopen(path.data, "w");
		}
		main_string_release(&path);
	}

	if (file == NULL)
	{
		r = -1;
	}
	else
	{
		print_options.is_non_constant_id = options->is_non_constant_id;
		print_options.package_name = (const char*) options->v_packages.head[i];
		print_options.step = 0;
		main_resource_table_print_java(&dstTable, file, &print_options);
		fclose(file);
		file = NULL;
	}

	main_resource_table_release(&dstTable);
	return r;
}
