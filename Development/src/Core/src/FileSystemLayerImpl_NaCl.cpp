/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"

#if ENGINE_PLATFORM == ENGINE_PLATFORM_NACL

#include <sys/types.h>
#include <ppapi\cpp\file_system.h>
#include <ppapi\cpp\file_ref.h>
#include <ppapi\cpp\file_io.h>

namespace vega
{
    //---------------------------------------------------------------------
	void FileSystem::getConfigPaths()
	{
		// use application path as first config search path
		mConfigPaths.push_back("");
	}
    //---------------------------------------------------------------------
	void FileSystem::prepareUserHome(const std::string& subdir)
	{
	}
    //---------------------------------------------------------------------
	bool FileSystem::fileExists(const std::string& path) const
	{
        return false;
    }
}
#endif