#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include "FileUtils.h"
#include "ResourceType.h"
#include "Generator.h"

static std::string createRFilePath(const std::string &genFolder, const std::string &packageName)
{
	std::string path(genFolder);
	std::string package(packageName);
	int size;

	size = (int)path.size();
	if (size != 0 && path[size - 1] != FILE_PATH_SEP_WIN_CHAR && path[size - 1]  != FILE_PATH_SEP_UNIX_CHAR)
	{
		path += "/";
	}

	size = (int)package.size();
	for (int i = 0; i != size; ++i)
	{
		if (package[i] == '.')
			package[i] = '/';
	}
	path += package;
	path += "/R.java";

	file_util::ensureFileParentDirectory(path);
	return path;
}

Generator::Generator()
: mIsNonConstantId(false)
{
}

Generator::~Generator()
{
}

bool Generator::doGenerate()
{
	ResourceTable iTable, oTable;
	std::fstream inFile;
	WriteJavaOptions options;
	int size;
	bool r;

	if (!file_util::ensureDirectory(mGenFolder))
	{
		return false;
	}

	if (!iTable.initFromFile(mRFile))
	{
		return false;
	}

	r = true;
	size = (int) mRFiles.size();
	for (int i = 0; i != size; ++i)
	{
		oTable.reset();
		if (!oTable.initFromFile(mRFiles[i]))
		{
			r = false;
			break;
		}

		oTable.updateResourceItemValueFromTable(iTable);
		inFile.open(createRFilePath(mGenFolder, mPackages[i]).c_str(), std::ios::out);
		if (!inFile.is_open())
		{
			r = false;
			break;
		}
		options.step = 0;
		options.isNonConstantId = this->mIsNonConstantId;
		options.packageName = mPackages[i];
		oTable.writeJavaFile(inFile, &options);
		inFile.close();
	}

	return r;
}

bool Generator::checkOptionsError()
{
	if (mGenFolder.empty() || mPackages.size() <= 0 || mPackages.size() != mRFiles.size()
		|| mRFile.empty())
	{
		return false;
	}

	if (!file_util::isFileExists(mRFile.c_str()))
	{
		return false;
	}

	return true;
}

void Generator::callHelp()
{
	std::cout << "aaptr Android Package Resource Assist Tool\n\n";
	std::cout << "aaptr -J gen_dir --R-file rfile -P packages... -R R-files...\n"
				 "    [--non-constant-id]";
	std::cout << "    --R-file rfile [--non-constant-id]\n";
	std::cout << "    -J generated directory\n";
	std::cout << "    -P package names\n";
	std::cout << "    -R resource R.txt files\n";
	std::cout << "    --non-constant-id generate non constant R.java file\n";
}

Generator *Generator::create(const std::vector<char*> &options)
{
	Generator *ptr = new Generator();
	int size = (int)options.size();
	int i;

	i = 0;
	while (i < size)
	{
		if (strcmp("-J", options[i]) == 0)
		{
			ptr->mGenFolder = options[++i];
			++i;
		}
		else if (strcmp("--R-file", options[i]) == 0)
		{
			ptr->mRFile = options[++i];
			++i;
		}
		else if (strcmp("-P", options[i]) == 0)
		{
			++i;
			while (i < size && *options[i] != '-')
			{
				ptr->mPackages.push_back(std::string(options[i++]));
			}
		}
		else if(strcmp("-R", options[i]) == 0)
		{
			++i;
			while (i < size && *options[i] != '-')
			{
				ptr->mRFiles.push_back(std::string(options[i++]));
			}
		}
		else if (strcmp("--non-constant-id", options[i]) == 0)
		{
			ptr->mIsNonConstantId = true;
			++i;
		}
		else
		{
			std::cout << "Unkown options " << options[i] << std::endl;
			break;
		}
	}

	if (!ptr->checkOptionsError())
	{
		delete ptr;
		ptr = NULL;
	}

	return ptr;
}