#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ios>
#include <iostream>
#include <fstream>
#include <vector>

#include "ResourceType.h"
#include "MainPrint.h"
#include "MainHelp.h"

#define TAB_SOFT_SPACING 	4

static void printNSoftTab(std::ostream &out, int n)
{
	printNChar(out, ' ', n * TAB_SOFT_SPACING);
}

static bool parseReosurceLine(const std::string &line, std::string &typeString, std::string &categoryName, std::string &name, std::string &value);

ResourceItem::ResourceItem(const ResourceCategory *category)
: mCategory(category)
, mValueType(ResourceItem::NoneType)
{
}

ResourceItem::~ResourceItem()
{
}

void ResourceItem::writeJavaFile(std::ostream &out, WriteJavaOptions *options)
{
	printNSoftTab(out, options->step);
	out << "public static ";
	if (!options->isNonConstantId)
		out << "final ";
	out << ResourceItem::valueTypeToString(mValueType) << " ";
	out << mName << " = " << mValue << ";" << std::endl;
}

void ResourceItem::writeResourceFile(std::ostream &out)
{
	out << ResourceItem::valueTypeToString(mValueType) << " "
	    << mCategory->name() << " "
	    << mName << " "
	    << mValue << std::endl;	
}

ResourceCategory::ResourceCategory(const std::string &name)
: mName(name)
{
	deleteContainerElements(mData);
}

// static
ResourceItem::ValueType ResourceItem::stringToValueType(const std::string &value)
{
	if (strcmp(value.c_str(), "int") == 0) {
		return ResourceItem::IntType;
	} else if (strcmp(value.c_str(), "int[]") == 0) {
		return ResourceItem::IntArrayType;
	} else {
		return ResourceItem::NoneType;
	}
}

// static
std::string ResourceItem::valueTypeToString(ResourceItem::ValueType type) 
{
	switch (type)
	{
		case ResourceItem::IntType:
			return "int";
		case ResourceItem::IntArrayType:
			return "int[]";
		default:
			return "";
	}
}

ResourceCategory::~ResourceCategory()
{
	deleteContainerElements(mData);
}

void ResourceCategory::addResourceItem(ResourceItem *item)
{
	mData.push_back(item);
}

void ResourceCategory::writeJavaFile(std::ostream &out, WriteJavaOptions *options)
{
	printNSoftTab(out, options->step);
	out << "public static class " << mName << " {" << std::endl;
	++options->step;
	for (std::vector<ResourceItem*>::iterator iter = mData.begin();
		 iter != mData.end(); ++iter) {
		(*iter)->writeJavaFile(out, options);
	}
	--options->step;
	printNSoftTab(out, options->step);
	out << "}\n";
}

void ResourceCategory::writeResourceFile(std::ostream &out)
{
	for (std::vector<ResourceItem*>::iterator iter = mData.begin();
		 iter != mData.end(); ++iter) 
	{
		(*iter)->writeResourceFile(out);
	}
}

void ResourceCategory::updateResourceItemValueFromCategory(ResourceCategory &category)
{
	ResourceItem *dstItem;

	for (std::vector<ResourceItem*>::iterator iter = mData.begin();
		 iter != mData.end(); ++iter) {
		if ((dstItem = category.findResourceItemByName((*iter)->name())) != NULL) {
			(*iter)->setValue(dstItem->value());
		} else {
			std::cout << "[ERROR:] Can't find resource item '" << (*iter)->name() <<"' from category '" 
				<< category.name() << "'" << std::endl; 
		}
	}
}

ResourceItem *ResourceCategory::findResourceItemByName(const std::string &name) 
{
	for (std::vector<ResourceItem*>::iterator iter = mData.begin();
		 iter != mData.end(); ++iter) {
		if (name == (*iter)->name()) {
			return *iter;
		}
	}

	return NULL;
}

ResourceTable::ResourceTable()
{
}

ResourceTable::~ResourceTable()
{
	reset();
}

