/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"

#if ENGINE_PLATFORM == ENGINE_PLATFORM_ANDROID

namespace vega
{
	FileSystem::FileSystem(EngineConfig*_config)
		:mConfig(_config)
	{
		// determine directories to search for config files
		getConfigPaths();
		// prepare write location in user directory
		prepareUserHome(ToStrW(_config->mProjectName.c_str()).c_str());
	}

	void FileSystem::getConfigPaths()
	{
	}
    //---------------------------------------------------------------------
	void FileSystem::prepareUserHome(const wchar_t* subdir)
	{
	}
    //---------------------------------------------------------------------
	bool FileSystem::fileExists(const wchar_t* path) const
	{
		return access(ToStr(path).c_str(), 00) == 0;
	}
    //---------------------------------------------------------------------
	bool FileSystem::createDirectory(const wchar_t* path)
	{
		return false;
	}
}
#endif