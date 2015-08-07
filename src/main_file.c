/*
 * main_file.c
 *
 *  Created on: 2015年6月25日
 *      Author: LiHongbing
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include "config.h"
#include "main_file.h"
#include "main_string.h"

int mainFileIsDirectory(const char *str)
{
	int r;
	DIR *dir;

	r = -1;
	if (access(str, F_OK) == 0) {
		dir = opendir(str);
		if (dir != NULL) {
			closedir(dir);
			r = 0;
		}
	} 

	return r;
}

int mainFileIsFile(const char *str)
{
	int r;

	r = -1;
	if (access(str, F_OK) == 0 && mainFileIsDirectory(str) != 0)
		r = 0;
	return r;
}

static int mainFileParentDirectoryPosition(char *path, int n)
{
	while (--n >= 0 && path[n] != FILE_SEP_UNIX && path[n] != FILE_SEP_WINS)
		;

	if (n < 0)
		n = -1;
	return n;
}

static int mainFileMkdirsImpl(char *path, int n)
{
	int i;
	int r;

	if (mainFileIsDirectory(path) == 0)
		return 1;

	i = mainFileParentDirectoryPosition(path, n);
	if (i >= 0)  {
		int old_c = path[i];
		path[i] = '\0';
		mainFileMkdirsImpl(path, i);
		path[i] = old_c;
	}

	r = mkdir(path);
	return r == 0 ? 0 : -1;
}

/**
 * 创建目录
 */
int mainFileMkdirs(const char *str)
{
	main_string path;
	int r;

	mainStringInitial(&path);
	mainStringAssign(&path, str);

	r = mainFileMkdirsImpl(path.data, path.n);

	mainStringRelease(&path);
	return r;
}

/**
 * 为文件准备目录
 */
int mainFileCheckCreateParentDir(const char *str)
{
	main_string *text;
	int r;
	int i;

	text = mainStringNew();
	mainStringAssign(text, str);
	
	r = 0;
	if ((i = mainFileParentDirectoryPosition(text->data, text->n)) >= 0) {
		char c = text->data[i];
		text->data[i] = '\0';
		mainFileMkdirs(text->data);
		r = mainFileIsDirectory(text->data);
		text->data[i] = c;
	}

	mainStringDelete(text);
	text = NULL;

	return r;
}
