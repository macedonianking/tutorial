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
#include "resource_table.h"

typedef struct HandleContext
{
	main_resource_table *table;
	main_string *genDir;
	int isNonConstantId;
} HandleContext;

static int doHandleItemImpl(HandleContext *context, char *pkgName, char *srcFile);

int mainResourceGeneratorExecute(main_options *options)
{
	HandleContext context;
	FILE *iFile;
	int r;
	char *srcFile;
	char *pkgName;

	mainFileMkdirs(options->gen_dir.data);
	if (mainFileIsDirectory(options->gen_dir.data) != 0) {
		fprintf(stdout, "create directory: %s failed\n", options->gen_dir.data);
		return -1;
	}

	if ((iFile = fopen(options->r_file.data, "r")) == NULL) {
		fprintf(stdout, "open file failure:%s\n", options->r_file.data);
		return -1;
	}

	context.table = mainResourceTableNew();
	r = mainResourceTableInitFromFile(context.table, iFile);
	mainResourceTableSetFileName(context.table, options->r_file.data);
	context.isNonConstantId = options->is_non_constant_id;
	context.genDir = &options->gen_dir;
	fclose(iFile);
	iFile = NULL;

	r = 0;
	for (int i = 0; i < options->v_packages.n && r == 0; ++i) {
		srcFile = (char*)options->v_resource_r_files.head[i];
		pkgName = (char*)options->v_packages.head[i];
		r = doHandleItemImpl(&context, pkgName, srcFile);
	}
	mainResourceTableRelease(context.table);
	
	if (r != 0)
		r = -1;
	return r;
}

int doHandleItemImpl(HandleContext *context, char *pkgName, char *srcFile)
{
	int r;
	main_resource_table *table;
	main_resource_print_options print_options;
	main_string path;
	FILE *file;

	r = 0;
	if ((file = fopen((char*) srcFile, "r")) == NULL)
		return -1;
	
	table = mainResourceTableNew();
	r = mainResourceTableInitFromFile(table, file);
	mainResourceTableSetFileName(table, srcFile);
	fclose(file);
	file = NULL;
	
	if (r == 0) 
		r = mainResourceTableReplace(table, context->table);
	if (r == 0) {
		mainStringInitial(&path);
		mainOptionsGetRFilePath(&path, context->genDir->data, pkgName);
		if (mainFileCheckCreateParentDir(path.data) == 0)
			file = fopen(path.data, "w");
		mainStringRelease(&path);
	}

	if (file != NULL) {
		print_options.is_non_constant_id = context->isNonConstantId;
		print_options.package_name = pkgName;
		print_options.step = 0;
		mainResourceTablePrintJava(table, file, &print_options);
		fclose(file);
		file = NULL;
		r = 0;
	} else {
		r = -1;
	}
	mainResourceTableDelete(table);
	table = NULL;

	return r;
}
