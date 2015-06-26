/*
 * file_util.c
 *
 *  Created on: 2015年6月7日
 *      Author: lhb
 */

#include <stdio.h>
#include <stdlib.h>
#include <io.h>

#include "file_util.h"
#include "main_string.h"
#include "config.h"

static void main_file_check_create_dir(char *path);

/**
 * 判断是否存在
 */
int main_file_is_exists(const char *path) {
	return access(path, F_OK) == 0;
}

/**
 * 创建文件夹
 */
int main_file_create_directory(const char *path) {
	struct main_string text;
	int r;

	main_string_initial(&text, -1);
	main_string_assign(&text, path);

	r = 0;
	if (main_file_is_exists(text.data))
		r = -1;

	if (r == 0) {
		main_file_check_create_dir(text.data);
		r = main_file_is_exists(text.data);
	}

	main_string_release(&text);
	return r;
}

void main_file_check_create_dir(char *path) {
	char *p;
	char c;
	int finished;

	p = path;
	finished = 0;
	while (!finished) {
		while (*p != E_CHAR && *p != '\\' && *p != '/')
			++p;

		c = *p;
		*p = E_CHAR;
		if (main_file_is_exists(path)) {
			*p = c;
			if (*p == E_CHAR)
				finished = 1;
			else
				++p;
			continue;
		}

		mkdir(path);
		if (main_file_is_exists(path)) {
			*p = c;
			if (*p == E_CHAR)
				finished = 1;
			else
				++p;
		} else {
			*p = c;
			finished = 1;
		}
	}
}
