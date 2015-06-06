/*
 * print_util.c
 *
 *  Created on: 2015年5月31日
 *      Author: lhb
 */

#include <stdio.h>
#include <stdlib.h>

#include "print_util.h"

void tutorial_print_n_char(int n, char c)
{
	while (n-- != 0)
	{
		putchar(c);
	}
}
