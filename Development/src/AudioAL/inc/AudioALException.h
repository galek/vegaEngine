#ifndef _OGREAL_EXCEPTION_H_
#define _OGREAL_EXCEPTION_H_

#include "OgreString.h"
#include "AudioALPrereqs.h"

namespace vega {
	void check(bool condition, const int errorNumber, const std::string& description, const std::string& source);
	void check(const Error error, const std::string& description, const std::string& source);

	std::string getErrorDescription(const Error error);

	#define CheckCondition(condition, errorNumber, description) check((condition), (errorNumber), (description), __FUNCTION__)
	#define CheckError(error, description) check((error), (description), __FUNCTION__)
} // Namespace
#endif
