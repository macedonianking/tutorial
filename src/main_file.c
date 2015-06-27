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

int main_file_is_directory(const char *str)
{
	int r;
	DIR *dir;

	r = access(str, F_OK);
	if (r == 0)
	{
		dir = opendir(str);
		if (dir != NULL)
		{
			closedir(dir);
		}
		else
		{
			r = -1;
		}
	}

	return r;
}

int main_file_is_file(const char *str)
{
	int r;

	r = -1;
	if (access(str, F_OK) && main_file_is_directory(str) != 0)
	{
		r = 0;
	}
	return r;
}

/**
 * 创建目录
 */
int main_file_mkdirs(const char *str)
{
	main_string path;
	int r, created;
	char *p;
	char c;

	main_string_initial(&path);
	main_string_assign(&path, str);

	r = 0;
	p = path.data;
	created = 0;
	while (*p != E_CHAR && r == 0)
	{
		if (*p == MAIN_FILE_SEP_UNIX_CHAR || *p == MAIN_FILE_SEP_WINS_CHAR)
		{
			c = *p;
			*p = E_CHAR;
			if (main_file_is_directory(path.data) != 0)
			{
				if (mkdir(path.data) == 0)
				{
					created = 1;
				}
				else
				{
					r = -1;
				}
			}

			*p = c;
		}
		++p;
	}

	if (r == 0)
	{
		if (main_file_is_directory(path.data) != 0)
		{
			if (mkdir(path.data) == 0)
			{
				created = 1;
			}
			else
			{
				r = -1;
			}
		}
	}

	if (r == 0 && !created)
	{
		r = 1;
	}

	main_string_release(&path);
	return r;
}

/**
 * 为文件准备目录
 */
int main_file_check_create_parent_dir(const char *str)
{
	main_string path;
	char *iter;
	char *last;
	char c;
	int r;

	main_string_initial(&path);
	main_string_assign(&path, str);
	for (iter = path.data, last = NULL; *iter != E_CHAR; ++iter)
	{
		if (*iter == MAIN_FILE_SEP_UNIX_CHAR || *iter == MAIN_FILE_SEP_WINS_CHAR)
		{
			last = iter;
		}
	}

	if (last != NULL)
	{
		c = *last;
		*last = E_CHAR;
		r = main_file_mkdirs(path.data);
		if (r == 0 || r == 1)
		{
			r = 0;
		}
		else
		{
			r = -1;
		}
		*last = c;
	}
	else
	{
		r = 0;
	}

	main_string_release(&path);
	return r;
}
