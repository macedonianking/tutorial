/*
 * main_file.h
 *
 *  Created on: 2015年6月25日
 *      Author: LiHongbing
 */

#ifndef MAIN_FILE_H
#define MAIN_FILE_H

#define MAIN_FILE_SEP_WINS_CHAR		'\\'
#define MAIN_FILE_SEP_UNIX_CHAR		'/'
#define MAIN_FILE_SEP_WINS_STRING	"\\"
#define MAIN_FILE_SEP_UNIX_STRING	"/"

#define MAIN_FILE_MAX_PATH	2048

/**
 * 是否是文件夹
 */
int main_file_is_directory(const char *str);

/**
 * 是否是文件
 */
int main_file_is_file(const char *str);

/**
 * 创建目录，如果目录已经存在，返回1，如果不能创建目录，返回-1，成功返回0
 */
int main_file_mkdirs(const char *str);

/**
 * 为文件准备目录
 */
int main_file_check_create_parent_dir(const char *str);

#endif // MAIN_FILE_H
