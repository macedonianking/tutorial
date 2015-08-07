#ifndef MAIN_OPTIONS_H
#define MAIN_OPTIONS_H

#include "main_string.h"
#include "main_vector.h"

typedef struct main_options
{
	main_string *packages;
	main_string *resource_r_files;
	main_vector v_packages;
	main_vector v_resource_r_files;
	int is_non_constant_id;
	struct main_string gen_dir;
	struct main_string r_file;
} main_options;

void main_options_initial(struct main_options *options);
void main_options_release(struct main_options *options);

void mainOptionsCallHelp();

/**
 * 运行参数解析
 */
int mainOptionsInitialFromArguments(main_options *options, int argc, char **argv);
int mainOptionsCheckStatus(main_options *options);

/**
 * 获取文件路径
 */
int mainOptionsGetRFilePath(main_string *dst, const char *genDir, const char *pkgName);
/**
 * 实际执行
 */
int doExecute(int argc, char **argv);

#endif // MAIN_OPTIONS_H