bool ResourceTable::initFromFile(const std::string &path)
{
	std::fstream inFile(path.c_str(), std::ios::in);
	std::string line, typeName, categoryName, name, value;
	bool r;
	ResourceCategory *category = NULL;
	ResourceItem *item;

	if (!inFile.good())
	{
		inFile.close();
		return false;
	}

	r = true;
	while (inFile.good())
	{
		std::getline(inFile, line);
		if (line.empty()) 
		{
			continue;
		}
		
		if (!parseReosurceLine(line, typeName, categoryName, name, value))
		{
			r = false;
			break;
		}

		if (!category || categoryName != category->name())
		{
			category = new ResourceCategory(categoryName);
			this->mData.push_back(category);
		}
		item = new ResourceItem(category);
		category->addResourceItem(item);
		item->setValueType(ResourceItem::stringToValueType(typeName));
		item->setName(name);
		item->setValue(value);
	}

	inFile.close();
	return r;
}

ResourceCategory *ResourceTable::findCategoryByName(const std::string &name)
{
	for (std::vector<ResourceCategory*>::iterator iter = mData.begin();
		 iter != mData.end(); ++iter) {
		if (name == (*iter)->name()) {
			return *iter;
		}
	}
	return NULL;
}

void ResourceTable::updateResourceItemValueFromTable(ResourceTable &table)
{
	ResourceCategory *category;
	for (std::vector<ResourceCategory*>::iterator iter = mData.begin();
		 iter != mData.end(); ++iter) {
		if ((category = table.findCategoryByName((*iter)->name())) != NULL) {
			(*iter)->updateResourceItemValueFromCategory(*category);
		} else {
			std::cout << "[ERROR:] Can't find category for '" << (*iter)->name() << "'" << std::endl;
		}
	}
}

void ResourceTable::writeJavaFile(std::ostream &out, WriteJavaOptions *options) 
{
	out << "package " << options->packageName << ";\n";
	out << "\n";
	out << "public class R {\n";

	++options->step;
	for(std::vector<ResourceCategory*>::iterator iter = mData.begin();
		iter != mData.end(); ++iter)
	{
		(*iter)->writeJavaFile(out, options);
		out << "\n";
	}
	--options->step;	

	out << "}";
}

void ResourceTable::writeResourceFile(std::ostream &out)
{
	for (std::vector<ResourceCategory*>::iterator iter = mData.begin();
		 iter != mData.end(); ++iter)
	{
		(*iter)->writeResourceFile(out);
	}
}

void ResourceTable::reset()
{
	deleteContainerElements(mData);
}

WriteJavaOptions::WriteJavaOptions()
: isNonConstantId(true)
, step(0)
{
}

WriteJavaOptions::~WriteJavaOptions()
{
}

static const char *skipWhitespace(const char *ptr) 
{
	while (*ptr != '\0' && (*ptr == ' ' || *ptr == '\t'))
		++ptr;
	return ptr;
}

static const char *skipNonWhitespace(const char *ptr)
{
	while (*ptr != '\0' && *ptr != ' ' && *ptr != '\t' && *ptr != '\n')
		++ptr;
	return ptr;
}

static const char *skipUtilNewline(const char *ptr)
{
	while (*ptr != '\0' && *ptr != '\n')
		++ptr;
	return ptr;
}

bool parseReosurceLine(const std::string &line, std::string &typeString, std::string &categoryName, std::string &name, std::string &value)
{
	const char *start;
	const char *end;

	start = line.c_str();
	if (*(start = skipWhitespace(start)) != '\0' && *(end = skipNonWhitespace(start)) != '\0') 
		typeString.assign(start, end);
	else 
		return false;

	if (*(start = skipWhitespace(end)) != '\0' && *(end = skipNonWhitespace(start)) != '\0')
		categoryName.assign(start, end);
	else
		return false;

	if (*(start = skipWhitespace(end)) != '\0' && *(end = skipNonWhitespace(start)) != '\0')
		name.assign(start, end);
	else
		return false;

	if (*(start = skipWhitespace(end)) != '\0' && (end = skipUtilNewline(start)))
		value.assign(start, end);
	else
		return false;

	return true;
}