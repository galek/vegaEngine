/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"
#include "macUtils.h"
#import <Foundation/NSPathUtilities.h>

namespace vega
{
	void FileSystem::getConfigPaths()
	{
		mConfigPaths.push_back(Ogre::macBundlePath() + "/../Documents/");
		mConfigPaths.push_back(Ogre::macBundlePath() + "/");
	}
    //---------------------------------------------------------------------
	void FileSystem::prepareUserHome(const std::string& subdir)
	{
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);

        mHomePath = std::string([[paths objectAtIndex:0] cStringUsingEncoding:NSASCIIStringEncoding]) + "/";
	}
    //---------------------------------------------------------------------
    bool FileSystem::fileExists(const std::string& path) const
	{
		return access(path.c_str(), R_OK) == 0;
	}
    //---------------------------------------------------------------------
	bool FileSystem::createDirectory(const std::string& path)
	{
		return false;
	}
}
