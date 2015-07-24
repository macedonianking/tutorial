#ifndef ResourceType_h
#define ResourceType_h

#include <iostream>
#include <string>
#include <vector>

class ReosurceItem;
class ResourceCategory;
class ResourceTable;
class WriteJavaOptions;

class ResourceItem
{
public:
	enum ValueType
	{
		NoneType,
		IntType,
		IntArrayType,
	};

	ResourceItem(const ResourceCategory *category);
	~ResourceItem();

	void setValueType(ValueType valueType)
	{
		mValueType = valueType;
	}

	ValueType valueType() const
	{
		return mValueType;
	}

	void setName(const std::string &name)
	{
		mName = name;
	}

	const std::string &name() const
	{
		return mName;
	}

	std::string &name()
	{
		return mName;
	}

	void setValue(const std::string &value)
	{
		this->mValue = value;
	}

	const std::string &value() const
	{
		return this->mValue;
	}

	std::string &value()
	{
		return this->mValue;
	}

	void writeJavaFile(std::ostream &out, WriteJavaOptions *options);
	void writeResourceFile(std::ostream &out);

	static ValueType stringToValueType(const std::string &name);
	static std::string valueTypeToString(ValueType type);
private:
	const ResourceCategory 	*mCategory;

	ValueType mValueType;
	std::string mName;
	std::string mValue;
};

class ResourceCategory
{
public:
	ResourceCategory(const std::string &name);
	~ResourceCategory();

	void addResourceItem(ResourceItem *item);
	void writeJavaFile(std::ostream &out, WriteJavaOptions *options);
	void writeResourceFile(std::ostream &out);
	void updateResourceItemValueFromCategory(ResourceCategory &category);

	const std::string &name() const
	{
		return mName;
	}

private:
	ResourceItem *findResourceItemByName(const std::string &name);

private:
	std::string mName;
	std::vector<ResourceItem*> mData;
};

class ResourceTable
{
public:
	ResourceTable();
	~ResourceTable();

	bool initFromFile(const std::string &path);
	void updateResourceItemValueFromTable(ResourceTable &table);
	void writeJavaFile(std::ostream &out, WriteJavaOptions *options);
	void writeResourceFile(std::ostream &out);
	void reset();
private:
	ResourceCategory *findCategoryByName(const std::string &name);

private:
	std::vector<ResourceCategory*> mData;
};


class WriteJavaOptions
{
public:
	WriteJavaOptions();
	~WriteJavaOptions();

	bool isNonConstantId;
	int  step;
	std::string packageName;
};

#endif // ResourceType_h