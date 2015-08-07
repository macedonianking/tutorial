#ifndef ResourceItem_h
#define ResourceItem_h

#include <stdio.h>

#include "main_string.h"

#define RESOURCE_ITEM_TYPE_INT			0x0000
#define RESOURCE_ITEM_TYPE_INT_ARRAY	0x0001
#define RESOURCE_ITEM_TYPE_NONE			0x0002

struct main_resource_print_options;

typedef struct main_resource_item {
	int type;
	char *category;
	char *name;
	char *value;
} main_resource_item;

void mainResourceItemInitial(main_resource_item *ptr);
void mainResourceItemRelease(main_resource_item *ptr);

void mainResourcItemSetNameValue(main_resource_item *ptr, main_string *name, main_string *value);
void mainResourceItemSetValue(main_resource_item *ptr, const char *value);
void mainResourceItemPrintJava(main_resource_item *ptr, FILE *file, struct main_resource_print_options *options);
void mainResourceItemPrint(main_resource_item *ptr, FILE *file);

#endif // ResourceItem_h
