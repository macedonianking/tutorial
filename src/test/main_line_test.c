/*
 * main_line_test.c
 *
 *  Created on: 2015年6月6日
 *      Author: lhb
 */

#include "main_line_test.h"

#include <stdlib.h>

#include "../main_options.h"
#include "../main_vector.h"

void main_line_test() {
	main_vector arguments;

	main_vector_initial(&arguments);
	main_vector_vappend(&arguments, "", "-J", "build/gen", "--R-file",
			"D:\\workspace\\MainBuild\\build\\R.txt", "--extra-packages",
			"com.oppo.build.library", "--r-resource-files",
			"D:\\workspace\\MainLibrary\\build\\R.txt", NULL);
	do_execute(arguments.n, (char**) arguments.head);
	main_vector_release(&arguments);
}
