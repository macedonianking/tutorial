#include "tutorial_max_line.h"

#include <stdio.h>
#include <stdlib.h>

#define MAIN_FILE_BUFFER_DEFAULT_CAPACITY 	1024

#define LINE_SPLIT_STATUS_NONE				0x0000
#define LINE_SPLIT_STATUS_LINE				0x0001
#define LINE_SPLIT_STATUS_STOP				0x0002

struct main_file_buffer *main_file_buffer_open(FILE *file, int capacity)
{
	struct main_file_buffer *buffer;

	buffer = (struct main_file_buffer*) malloc(sizeof(struct main_file_buffer));
	buffer->c = capacity > 0 ? capacity : MAIN_FILE_BUFFER_DEFAULT_CAPACITY;
	buffer->head = (char*) malloc(sizeof(char) * buffer->c);
	buffer->file = file;
	buffer->n = buffer->s = 0;

	return buffer;
}

void main_file_buffer_close(struct main_file_buffer *buffer)
{
	if (buffer != NULL)
	{
		free(buffer->head);
		buffer->c = buffer->n = buffer->s = 0;
		free(buffer);
	}
}

void main_file_buffer_parse(struct main_file_buffer *buffer, struct main_file_buffer_parser *parser,
		void *data)
{
	int p;

	buffer->status = MAIN_FILE_STATUS_DOCUMENT_ENTER;
	(*parser->file_buffer_enter)(data);
	while (1)
	{
		if (buffer->s == buffer->n)
		{
			buffer->n = fread(buffer->head, 1, buffer->c, buffer->file);
			buffer->s = 0;
			if (buffer->n <= 0)
				break;
		}

		p = buffer->s;
		while (buffer->s < buffer->n && buffer->head[buffer->s] != '\n')
			++buffer->s;

		if (buffer->status == MAIN_FILE_STATUS_DOCUMENT_LEAVE
				|| buffer->status == MAIN_FILE_STATUS_LINE_LEAVE)
		{
			buffer->status = MAIN_FILE_STATUS_LINE_ENTER;
			(*parser->file_buffer_line_enter)(buffer->head + p, buffer->s - p, data);
		}
		else
		{
			(*parser->file_buffer_line_seek)(buffer->head + p, buffer->s - p, data);
		}

		if (buffer->head[buffer->s] == '\n')
		{
			(*parser->file_buffer_line_leave)(data);
			buffer->status = MAIN_FILE_STATUS_LINE_LEAVE;
			++buffer->s;
		}
	}
}

void main_line_split_initial(struct main_line_split *ptr, FILE *file, int c)
{
	c = c < MAIN_FILE_BUFFER_DEFAULT_CAPACITY ?
	MAIN_FILE_BUFFER_DEFAULT_CAPACITY : c;
	ptr->c = c;
	ptr->head = (char *) malloc(sizeof(char) * ptr->c);
	ptr->n = ptr->seek = 0;
	ptr->file = file;
	main_string_ninitial(&ptr->text, MAIN_FILE_BUFFER_DEFAULT_CAPACITY);
}

void main_line_split_release(struct main_line_split *ptr)
{
	if (ptr != NULL)
	{
		free(ptr->head);
		ptr->c = ptr->seek = ptr->n = 0;
		ptr->file = NULL;
		main_string_release(&ptr->text);
	}
}

const char *main_line_split_next_line(struct main_line_split *ptr, int *n)
{
	int status;
	int start;

	status = LINE_SPLIT_STATUS_NONE;
	main_string_reset(&ptr->text);
	while (1)
	{
		if (ptr->seek == ptr->n)
		{
			ptr->n = fread(ptr->head, 1, ptr->c, ptr->file);
			ptr->seek = 0;
			if (ptr->n <= 0)
				break;
		}

		start = ptr->seek;
		while (ptr->seek != ptr->n && ptr->head[ptr->seek] != '\n')
			++ptr->seek;

		if (status == LINE_SPLIT_STATUS_NONE)
			status = LINE_SPLIT_STATUS_LINE;
		main_string_nappend(&ptr->text, ptr->head + start, ptr->seek - start);
		if (ptr->seek == ptr->n)
		{
			continue;
		}
		else
		{
			++ptr->seek;
			break;
		}
	}

	if (status == LINE_SPLIT_STATUS_LINE)
		status = LINE_SPLIT_STATUS_STOP;

	if (status == LINE_SPLIT_STATUS_NONE)
	{
		*n = 0;
		return NULL;
	}
	else
	{
		*n = ptr->text.n;
		return ptr->text.data;
	}
}
