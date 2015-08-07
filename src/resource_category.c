#include <stdio.h>
#include <stdlib.h>


#include "resource_category.h"
#include "main_math.h"
#include "print_util.h"
#include "resource_table.h"

static void mainResourceCategoryCheckCapacity(main_resource_category *ptr, int c) {
	if (c <= ptr->c)
		return;
	
	ptr->c = MAX(ptr->c * 2, c);
	if (ptr->head == NULL) {
		ptr->head = (main_resource_item*) malloc(ptr->c * sizeof(main_resource_item));
	} else {
		ptr->head = (main_resource_item*) realloc(ptr->head, ptr->c * sizeof(main_resource_item));
	}
}

void mainResourceCategoryInitial(struct main_resource_category *ptr, int c)
{
	if (c <= 0)
		c = 512;
	ptr->c = ptr->n = 0;
	ptr->head = NULL;
	mainResourceCategoryCheckCapacity(ptr, c);
}

void mainResourceCategoryRelease(struct main_resource_category *ptr)
{
	for (int i = 0; i < ptr->n; ++i)
		mainResourceItemRelease(ptr->head + i);
	free(ptr->head);
	ptr->head = NULL;
	free(ptr->name);
	ptr->name = NULL;
	ptr->n = ptr->c = 0;
}

main_resource_item *mainResourceCategoryAppend(main_resource_category *ptr)
{
	main_resource_item *item;
	
	mainResourceCategoryCheckCapacity(ptr, ptr->n + 1);
	item = &ptr->head[ptr->n++];
	mainResourceItemInitial(item);

	return item;
}

/**
 * 删除一个子成员
 */
void mainResourceCategoryRemove(main_resource_category *ptr, int i)
{
	main_resource_item *src, *dst, *end;

	src = ptr->head + i;
	dst = src + 1;
	end = ptr->head + ptr->n;
	mainResourceItemRelease(&ptr->head[i]);
	while (dst < end)
		*src++ = *dst++;
	--ptr->n;
}

void mainResourceCategoryPrint(main_resource_category *ptr, FILE *file)
{
	for (int i = 0; i < ptr->n; ++i) {
		mainResourceItemPrint(&ptr->head[i], file);
		fputc('\n', file);
		fflush(file);
	}
}

void mainResourceCategoryPrintJava(main_resource_category *ptr, FILE *file,
		main_resource_print_options *options)
{
	main_fprint_nchar(file, options->step, '\t');
	fprintf(file, "public static final class %s {\n", ptr->name);
	++options->step;

	for (int i = 0; i < ptr->n; ++i)
		mainResourceItemPrintJava(ptr->head + i, file, options);

	--options->step;
	main_fprint_nchar(file, options->step, '\t');
	fprintf(file, "}\n");
}