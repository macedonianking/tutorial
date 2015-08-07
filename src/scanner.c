#include "scanner.h"

#include <stdio.h>
#include <stdlib.h>

#include "main_math.h"

#define MainFileBufferDefaultCapacity 	1024

#define LineSplitStatusNone				0x0000
#define LineSplitStatusLine				0x0001
#define LineSplitStatusStop				0x0002

void mainLineSplitInitial(main_line_split *ptr, FILE *file, int c)
{
	ptr->c = MAX(c, MainFileBufferDefaultCapacity);
	ptr->head = (char *) malloc(sizeof(char) * ptr->c);
	ptr->n = ptr->seek = 0;
	ptr->file = file;
	mainStringNinitial(&ptr->text, MainFileBufferDefaultCapacity);
}

void mainLineSplitRelease(struct main_line_split *ptr)
{
	if (ptr != NULL) {
		free(ptr->head);
		ptr->c = ptr->seek = ptr->n = 0;
		ptr->file = NULL;
		mainStringRelease(&ptr->text);
	}
}

const char *mainLineSplitNextLine(main_line_split *ptr, int *n)
{
	int status;
	int start;

	status = LineSplitStatusNone;
	mainStringReset(&ptr->text);
	while (1) {
		if (ptr->seek == ptr->n) {
			ptr->n = fread(ptr->head, 1, ptr->c, ptr->file);
			ptr->seek = 0;
			if (ptr->n <= 0)
				break;
		}

		start = ptr->seek;
		while (ptr->seek != ptr->n && ptr->head[ptr->seek] != '\n')
			++ptr->seek;

		if (status == LineSplitStatusNone)
			status = LineSplitStatusLine;
		mainStringNappend(&ptr->text, ptr->head + start, ptr->seek - start);
		if (ptr->seek == ptr->n) {
			continue;
		} else {
			++ptr->seek;
			break;
		}
	}

	if (status == LineSplitStatusLine)
		status = LineSplitStatusStop;
	if (status == LineSplitStatusNone) {
		*n = 0;
		return NULL;
	} else {
		*n = ptr->text.n;
		return ptr->text.data;
	}
}
