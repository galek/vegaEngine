/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"

using namespace Ogre;
#pragma message("NICK-REFACTOR IT 2015")
namespace vega
{
	//http://www.ogre3d.org/forums/viewtopic.php?p=507168#p507168
	//-------------------------------------------------------------------------------------
	const char* VFile::LoadFileHowCChar(const char *_mFilename)
	{
		try
		{
			pStream = ResourceGroupManager::getSingletonPtr()->openResource(_mFilename, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME.c_str()); // open the file 

			return (pStream->getAsString()).c_str();
		}
		catch (Ogre::Exception &e)
		{
			SeriousWarning(true, "[VFILE]Failed Open file %s", _mFilename);
			Debug(e.getFullDescription().c_str());
			return "Errror Loading";
		}
	}

	//-------------------------------------------------------------------------------------
	size_t VFile::LoadFileAndGetSize(const char *_mFilename)
	{
		try
		{
			pStream = ResourceGroupManager::getSingletonPtr()->openResource(_mFilename, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME.c_str()); // open the file 

			return (pStream->getAsString()).size();
		}
		catch (Ogre::Exception &e)
		{
			SeriousWarning(true, "[VFILE]Failed Open file %s", _mFilename);
			Debug(e.getFullDescription().c_str());
			return 0;
		}

	}

	//-------------------------------------------------------------------------------------
	const char* VFile::GetHowCChar()
	{
		if (!pStream.isNull())
			return (pStream->getAsString()).c_str();
		else
			return "";
	}

	//-------------------------------------------------------------------------------------
	size_t VFile::GetSize()
	{
		if (!pStream.isNull())
			return pStream->getAsString().size();
		else
			return 0;
	}

	//-------------------------------------------------------------------------------------
	std::string VFile::LoadFileHowString(const char *_mFilename)
	{
		try
		{
			pStream = ResourceGroupManager::getSingletonPtr()->openResource(_mFilename, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME.c_str()); // open the file 

			return pStream->getAsString();
		}
		catch (Ogre::Exception &e)
		{
			SeriousWarning(true, "[VFILE]Failed Open file %s", _mFilename);
			Debug(e.getFullDescription().c_str());
			return "Errror Loading";
		}
	}
	//-------------------------------------------------------------------------------------
}
