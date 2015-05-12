/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"

#if ENGINE_PLATFORM == ENGINE_PLATFORM_LINUX
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

namespace vega
{
	/** Get actual file pointed to by symlink */
	const std::string resolveSymlink(const std::string& symlink)
	{
		ssize_t bufsize = 256;
		char* resolved = 0;
		do
		{
			char* buf = OGRE_ALLOC_T(char, bufsize, Ogre::MEMCATEGORY_GENERAL);
			ssize_t retval = readlink(symlink.c_str(), buf, bufsize);
			if (retval == -1)
			{
				OGRE_FREE(buf, Ogre::MEMCATEGORY_GENERAL);
				break;
			}

			if (retval < bufsize)
			{
				// operation was successful.
				// readlink does not guarantee to 0-terminate, so do this manually
				buf[retval] = '\0';
				resolved = buf;
			}
			else
			{
				// buffer was too small, grow buffer and try again
				OGRE_FREE(buf, Ogre::MEMCATEGORY_GENERAL);
				bufsize <<= 1;
			}
		} while (!resolved);

		if (resolved)
		{
			std::string result (resolved);
			OGRE_FREE(resolved, Ogre::MEMCATEGORY_GENERAL);
			return result;
		}
		else
			return "";
	}
	//---------------------------------------------------------------------
	void FileSystem::getConfigPaths()
	{
		// try to determine the application's path:
		// recent systems should provide the executable path via the /proc system
		std::string appPath = resolveSymlink("/proc/self/exe");
		if (appPath.empty())
		{
			// if /proc/self/exe isn't available, try it via the program's pid
			pid_t pid = getpid();
			char proc[64];
			int retval = snprintf(proc, sizeof(proc), "/proc/%llu/exe", (unsigned long long) pid);
			if (retval > 0 && retval < (long)sizeof(proc))
				appPath = resolveSymlink(proc);
		}

		if (!appPath.empty())
		{
			// we need to strip the executable name from the path
			std::string::size_type pos = appPath.rfind('/');
			if (pos != std::string::npos)
				appPath.erase(pos);
		}
		else
		{
			// couldn't find actual executable path, assume current working dir
			appPath = ".";
		}

		// use application path as first config search path
		mConfigPaths.push_back(appPath + '/');
		// then search inside ../share/OGRE
		mConfigPaths.push_back(appPath + "/../share/OGRE/");
		// then try system wide /etc
		mConfigPaths.push_back("/etc/OGRE/");
	}
	//---------------------------------------------------------------------
	void FileSystem::prepareUserHome(const wchar_t * subdir)
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
			// create an .ogre subdir
			mHomePath.append("/.ogre/");

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
	bool FileSystem::fileExists(const wchar_t * path) const
	{
		return access(path, R_OK) == 0;
	}
	//---------------------------------------------------------------------
	bool FileSystem::createDirectory(const wchar_t * path)
	{
		return !mkdir(path, 0755) || errno == EEXIST;
	}
}
#endif