/*
 * line_count.c
 *
 *  Created on: 2015年5月31日
 *      Author: lhb
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h> 

#include "config.h"
#include "print_util.h"
#include "line_count.h"

struct tutorial_line_count
{
	int ndigits[10];
	int nwhites;
	int nothers;
};

static void tutorial_count_file(FILE *file, struct tutorial_line_count *buf);
static void tutorial_print_horizontal_histogram(struct tutorial_line_count *buf);

void tutorial_line_count_main()
{
	struct tutorial_line_count buf;
	FILE *file;

	if ((file = fopen("..\\src\\line_count.c", "r")) == NULL)
	{
		return;
	}

	/**
	 * 统计个数
	 */
	memset(&buf, 0, sizeof(struct tutorial_line_count));
	tutorial_count_file(file, &buf);
	fclose(file);
	file = NULL;

	/**
	 * 绘图功能
	 */
	tutorial_print_horizontal_histogram(&buf);
}

void tutorial_count_file(FILE *file, struct tutorial_line_count *buf)
{
	int c;

	while ((c = fgetc(file)) != EOF)
	{
		if (c >= '0' && c <= '9')
		{
			++buf->ndigits[c - '0'];
		}
		else if (c == ' ' || c == '\t' || c == '\n')
		{
			++buf->nwhites;
		}
		else
		{
			++buf->nothers;
		}
	}
}

void tutorial_print_horizontal_histogram(struct tutorial_line_count *buf)
{
	for (int i = 0; i < 10; ++i)
	{
		printf("%6d:", i);
		tutorial_print_n_char(buf->ndigits[i], '*');
		putchar('\n');
	}

	printf("whites:");
	tutorial_print_n_char(buf->nwhites, '*');
	putchar('\n');

	printf("others:");
	tutorial_print_n_char(buf->nothers, '*');
	putchar('\n');
}

