/*
 * main_string.h
 *
 *  Created on: 2015年6月5日
 *      Author: lhb
 */

#ifndef MAIN_STRING_H
#define MAIN_STRING_H

typedef struct main_string
{
	char *data;
	int n;
	int c;
} main_string;

void mainStringInitial(main_string *ptr);
void mainStringNinitial(main_string *ptr, int capacity);
void mainStringRelease(main_string *ptr);

main_string *mainStringNew();
void mainStringDelete(main_string *ptr);

void mainStringReset(main_string *ptr);
void mainStringAppend(main_string *ptr, const char *data);
void mainStringNappend(main_string *ptr, const char *data, int n);
void mainStringAssign(main_string *ptr, const char *data);
void mainStringNAssign(main_string *ptr, const char *data, int n);

char *mainStringNewString(main_string *ptr);
char *mainStringCloneString(const char *str);

#endif // MAIN_STRING_H
