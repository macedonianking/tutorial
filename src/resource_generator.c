/*
 * resource_generator.c
 *
 *  Created on: 2015年6月4日
 *      Author: lhb
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#define ENTITY_SIZE_DEFUALT		100
#define CATEGORY_SIZE_DEFAULT	20

#define ENTITY_TYPE_NONE		0x0000
#define ENTITY_TYPE_INT 		0x0001
#define ENTITY_TYPE_INT_BUFFER	0x0002

#define TUTORIAL_FILE_BUFFER_SIZE	4096

#define INITIAL_FIND_WORD	0x0000
#define INITIAL_FIND_SPACE	0x0001
#define INITIAL_FIND_ENDL	0x0002

#define INITIAL_CONTEXT_TARGET_DATA_TYPE	0x0001
#define INITIAL_CONTEXT_TARGET_CATEGORY		0x0002
#define INITIAL_CONTEXT_TARGET_NAME			0x0003
#define INITIAL_CONTEXT_TARGET_VALUE		0x0004

struct tutorial_resource_entity
{
	int type;
	char *name;
	char *value;
};

struct tutorial_resource_category
{
	char *name;
	struct tutorial_resource_entity *head;
	int size;
	int capacity;
};

struct tutorial_resource_generator
{
	struct tutorial_resource_category *head;
	int size;
	int capacity;
};

struct tutorial_resource_line
{
	char *str;
	int length;
	int capacity;
	int curr_find;
	int curr_seek;
	int next_find;
};

struct tutorial_resource_file_buffer
{
	char buffer[TUTORIAL_FILE_BUFFER_SIZE];
	char *s;
	char *p;
	FILE *file;
};

struct tutorial_resource_initial_context
{
	struct tutorial_resource_generator *generator;
	struct tutorial_resource_category *category;
	struct tutorial_resource_entity entity;
	int target;
};

/**
 * 销毁内部成员
 */
static void tutorial_resource_entity_release(struct tutorial_resource_entity *ptr);
/**
 * 销毁资源类
 */
static void tutorial_resource_category_release(struct tutorial_resource_category *ptr);
/**
 * 
 */
static char* tutorial_resource_generator_skip_char(char *str, int find);

static int tutorial_resource_line_next(struct tutorial_resource_initial_context *context,
		struct tutorial_resource_file_buffer *buffer, struct tutorial_resource_line *line);

static void tutorial_resource_initial_context_handle_line(
		struct tutorial_resource_initial_context *initial_context,
		struct tutorial_resource_line *line);

/**
 * 
 */
static int tutorial_resource_file_buffer_next(struct tutorial_resource_file_buffer *buffer);

static struct tutorial_resource_category *tutorial_resource_generator_find_category(
		struct tutorial_resource_generator *ptr, const char *name);
static struct tutorial_resource_category *tutorial_resource_generator_add_category(
		struct tutorial_resource_generator *ptr, const char *name);
static struct tutorial_resource_category *tutorial_resource_generator_safe_get_category(
		struct tutorial_resource_generator *ptr, const char *name);

struct tutorial_resource_generator *tutorial_resource_generator_new()
{
	struct tutorial_resource_generator *ptr;

	ptr = (struct tutorial_resource_generator*) malloc(sizeof(struct tutorial_resource_generator));
	ptr->size = 0;
	ptr->capacity = CATEGORY_SIZE_DEFAULT;
	ptr->head = (struct tutorial_resource_category*) malloc(
			sizeof(struct tutorial_resource_category) * ptr->capacity);
	memset(ptr->head, sizeof(struct tutorial_resource_category) * ptr->capacity, 0);

	return ptr;
}

/**
 * 释放所有资源 
 */
void tutorial_resource_generator_release(struct tutorial_resource_generator* ptr)
{
	if (ptr != NULL)
	{
		for (int i = 0; i != ptr->size; ++i)
		{
			tutorial_resource_category_release(ptr->head + i);
		}
		if (ptr->head != NULL)
		{
			free(ptr->head);
			ptr->head = NULL;
		}
		ptr->size = ptr->capacity = 0;
		free(ptr);
	}
}

