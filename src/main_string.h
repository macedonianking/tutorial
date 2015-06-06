/*
 * main_string.h
 *
 *  Created on: 2015年6月5日
 *      Author: lhb
 */

#ifndef MAIN_STRING_H
#define MAIN_STRING_H

#define MAIN_STRING_DEFAULT_CAPACITY 	1024

struct main_string
{
	char *data;
	int n;
	int c;
};

void main_string_initial(struct main_string *ptr, int capacity);
void main_string_release(struct main_string *ptr);
void main_string_erase(struct main_string *ptr);
void main_string_append(struct main_string *ptr, const char *data);
void main_string_nappend(struct main_string *ptr, const char *data, int n);
char *main_string_new(struct main_string *ptr);

#endif // MAIN_STRING_H
