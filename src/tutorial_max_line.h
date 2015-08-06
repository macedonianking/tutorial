/*
 * tutorial_max_line.h
 *
 *  Created on: 2015年6月1日
 *      Author: lhb
 */
#ifndef TUTORIAL_MAX_LINE_H
#define TUTORIAL_MAX_LINE_H

#include <stdio.h>
#include "main_string.h"

#define MAIN_FILE_STATUS_NONE				0x0000
#define MAIN_FILE_STATUS_DOCUMENT_ENTER		0x0001
#define MAIN_FILE_STATUS_DOCUMENT_LEAVE		0X0002
#define MAIN_FILE_STATUS_LINE_ENTER			0x0002
#define MAIN_FILE_STATUS_LINE_LEAVE			0x0003

struct main_file_buffer
{
	char *head;
	int n;
	int s;
	int c;
	FILE *file;
	int status;
};

struct main_line_split
{
	FILE *file;
	char *head;
	int n;
	int c;
	int seek;
	struct main_string text;
};

struct main_file_buffer_parser
{
	void (*file_buffer_enter)(void *data);
	void (*file_buffer_leave)(void *data);
	void (*file_buffer_line_enter)(char *head, int n, void *data);
	void (*file_buffer_line_seek)(char *head, int n, void *data);
	void (*file_buffer_line_leave)(void *data);
};

struct main_file_buffer *main_file_buffer_open(FILE *file, int category);

void main_file_buffer_close(struct main_file_buffer *file_buffer);

void main_file_buffer_parse(struct main_file_buffer *file_buffer,
		struct main_file_buffer_parser *parser, void *data);

void main_line_split_initial(struct main_line_split *ptr, FILE *file, int capactiy);
void main_line_split_release(struct main_line_split *ptr);
const char* main_line_split_next_line(struct main_line_split *ptr, int *n);
#endif // TUTORIAL_MAX_LINE_H

