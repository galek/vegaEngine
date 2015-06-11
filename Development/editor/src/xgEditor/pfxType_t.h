#pragma once

#include "type_t.h"

namespace vega
{
	class pfxType_t :public type_t
	{
	public:
		const std::string templateName;
		pfxType_t(const type_t&me, const char *templateName) :type_t(me), templateName(templateName){}
	};
}