/*
 * main_vector.h
 *
 *  Created on: 2015年6月7日
 *      Author: lhb
 */

#ifndef MAIN_VECTOR_H
#define MAIN_VECTRO_H

struct main_vector
{
	void **head;
	int n;
	int c;
};

void main_vector_initial(struct main_vector *p, int c);
void main_vector_release(struct main_vector *p);
int main_vector_append(struct main_vector *p, void *data);
void main_vector_erase(struct main_vector *p, int i);

#endif // MAIN_VECTOR_H
