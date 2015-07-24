#include "MainPrint.h"

void printNChar(std::ostream &out, char c, int n)
{
	while (n-- > 0)
		out << c;
}