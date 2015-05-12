/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"


namespace vega
{
	const wchar_t*  FileSystem::getConfigFilePath(const wchar_t* _filename) const
	{
		// look for the requested file in several locations:

		// 1. in the writable path (so user can provide custom files)
		const wchar_t* path = getWritablePath(_filename);
		if (fileExists(path))
			return path;

		// 2. in the config file search paths
		for (size_t i = 0; i < mConfigPaths.size(); ++i)
		{
			path = (mConfigPaths[i] + _filename).c_str();
			if (fileExists(path))
				return path;
		}

		// 3. fallback to current working dir
		return _filename;
	}
	//---------------------------------------------------------------------

}