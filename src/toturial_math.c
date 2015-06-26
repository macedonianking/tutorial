/*
 * toturial_math.c
 *
 *  Created on: 2015年5月31日
 *      Author: lhb
 */

#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "tutorial_math.h"

int tutorial_power(int base, int n) {
	int r;

	r = 1;
	for (int i = 0; i < n; ++i)
		r *= base;
	return r;
}
