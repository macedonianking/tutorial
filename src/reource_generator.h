/*
 * reource_generator.h
 *
 *  Created on: 2015年6月4日
 *      Author: lhb
 */

#ifndef TUTORIAL_RESOURCE_GENERATOR_H
#define TUTORIAL_RESOURCE_GENERATOR_H

struct tutorial_resource_generator;

/**
 * 创建对象
 */
struct tutorial_resource_generator *tutorial_resource_generator_new();
/**
 * 销毁对象
 */
void tutorial_resource_generator_release(struct tutorial_resource_generator *ptr);
/**
 * 初始化
 */
int tutorial_resource_generator_initial(struct tutorial_resource_generator *ptr, FILE *file);

#endif // TUTORIAL_RESOURCE_GENERATOR_H
