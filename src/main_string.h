/*
 * main_string.h
 *
 *  Created on: 2015年6月5日
 *      Author: lhb
 */

#ifndef MAIN_STRING_H
#define MAIN_STRING_H

#define MAIN_STRING_DEFAULT_CAPACITY 	1024

typedef struct main_string {
	char *data;
	int n;
	int c;
} main_string;

void main_string_initial(struct main_string *ptr, int capacity);
void main_string_release(struct main_string *ptr);

void main_string_erase(struct main_string *ptr);
void main_string_append(struct main_string *ptr, const char *data);
void main_string_nappend(struct main_string *ptr, const char *data, int n);
void main_string_assign(struct main_string *ptr, const char *data);
void main_string_nassign(struct main_string *ptr, const char *data, int n);

char *main_string_new(struct main_string *ptr);

char *main_string_new_string(const char *str);

#endif // MAIN_STRING_H
