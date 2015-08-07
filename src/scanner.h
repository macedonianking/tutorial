/*
 * tutorial_max_line.h
 *
 *  Created on: 2015年6月1日
 *      Author: lhb
 */
#ifndef MAIN_LINE_SPLIT
#define MAIN_LINE_SPLIT

#include <stdio.h>
#include "main_string.h"

typedef struct main_line_split
{
	FILE *file;
	char *head;
	int n;
	int c;
	int seek;
	struct main_string text;
} main_line_split;


void mainLineSplitInitial(main_line_split *ptr, FILE *file, int capactiy);
void mainLineSplitRelease(main_line_split *ptr);
const char* mainLineSplitNextLine(main_line_split *ptr, int *n);

#endif // MAIN_LINE_SPLIT

