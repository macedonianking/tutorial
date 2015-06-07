/*
 ============================================================================
 Name        : main.c
 Author      : LiHongbing
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <dir.h>

#include "line_count.h"
#include "tutorial_max_line.h"
#include "file_util.h"
#include "main_options.h"

#include "test/main_line_test.h"

int main(int argc, char **argv)
{
	main_options_call_help(stdout);
	return EXIT_SUCCESS;
}