int tutorial_resource_generator_initial(struct tutorial_resource_generator *ptr, FILE *file)
{
	struct tutorial_resource_initial_context context;
	struct tutorial_resource_line line;
	struct tutorial_resource_file_buffer file_buffe;

	line.curr_find = INITIAL_FIND_ENDL;
	line.next_find = INITIAL_FIND_WORD;
	while (tutorial_resource_file_buffer_next(&file_buffe) == 0)
	{
		tutorial_resource_line_next(&context, &file_buffe, &line);
		tutorial_resource_initial_context_handle_line(&context, &line);
	}

	return 0;
}

void tutorial_resource_entity_release(struct tutorial_resource_entity *ptr)
{
	if (ptr != NULL)
	{
		if (ptr->name != NULL)
		{
			free(ptr->name);
			ptr->name = NULL;
		}
		if (ptr->value != NULL)
		{
			free(ptr->value);
			ptr->value = NULL;
		}
	}
}

void tutorial_resource_category_release(struct tutorial_resource_category *ptr)
{
	if (ptr != NULL)
	{
		if (ptr->name != NULL)
		{
			free(ptr->name);
			ptr->name = NULL;
		}

		for (int i = 0; i != ptr->size; ++i)
		{
			tutorial_resource_entity_release(ptr->head + i);
		}
		if (ptr->head != NULL)
		{
			free(ptr->head);
			ptr->head = NULL;
		}
		ptr->size = 0;
		ptr->capacity = 0;
	}
}

char *tutorial_resource_generator_skip_char(char *str, int find)
{
	if (find == INITIAL_FIND_WORD)
	{
		while (*str != E_CHAR && *str != ' ' && *str != '\n' && *str != '\t')
			++str;
	}
	else if (find == INITIAL_FIND_SPACE)
	{
		while (*str == ' ' || *str == '\t')
			++str;
	}
	else if (find == INITIAL_FIND_ENDL)
	{
		while (*str != E_CHAR && *str != '\n')
			++str;
	}
	else
	{
		DCHECK(DCHECK_FAILURE);
	}
	return str;
}

int tutorial_resource_file_buffer_next(struct tutorial_resource_file_buffer *ptr)
{
	return 0;
}

void tutorial_resource_line_append(struct tutorial_resource_line *ptr, char *s, char *e)
{
}

int tutorial_resource_line_next(struct tutorial_resource_initial_context *context,
		struct tutorial_resource_file_buffer *buffer, struct tutorial_resource_line *line)
{
	char *ptr;

	ptr = buffer->p;
	if (line->curr_find == INITIAL_FIND_ENDL)
	{
		DCHECK(line->next_find == INITIAL_FIND_WORD);
		while (*ptr != E_CHAR && (*ptr == ' ' || *ptr == '\t' || *ptr == '\n'))
			++ptr;
		if (*ptr != E_CHAR && *ptr != ' ' && *ptr != '\t' && *ptr != '\n')
		{
			line->curr_find = INITIAL_FIND_WORD;
			line->curr_seek = INITIAL_FIND_WORD;
			line->next_find = INITIAL_FIND_SPACE;
			buffer->s = ptr;
		}
		buffer->p = ptr;
	}
	else if (line->curr_find == INITIAL_FIND_SPACE)
	{
		DCHECK(line->next_find == INITIAL_FIND_WORD);
		while (*ptr != E_CHAR && *ptr == ' ' && *ptr == '\t' && *ptr == '\n')
			++ptr;
		if (*ptr != E_CHAR && *ptr != ' ' && *ptr != '\t' && *ptr != '\n')
		{
			line->curr_find = INITIAL_FIND_WORD;
			line->curr_seek = INITIAL_FIND_WORD;
			if (context->target == INITIAL_CONTEXT_TARGET_VALUE)
				line->next_find = INITIAL_FIND_ENDL;
			else
				line->next_find = INITIAL_FIND_SPACE;
			buffer->s = ptr;
		}
	}

