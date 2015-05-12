/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"

#if ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
#include <shlobj.h>
#include <io.h>

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
		// try to determine the application's path
		DWORD bufsize = 256;
		wchar_t* resolved = 0;
		do
		{
			wchar_t* buf = OGRE_ALLOC_T(wchar_t, bufsize, Ogre::MEMCATEGORY_GENERAL);
			DWORD retval = GetModuleFileNameW(NULL, buf, bufsize);
			if (retval == 0)
			{
				// failed
				OGRE_FREE(buf, Ogre::MEMCATEGORY_GENERAL);
				break;
			}

			if (retval < bufsize)
				resolved = buf;// operation was successful.
			else
			{
				// buffer was too small, grow buffer and try again
				OGRE_FREE(buf, Ogre::MEMCATEGORY_GENERAL);
				bufsize <<= 1;
			}
		} while (!resolved);

		std::wstring appPath = resolved;
		if (resolved)
			OGRE_FREE(resolved, Ogre::MEMCATEGORY_GENERAL);
		if (!appPath.empty())
		{
			// need to strip the application filename from the path
			std::wstring::size_type pos = appPath.rfind(L"\\");
			if (pos != std::wstring::npos)
				appPath.erase(pos);
		}
		else			
			appPath = L".";// fall back to current working dir
		

		// use application path as config search path
		mConfigPaths.push_back(appPath + L"\\");
	}
	//---------------------------------------------------------------------
	void FileSystem::prepareUserHome(const wchar_t* subdir)
	{
		wchar_t path[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PERSONAL|CSIDL_FLAG_CREATE, NULL, 0, path)))
		{
			mHomePath=path;
			// create Ogre subdir
			mHomePath += L"\\" + ToStrW(mConfig->mCompany.c_str()) + L"\\";
			if (! CreateDirectoryW(mHomePath.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
				mHomePath.clear();// couldn't create directory, fall back to current working dir

			else
			{
				mHomePath += subdir;
				mHomePath += L"\\";
				// create release subdir
				if (! CreateDirectoryW(mHomePath.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
					mHomePath.clear();// couldn't create directory, fall back to current working dir
			}
		}

		if (mHomePath.empty())
			mHomePath = L"Empty";// couldn't create dir in home directory, fall back to cwd

	}
	//---------------------------------------------------------------------
	bool FileSystem::fileExists(const wchar_t* path) const	{
		return _waccess(path, 00) == 0;
	}
	//---------------------------------------------------------------------
	bool FileSystem::createDirectory(const wchar_t* path)	{
		return CreateDirectoryW(path, NULL) != 0 || 
			GetLastError() == ERROR_ALREADY_EXISTS;
	}
}
#endif