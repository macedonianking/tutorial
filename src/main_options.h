#ifndef MAIN_OPTIONS_H
#define MAIN_OPTIONS_H

#include "main_string.h"

struct main_options
{
	struct main_string *packages;
	struct main_string *resource_r_files;
	int n;
	int is_non_constant_id;
	struct main_string gen_dir;
	struct main_string r_file;
};

void main_options_initial(struct main_options *options);
void main_options_release(struct main_options *options);
void main_options_prepare_size(struct main_options *options, int n);

void main_options_call_help(FILE *file);

/**
 * 运行参数解析
 */
int main_options_initialize_from_arguments(struct main_options *options, int argc, char **argv);

#endif // MAIN_OPTIONS_H
