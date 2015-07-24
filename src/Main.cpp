//============================================================================
// Name        : Main.cpp
// Author      : LiHongbing
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>

#include "FileUtils.h"
#include "Generator.h"

int main(int argc, char **argv) 
{
	std::vector<char*> options;
	Generator *generator;
	int r;

	for (int i = 1; i != argc; ++i)
	{
		options.push_back(argv[i]);
	}

	r = 0;
	generator = Generator::create(options);
	if (generator != NULL) 
	{
		generator->doGenerate();
	}
	else
	{
		Generator::callHelp();
		r = -1;
	}

	return r;
}
