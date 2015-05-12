/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#ifndef __FileSystem_H__
#define __FileSystem_H__

namespace vega
{
	struct EngineConfig;
	/** Implementation for the iFileSystem interface. */
	/** Implementation for the iFileSystem interface. */
	class FileSystem : public iFileSystem
	{
	public:
		/** Creates a concrete platform-dependent implementation of iFileSystem.
		@param
		A subdirectory inside the user's path to distinguish between
		different Ogre releases.
		*/
		FileSystem(EngineConfig*_config);


		const wchar_t* getConfigFilePath(const wchar_t* filename) const;

		FORCEINLINE const wchar_t* getWritablePath(const wchar_t* filename) const		{
			return (mHomePath + filename).c_str();
		}

		void setConfigPaths(const std::vector<std::wstring> &paths){
			mConfigPaths = paths;
		}

		void setHomePath(const wchar_t *path){
			mHomePath = path;
		}

		/** Create a directory */
		bool createDirectory(const wchar_t * name);

	private:
		std::vector<std::wstring> mConfigPaths;
		std::wstring mHomePath;

		/** Determine config search paths. */
		void getConfigPaths();

		/** Create an Ogre directory and the given subdir in the user's home. */
		void prepareUserHome(const wchar_t * subdir);

		/** Test if the given file exists. */
		bool fileExists(const wchar_t * path) const;
	private:
		EngineConfig*mConfig;
	};
}

#endif
