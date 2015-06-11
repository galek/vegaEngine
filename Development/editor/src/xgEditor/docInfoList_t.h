#pragma once
#include <vector>
#include "docInfo_t.h"
namespace vega
{
	class docInfoList_t :
		public std::vector < docInfo_t >
	{
	public:
		bool loadPluginListFile(const char *fileName);
		docInfoList_t(void);
		~docInfoList_t(void);
	};
}