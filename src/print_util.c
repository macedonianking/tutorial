/*
 * print_util.c
 *
 *  Created on: 2015年5月31日
 *      Author: lhb
 */

#include <stdio.h>
#include <stdlib.h>

#include "print_util.h"

void main_print_nchar(int n, char c) {
	while (n-- != 0) {
		putchar(c);
	}
}

void main_fprint_nchar(FILE *file, int n, int c) {
	while (n-- != 0)
		fputc(c, file);
}
