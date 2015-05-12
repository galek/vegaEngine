/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"

#if ENGINE_PLATFORM == ENGINE_PLATFORM_MAC
#include "macUtils.h"
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

namespace vega
{
	void FileSystem::getConfigPaths()
	{
		mConfigPaths.push_back(Ogre::macBundlePath() + "/Contents/Resources/");
		mConfigPaths.push_back(Ogre::macBundlePath() + "/");
	}
    //---------------------------------------------------------------------
	void FileSystem::prepareUserHome(const std::string& subdir)
	{
		struct passwd* pwd = getpwuid(getuid());
		if (pwd)
		{
			mHomePath = pwd->pw_dir;
		}
		else
		{
			// try the $HOME environment variable
			mHomePath = getenv("HOME");
		}

		if (!mHomePath.empty())
		{
			// create an Ogre subdir in application support
			mHomePath.append("/Library/Application Support/Ogre/");
			if (mkdir(mHomePath.c_str(), 0755) != 0 && errno != EEXIST)
			{
				// can't create dir
				mHomePath.clear();
			}
			else
			{
				// now create the given subdir
				mHomePath.append(subdir + '/');
				if (mkdir(mHomePath.c_str(), 0755) != 0 && errno != EEXIST)
				{
					// can't create dir
					mHomePath.clear();
				}
			}
		}

		if (mHomePath.empty())
		{
			// couldn't create dir in home directory, fall back to cwd
			mHomePath = "./";
		}
	}
    //---------------------------------------------------------------------
	bool FileSystem::fileExists(const std::string& path) const
	{
		return access(path.c_str(), R_OK) == 0;
	}
    //---------------------------------------------------------------------
	bool FileSystem::createDirectory(const std::string& path)
	{
		return !mkdir(path.c_str(), 0755) || errno == EEXIST;
	}
}
#endif