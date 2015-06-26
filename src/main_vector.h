/*
 * main_vector.h
 *
 *  Created on: 2015年6月7日
 *      Author: lhb
 */

#ifndef MAIN_VECTOR_H
#define MAIN_VECTOR_H

typedef struct main_vector {
	void **head;
	int n;
	int c;
} main_vector;

void main_vector_initial(main_vector *p);
void main_vector_release(main_vector *p);

int main_vector_append(main_vector *p, void *data);
void main_vector_erase(main_vector *p, int i);

void main_vector_vappend(main_vector *ptr, ...);
#endif // MAIN_VECTOR_H
