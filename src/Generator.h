#ifndef Generator_h
#define Generator_h

#include <string>
#include <vector>

class Generator
{
public:
	static Generator *create(const std::vector<char*> &options);
	static void callHelp();

	Generator();
	~Generator();

	bool doGenerate();
private:
	bool checkOptionsError();

private:
	std::string mGenFolder;
	std::string mRFile;
	std::vector<std::string> mRFiles;
	std::vector<std::string> mPackages;
	
	bool mIsNonConstantId;
};

#endif // Generator_h