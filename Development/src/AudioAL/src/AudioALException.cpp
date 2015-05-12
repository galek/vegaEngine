#include "AudioALException.h"
#include "OgreException.h"
#include "OgreLogManager.h"

namespace vega {
	void check(bool condition, const int errorNumber, const std::string& description, const std::string& source)
	{
		if (!condition)
		{
			Ogre::Exception exception(errorNumber, description, source);
			Ogre::LogManager::getSingleton().logMessage(exception.getFullDescription());
			throw exception;
		}
	}

	void check(const Error error, const std::string& description, const std::string& source)
	{
		if (error != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << description << ": OpenAL Error: " << getErrorDescription(error);
			Ogre::Exception exception(error, ss.str(), source);
			Ogre::LogManager::getSingleton().logMessage(exception.getFullDescription());
			throw exception;
		}
	}

	std::string getErrorDescription(const Error error)
	{
		switch(error)
		{
		case AL_INVALID_VALUE:
			return std::string("The value pointer given is not valid");
			break;
		case AL_INVALID_ENUM:
			return std::string("The specified parameter is not valid");
			break;
		case AL_INVALID_NAME:
			return std::string("The specified source name is not valid");
			break;
		case AL_INVALID_OPERATION:
			return std::string("There is no current context");
			break;
		case AL_OUT_OF_MEMORY:
			return std::string("The requested operation resulted in OpenAL running out of memory");
			break;
/*		case OV_EREAD:
			return std::string("Read from media.");
			break;
		case OV_ENOTVORBIS:
			return std::string("Not Vorbis data.");
			break;
		case OV_EVERSION:
			return std::string("Vorbis version mismatch.");
			break;
		case OV_EBADHEADER:
			return std::string("Invalid Vorbis header.");
			break;
		case OV_EFAULT:
			return std::string("Internal logic fault (bug or heap/stack corruption.");
			break;
*/		default:
			return std::string("Unknown Error");
			break;
		}
	}
} // Namespace
