#ifndef ResourceCategory_h
#define ResourceCategory_h

#include "resource_item.h"

struct main_resource_print_options;

typedef struct main_resource_category {
	struct main_resource_item *head;
	char *name;
	volatile int n;
	int c;
} main_resource_category;

void mainResourceCategoryInitial(main_resource_category *ptr, int c);
void mainResourceCategoryRelease(main_resource_category *ptr);

main_resource_item *mainResourceCategoryAppend(main_resource_category *ptr);
void mainResourceCategoryRemove(main_resource_category *ptr, int i);
void mainResourceCategoryPrint(main_resource_category *ptr, FILE *file);
void mainResourceCategoryPrintJava(main_resource_category *ptr, FILE *file, struct main_resource_print_options *options);

#endif // ResourceCategory_h