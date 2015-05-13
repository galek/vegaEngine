#pragma once
#include <vector>
#include "docInfo_t.h"
namespace vega
{
	class docInfoList_t :
		public std::vector < docInfo_t >
	{
	public:
		//##ModelId=44B8E82C0003
		bool loadPluginListFile(const char *fileName);
		//##ModelId=44B8E82C0005
		docInfoList_t(void);
		//##ModelId=44B8E82C000F
		~docInfoList_t(void);
	};
}