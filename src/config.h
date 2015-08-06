/*
 * config.h
 *
 *  Created on: 2015年5月31日
 *      Author: lhb
 */

#ifndef TUTORIAL_CONFIG_H
#define TUTORIAL_CONFIG_H

#ifndef NDEBUG
#define IS_DEBUG
#else
#endif // NDEBUG

#ifdef IS_DEBUG
#define DCHECK(cond) do {  \
	if (!(cond)) \
		(*(int*)0) = 1024; \
	} while (0)
#else
#define DCHECK(cond)
#endif // IS_DEBUG

#define E_CHAR			'\0'
#define DCHECK_SUCCESS	1
#define DCHECK_FAILURE	0

#endif // TUTORIAL_CONFIG_H
