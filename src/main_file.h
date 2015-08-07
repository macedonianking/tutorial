/*
 * main_file.h
 *
 *  Created on: 2015年6月25日
 *      Author: LiHongbing
 */

#ifndef MAIN_FILE_H
#define MAIN_FILE_H

#define FILE_SEP_WINS		'\\'
#define FILE_SEP_UNIX		'/'
#define FILE_SEP_WINS_STRING	"\\"
#define FILE_SEP_UNIX_STRING	"/"

#define MAIN_FILE_MAX_PATH	2048

/**
 * 是否是文件夹
 */
int mainFileIsDirectory(const char *str);

/**
 * 是否是文件
 */
int mainFileIsFile(const char *str);

/**
 * 创建目录，如果目录已经存在，返回1，如果不能创建目录，返回-1，成功返回0
 */
int mainFileMkdirs(const char *str);

/**
 * 为文件准备目录
 */
int mainFileCheckCreateParentDir(const char *str);

#endif // MAIN_FILE_H
