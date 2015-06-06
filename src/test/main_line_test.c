/*
 * main_line_test.c
 *
 *  Created on: 2015年6月6日
 *      Author: lhb
 */

#include <stdio.h>
#include <stdlib.h>

#include "../config.h"
#include "main_line_test.h"

#include "../main_string.h"
#include "../tutorial_max_line.h"

void main_line_test()
{
	struct main_line_split split;
	FILE *file;
	const char *ptr;
	int n;

	if ((file = fopen("D:\\workspace_cpp\\Tutorial\\a.txt", "r")) == NULL)
	{
		return;
	}

	main_line_split_initial(&split, file, 4096);
	while ((ptr = main_line_split_next_line(&split, &n)) != NULL)
		printf("%s\n", ptr);
	main_line_split_release(&split);

	fclose(file);
	file = NULL;
}
