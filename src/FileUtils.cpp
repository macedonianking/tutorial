#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include <string>

#include "FileUtils.h"

namespace file_util 
{

bool isPathExists(const char *path)
{
	return access(path, F_OK) == 0;
}

bool isFileExists(const char *path)
{
	return isPathExists(path) && !isDirectoryExists(path);
}

bool isDirectoryExists(const char *path)
{
	DIR *dir;

	dir = opendir(path);
	if (dir != NULL)
	{
		closedir(dir);	
		return true;
	}
	return false;
}

static bool createDirectoryImpl(char *start, char *end)
{
	int c;

	if (isDirectoryExists(start)) 
	{
		return false;
	}

	c = -1;
	while (--end >= start)
	{
		if (*end == FILE_PATH_SEP_WIN_CHAR || *end == FILE_PATH_SEP_UNIX_CHAR)
		{
			c = *end;
			break;
		}
	}

	if (c != -1)
	{
		*end = '\0';
		createDirectoryImpl(start, end);
		*end = c;
	}
	mkdir(start);
	return isDirectoryExists(start);
}

bool createDirectory(const char *path) 
{
	std::string pathText(path);
	char *start;

	start = (char*)pathText.c_str();
	return createDirectoryImpl(start, start + pathText.size());
}

std::string getParentDirectory(const std::string &path)
{
	int endIndex;

	endIndex = (int) path.size() - 1;
	while (endIndex >= 0 && path[endIndex] != FILE_PATH_SEP_UNIX_CHAR && path[endIndex] != FILE_PATH_SEP_WIN_CHAR)
	{
		--endIndex;
	}

	if (endIndex > 0)
		return path.substr(0, endIndex);
	return std::string();
}

bool ensureFileParentDirectory(const std::string &path)
{
	std::string parentDir = getParentDirectory(path);
	if (!parentDir.empty())
	{
		return ensureDirectory(parentDir.c_str());		
	}

	return true;
}

bool ensureDirectory(const std::string &path)
{
	createDirectory(path.c_str());
	return isDirectoryExists(path.c_str());
}

}
