#pragma once

#include <string> 

struct docInfo_t
{
#ifdef _UNICODE
	std::wstring name;
	std::wstring desc;
	std::wstring dllName;
	std::wstring entryFunName;
#else
	std::string name;
	std::string desc;
	std::string dllName;
	std::string entryFunName;
#endif
};