	if (line->curr_find == INITIAL_FIND_WORD)
	{
		buffer->p = tutorial_resource_generator_skip_char(buffer->s, line->next_find);
		tutorial_resource_line_append(line, buffer->s, buffer->p);
		ptr = buffer->p;
		if (*ptr == E_CHAR)
		{
			line->curr_seek = INITIAL_FIND_WORD; //
		}
		else
		{
			if (line->next_find == INITIAL_FIND_ENDL && *ptr != '\n')
			{
				// We meet an ERROR.
			}
			if (line->next_find == INITIAL_FIND_SPACE && *ptr != ' ' && *ptr != '\t')
			{
				// We meet an ERROR.
			}
			line->curr_seek = INITIAL_FIND_SPACE; // 查找到解决
		}
	}
	return 0;
}

void tutorial_resource_initial_context_handle_line(
		struct tutorial_resource_initial_context *context, struct tutorial_resource_line *line)
{
	if (line->curr_find != INITIAL_FIND_WORD || line->curr_find != INITIAL_FIND_SPACE)
	{
		return;
	}

	if (context->target == INITIAL_CONTEXT_TARGET_DATA_TYPE)
	{
		int type = ENTITY_TYPE_NONE;
		if (strcmp("int", line->str) == 0)
			type = ENTITY_TYPE_INT;
		else if (strcmp("int[]", line->str) == 0)
			type = ENTITY_TYPE_INT_BUFFER;
		if (type == ENTITY_TYPE_NONE)
			exit(-1);
		context->entity.type = type;
	}
	else if (context->target == INITIAL_CONTEXT_TARGET_CATEGORY)
	{
		if (context->category == NULL || strcmp(context->category->name, line->str) != 0)
			context->category = tutorial_resource_generator_safe_get_category(context->generator,
					line->str);
	}
	else if (context->target == INITIAL_CONTEXT_TARGET_NAME)
	{
		context->entity.name = (char*) malloc((line->length + 1) * sizeof(char));
		strcpy(context->entity.name, line->str);
	}
	else if (context->target == INITIAL_CONTEXT_TARGET_VALUE)
	{
		context->entity.value = (char*) malloc((line->length + 1) * sizeof(char));
		strcpy(context->entity.value, line->str);
	}
	line->curr_find = INITIAL_FIND_SPACE;
	line->curr_seek = INITIAL_FIND_SPACE;
	line->next_find = INITIAL_FIND_WORD;
}

struct tutorial_resource_category *tutorial_resource_generator_add_category(
		struct tutorial_resource_generator *ptr, const char *name)
{
	struct tutorial_resource_category *q;

	if (ptr->size >= ptr->capacity)
	{
		ptr->capacity *= 2;
		ptr->head = (struct tutorial_resource_category*) realloc(ptr->head,
				ptr->capacity * sizeof(struct tutorial_resource_category));
	}
	q = &ptr->head[ptr->size++];
	q->name = (char*) malloc(strlen(name) + 1);
	strcpy(q->name, name);
	return q;
}

struct tutorial_resource_category *tutorial_resource_generator_find_category(
		struct tutorial_resource_generator *ptr, const char *name)
{
	struct tutorial_resource_category *iter;
	struct tutorial_resource_category *end;

	iter = ptr->head;
	end = ptr->head + ptr->size;
	while (iter != end)
	{
		if (strcmp(iter->name, name) == 0)
			break;
		++iter;
	}
	return iter != end ? iter : NULL;
}

struct tutorial_resource_category *tutorial_resource_generator_safe_get_category(
		struct tutorial_resource_generator *ptr, const char *name)
{
	struct tutorial_resource_category *iter;

	if ((iter = tutorial_resource_generator_find_category(ptr, name)) == NULL)
		iter = tutorial_resource_generator_add_category(ptr, name);
	return iter;
}
