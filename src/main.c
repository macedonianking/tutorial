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

#include "line_count.h"
#include "tutorial_max_line.h"

#include "test/main_line_test.h"

int main(int argc, char **argv)
{
	main_line_test();
	printf("%s:Main Finish!!!\n", argv[0]);
	return EXIT_SUCCESS;
}
