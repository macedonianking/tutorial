#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resource_item.h"
#include "print_util.h"
#include "resource_table.h"

static const char *gResourceTypeNames[] = { "int", "int[]" };

void mainResourceItemInitial(main_resource_item *ptr)
{
	ptr->category = ptr->name = ptr->value = NULL;
	ptr->type = RESOURCE_ITEM_TYPE_NONE;
}

void mainResourceItemRelease(main_resource_item *ptr)
{
	if (ptr->name) {
		free(ptr->name);
		ptr->name = NULL;
	}
	if (ptr->value) {
		free(ptr->value);
		ptr->value = NULL;
	}

	ptr->category = NULL;
	ptr->type = RESOURCE_ITEM_TYPE_NONE;
}

void mainResourcItemSetNameValue(main_resource_item *ptr, main_string *name,
		main_string *value)
{
	if (ptr->name)
		free(ptr->name);
	ptr->name = mainStringNewString(name);

	if (ptr->value)
		free(ptr->value);
	ptr->value = mainStringNewString(value);
}

void mainResourceItemPrintJava(struct main_resource_item *ptr, FILE *file, main_resource_print_options *options)
{
	main_fprint_nchar(file, options->step, '\t');
	if (options->is_non_constant_id)
		fprintf(file, "public static %s %s = %s;\n", gResourceTypeNames[ptr->type], ptr->name, ptr->value);
	else
		fprintf(file, "public static final %s %s = %s;\n", gResourceTypeNames[ptr->type], ptr->name, ptr->value);
}

void mainResourceItemPrint(struct main_resource_item *ptr, FILE *file)
{
	const char *text;

	text = gResourceTypeNames[ptr->type];
	fwrite(text, 1, strlen(text), file);
	fputc(' ', file);
	fwrite(ptr->category, 1, strlen(ptr->category), file);
	fputc(' ', file);
	fwrite(ptr->name, 1, strlen(ptr->name), file);
	fputc(' ', file);
	fwrite(ptr->value, 1, strlen(ptr->value), file);
}

void mainResourceItemSetValue(main_resource_item *ptr, const char *value)
{
	if (ptr->value)
		free(ptr->value);
	ptr->value = mainStringCloneString(value);
}