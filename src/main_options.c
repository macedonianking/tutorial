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
#include "resource_generator.h"

#define APP_NAME	"aaptr"

void main_options_initial(struct main_options *options)
{
	options->is_non_constant_id = 0;
	main_vector_initial(&options->v_packages);
	main_vector_initial(&options->v_resource_r_files);
	mainStringInitial(&options->gen_dir);
	mainStringInitial(&options->r_file);
}

void main_options_release(struct main_options *options)
{
	main_vector_release(&options->v_packages);
	main_vector_release(&options->v_resource_r_files);
	mainStringRelease(&options->gen_dir);
	mainStringRelease(&options->r_file);
}

void mainOptionsCallHelp()
{
	fprintf(stdout, "aaptr Android Package Resource Assist Tool\n\n");
	fprintf(stdout, "%s -J gen_dir --R-file rfile -P packages... -R R-files...\n"
			"    [--non-constant-id]\n", APP_NAME);
	fprintf(stdout, "    --R-file rfile [--non-constant-id]\n");
	fprintf(stdout, "    -J generated directory\n");
	fprintf(stdout, "    -P package names\n");
	fprintf(stdout, "    -R resource R.txt files\n");
	fprintf(stdout, "    --non-constant-id generate non constant R.java file\n");
}

int mainOptionsInitialFromArguments(struct main_options *options, int argc, char **argv)
{
	int i;
	int r;

	i = 1;
	r = 0;
	while (i < argc) {
		if (strcmp("-J", argv[i]) == 0)	{
			++i;
			if (i < argc) {
				mainStringAssign(&options->gen_dir, argv[i]);
				++i;
			} else {
				r = -1;
			}
		} else if (strcmp("--R-file", argv[i]) == 0) {
			++i;
			if (i < argc) {
				mainStringAssign(&options->r_file, argv[i]);
				++i;
			} else {
				r = -1;
			}
		} else if (strcmp("--non-constant-id", argv[i]) == 0) {
			options->is_non_constant_id = 1;
			++i;
		} else if (strcmp("-P", argv[i]) == 0) {
			++i;
			while (i < argc && *argv[i] != '-') {
				main_vector_vappend(&options->v_packages, argv[i], NULL);
				++i;
			}
		} else if (strcmp("-R", argv[i]) == 0) {
			++i;
			while (i < argc && *argv[i] != '-') {
				main_vector_vappend(&options->v_resource_r_files, argv[i], NULL);
				++i;
			}
		} else {
			fprintf(stdout, "Unknown arguments:'%s'\n", argv[i]);
			r = -1;
		}

		if (r != 0)
			break;
	}

	if (r == 0 && options->v_resource_r_files.n != options->v_packages.n)
		r = -1;

	return r;
}

int mainOptionsCheckStatus(struct main_options *options)
{
	if (options->gen_dir.n <= 0)
	{
		return -1;
	}

	if (options->r_file.n <= 0)
	{
		return -1;
	}
	if (mainFileIsFile(options->r_file.data) != 0)
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
		if (mainFileIsFile((char*) options->v_resource_r_files.head[i]) != 0)
		{
			return -1;
		}
	}
	return 0;
}

/**
 * 创建资源文件的路径
 */
int mainOptionsGetRFilePath(main_string *dst, const char *genDir, const char *pkgName)
{
	char buffer[MAIN_FILE_MAX_PATH];
	char *ptr;
	int n;

	mainStringReset(dst);
	mainStringAppend(dst, genDir);
	if ((n = dst->n - 1) >= 0 && dst->data[n] != FILE_SEP_UNIX && dst->data[n] != FILE_SEP_WINS)
		mainStringAppend(dst, FILE_SEP_WINS_STRING);

	strcpy(buffer, pkgName);
	ptr = buffer;
	while (*ptr != E_CHAR) {
		if (*ptr == '.')
			*ptr = FILE_SEP_WINS;
		++ptr;
	}
	mainStringAppend(dst, buffer);
	mainStringAppend(dst, FILE_SEP_WINS_STRING);
	mainStringAppend(dst, "R.java");
	return 0;
}

int doExecute(int argc, char **argv)
{
	int r;
	main_options options;

	r = 0;
	main_options_initial(&options);
	if (!mainOptionsInitialFromArguments(&options, argc, argv) && !mainOptionsCheckStatus(&options)) {
		r = mainResourceGeneratorExecute(&options);
	}
	else {
		mainOptionsCallHelp();
		r = -1;
	}
	main_options_release(&options);

	return r;
}
