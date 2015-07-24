#ifndef FileUtils_h
#define FileUtils_h

#define FILE_PATH_SEP_WIN_CHAR		'\\'
#define FILE_PATH_SEP_UNIX_CHAR		'/'

namespace file_util {

bool isPathExists(const char *path);
bool isFileExists(const char *path);
bool isDirectoryExists(const char* path);
bool createDirectory(const char *path);
std::string getParentDirectory(const std::string &path);
bool ensureFileParentDirectory(const std::string &path);
bool ensureDirectory(const std::string &path);

}

#endif // FileUtils_h