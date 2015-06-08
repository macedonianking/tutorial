/*
 * main_options.c
 *
 *  Created on: 2015年6月7日
 *      Author: lhb
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main_options.h"
#include "config.h"
#include "main_vector.h"
#include "file_util.h"

#define APP_NAME	"Tutorial"

#define CHECK_STATUS_NONE	0x0000
#define CHECK_STATUS_RUNN	0x0001

static int main_options_check_package_and_resource_files(struct main_options *options,
		char *extra_packages, char *r_resource_files);

void main_options_initial(struct main_options *options)
{
	options->n = 0;
	options->is_non_constant_id = 0;
	options->packages = options->resource_r_files = NULL;
	main_string_initial(&options->gen_dir, -1);
	main_string_initial(&options->r_file, -1);
}

void main_options_release(struct main_options *options)
{
	for (int i = 0; i < options->n; ++i)
	{
		main_string_release(options->packages + i);
		main_string_release(options->resource_r_files + i);
	}
	options->n = 0;
	free(options->packages);
	free(options->resource_r_files);
	main_string_release(&options->gen_dir);
	main_string_release(&options->r_file);
}

void main_options_prepare_size(struct main_options *options, int n)
{
	DCHECK(!options->n && !options->packages && !options->resource_r_files);
	DCHECK(n > 0);
	options->n = n;
	options->packages = (struct main_string*) malloc(sizeof(struct main_string) * n);
	options->resource_r_files = (struct main_string*) malloc(sizeof(struct main_string) * n);
	for (int i = 0; i != n; ++i)
	{
		main_string_initial(options->packages + i, -1);
		main_string_initial(options->resource_r_files + i, -1);
	}
}

void main_options_call_help(FILE *file)
{
	fprintf(file, "%s -J gen_dir --extra-packages packages --resource-r-files rfiles\n", APP_NAME);
	fprintf(file, "\t--R-file rfile [--non-constant-id]\n\n");
	fprintf(file, "\t-J: generated directory\n");
	fprintf(file, "\t--extra-packages: \":\" separated\n");
	fprintf(file, "\t--resource-r-files: \":\" separacted\n");
	fprintf(file, "\t--non-constant-id: generate non constant R.java file\n");
}

int main_options_initialize_from_arguments(struct main_options *options, int argc, char **argv)
{
	int i;
	int r;
	struct main_string extra_packages;
	struct main_string r_resource_files;

	i = 0;
	r = 0;
	main_string_initial(&extra_packages, -1);
	main_string_initial(&r_resource_files, -1);
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
				fprintf(stdout, "-J paramater not set\n");
				r = -1;
				break;
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
				fprintf(stdout, "--R-file not specified\n");
				r = -1;
				break;
			}
		}
		else if (strcmp("--extra-packages", argv[i]) == 0)
		{
			++i;
			if (i < argc)
			{
				main_string_assign(&extra_packages, argv[i]);
				++i;
			}
			else
			{
				fprintf(stdout, "--extra-packages not specified\n");
				r = -1;
				break;
			}
		}
		else if (strcmp("--r-resource-files", argv[i]) == 0)
		{
			++i;
			if (i < argc)
			{
				main_string_assign(&r_resource_files, argv[i]);
				++i;
			}
			else
			{
				fprintf(stdout, "--r-resource-files not specified\n");
				r = -1;
				break;
			}
		}
		else if (strcmp("--non-constant-id", argv[i]) == 0)
		{
			options->is_non_constant_id = 1;
			++i;
		}
		else
		{
			fprintf(stdout, "Unknown arguments:'%s'\n", argv[i]);
			r = -1;
			break;
		}
	}

	if (r == 0)
		r = main_options_check_package_and_resource_files(options, extra_packages.data,
				r_resource_files.data);

	main_string_release(&extra_packages);
	main_string_release(&r_resource_files);
	return r;
}

static int main_options_split_item_from_clone(char *params, struct main_vector *start_vector,
		struct main_vector *empty_vector)
{
	char *s, *p;
	int r;

	s = NULL;
	p = params;
	r = 0;
	while (*p != E_CHAR)
	{
		if (*p == ':')
		{
			if (s == NULL)
			{
				r = -1;
				break;
			}
			else
			{
				*p = E_CHAR;
				main_vector_append(start_vector, s);
				main_vector_append(empty_vector, p);
				s = NULL;
			}
		}
		else if (s == NULL)
		{
			s = p;
		}
		++p;
	}

	if (!r && s != NULL)
	{
		main_vector_append(start_vector, s);
		s = NULL;
	}

	return r;
}

int main_options_check_package_and_resource_files(struct main_options *options,
		char *extra_packages, char *resource_files)
{
	struct main_vector p_vector, r_vector, pe_vector, re_vector;
	int r;

	main_vector_initial(&p_vector, -1);
	main_vector_initial(&r_vector, -1);
	main_vector_initial(&pe_vector, -1);
	main_vector_initial(&re_vector, -1);

	r = 0;
	r = main_options_split_item_from_clone(extra_packages, &p_vector, &pe_vector);
	if (!r)
		r = main_options_split_item_from_clone(resource_files, &r_vector, &re_vector);

	if (!r && p_vector.n != r_vector.n)
	{
		fprintf(stdout, "extra packages size is not equal to resource files size\n");
		r = -1;
	}

	if (!r && !p_vector.n)
	{
		fprintf(stdout, "extra packages not set\n");
		r = -1;
	}

	if (!r)
	{
		main_options_prepare_size(options, p_vector.n);
		for (int i = 0; i < p_vector.n; ++i)
		{
			main_string_assign(&options->packages[i], (char*) p_vector.head[i]);
			main_string_assign(&options->resource_r_files[i], (char*) r_vector.head[i]);
		}
	}

	for (int i = 0; i < pe_vector.n; ++i)
		*(char*) pe_vector.head[i] = ':';
	for (int i = 0; i < re_vector.n; ++i)
		*(char*) re_vector.head[i] = ':';

	main_vector_release(&p_vector);
	main_vector_release(&r_vector);
	main_vector_release(&pe_vector);
	main_vector_release(&re_vector);

	return r;
}

int main_options_check_statements(struct main_options *options)
{
	if (options->gen_dir.n <= 0)
	{
		fprintf(stdout, "gen directory not set\n");
		return -1;
	}
	if (options->n <= 0)
	{
		fprintf(stdout, "extra packages and resource-r-files not set\n");
		return -1;
	}
	if (options->r_file.n <= 0)
	{
		fprintf(stdout, "--R-file is not set\n");
		return -1;
	}
	if (!main_file_is_exists(options->r_file.data))
	{
		fprintf(stdout, "--R-file '%s' is not existed\n", options->r_file.data);
		return -1;
	}
	for (int i = 0; i < options->n; ++i)
	{
		if (options->packages[i].n <= 0)
		{
			fprintf(stdout, "illegal package name\n");
			return -1;
		}

		if (options->resource_r_files[i].n <= 0)
		{
			fprintf(stdout, "illegal resource-r-file\n");
			return -1;
		}

		if (!main_file_is_exists(options->resource_r_files[i].data))
		{
			fprintf(stdout, "resource-r-file does not exists:%s\n",
					options->resource_r_files[i].data);
			return -1;
		}
	}
	return 0;
}
