#ifndef MainHelp_h
#define MainHelp_h

#include <vector>

template<typename Container>
void deleteContainerElements(Container &_cont)
{
	for (typename Container::iterator iter = _cont.begin();
		 iter != _cont.end(); ++iter) {
		delete *iter;
	}
	_cont.clear();
}


#endif // MainHelp_h