/*
 * main_options.c
 *
 *  Created on: 2015年6月7日
 *      Author: lhb
 */

#include "main_options.h"

#include <stdio.h>
#include <string.h>

#include "config.h"
#include "main_file.h"
#include "main_resource_generator.h"

#define APP_NAME	"Tutorial"

void main_options_initial(struct main_options *options)
{
	options->is_non_constant_id = 0;
	main_vector_initial(&options->v_packages);
	main_vector_initial(&options->v_resource_r_files);
	main_string_initial(&options->gen_dir);
	main_string_initial(&options->r_file);
}

void main_options_release(struct main_options *options)
{
	main_vector_release(&options->v_packages);
	main_vector_release(&options->v_resource_r_files);
	main_string_release(&options->gen_dir);
	main_string_release(&options->r_file);
}

void main_options_call_help()
{
	fprintf(stdout, "aaptr Android Package Resource Assist Tool\n\n");
	fprintf(stdout, "%s -J gen_dir --R-file rfile -P packages... -R R-files\n"
			"    --resource-r-files rfiles\n",
	APP_NAME);
	fprintf(stdout, "    --R-file rfile [--non-constant-id]\n");
	fprintf(stdout, "    -J generated directory\n");
	fprintf(stdout, "    -P package names\n");
	fprintf(stdout, "    -R resource R.txt files\n");
	fprintf(stdout, "    --extra-packages: ':' separated\n");
	fprintf(stdout, "    --resource-r-files: ':' separacted\n");
	fprintf(stdout, "    --non-constant-id: generate non constant R.java file\n");
}

int main_options_initialize_from_arguments(struct main_options *options, int argc, char **argv)
{
	int i;
	int r;

	i = 1;
	r = 0;
	while (i < argc)
	{
		if (strcmp("-J", argv[i]) == 0)
		{
			++i;
			if (i < argc)
			{
				main_string_assign(&options->gen_dir, argv[i]);
				++i;
			}
			else
			{
				r = -1;
			}
		}
		else if (strcmp("--R-file", argv[i]) == 0)
		{
			++i;
			if (i < argc)
			{
				main_string_assign(&options->r_file, argv[i]);
				++i;
			}
			else
			{
				r = -1;
			}
		}
		else if (strcmp("--non-constant-id", argv[i]) == 0)
		{
			options->is_non_constant_id = 1;
			++i;
		}
		else if (strcmp("-P", argv[i]) == 0)
		{
			++i;
			while (i < argc && *argv[i] != '-')
			{
				main_vector_vappend(&options->v_packages, argv[i], NULL);
				++i;
			}
		}
		else if (strcmp("-S", argv[i]) == 0)
		{
			++i;
			while (i < argc && *argv[i] != '-')
			{
				main_vector_vappend(&options->v_resource_r_files, argv[i],
				NULL);
			}
		}
		else
		{
			fprintf(stdout, "Unknown arguments:'%s'\n", argv[i]);
			r = -1;
		}

		if (r != 0)
		{
			break;
		}
	}

	if (r == 0 && options->v_resource_r_files.n != options->v_packages.n)
	{
		r = -1;
	}

	return r;
}

int main_options_check_statements(struct main_options *options)
{
	if (options->gen_dir.n <= 0)
	{
		return -1;
	}

	if (options->r_file.n <= 0)
	{
		return -1;
	}
	if (main_file_is_file(options->r_file.data) != 0)
	{
		return -1;
	}

	if (options->v_packages.n <= 0 || options->v_packages.n != options->v_resource_r_files.n)
	{
		return -1;
	}

	for (int i = 0; i < options->v_resource_r_files.n; ++i)
	{
		/**
		 * 检查文件都存在
		 */
		if (main_file_is_file((char*) options->v_resource_r_files.head[i]) != 0)
		{
			return -1;
		}
	}
	return 0;
}

/**
 * 获取文件的路径
 */
int main_options_get_r_file_path(main_string *dst, main_options *options, int i)
{
	char buffer[MAIN_FILE_MAX_PATH];
	char *ptr;

	main_string_reset(dst);
	main_string_append(dst, options->gen_dir);
	main_string_append(dst, MAIN_FILE_SEP_WINS_STRING);
	ptr = (char*) options->v_packages.head[i];
	strcpy(buffer, ptr);

	ptr = buffer;
	while (*ptr != E_CHAR)
	{
		if (*ptr == '.')
		{
			*ptr = MAIN_FILE_SEP_WINS_CHAR;
		}
		++ptr;
	}
	main_string_append(dst, buffer);
	main_string_append(dst, MAIN_FILE_SEP_WINS_STRING);
	main_string_append(dst, "R.java");

	return 0;
}

int do_execute(int argc, char **argv)
{
	int r;
	main_options options;

	r = 0;
	main_options_initial(&options);
	r = main_options_initialize_from_arguments(&options, argc, argv);
	if (r == 0)
	{
		r = main_options_check_statements(&options);
	}
	else
	{
		main_options_call_help();
	}

	if (r == 0)
	{
		r = main_resource_generator_execute(&options);
	}

	main_options_release(&options);

	return r;
}
