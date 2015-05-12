/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

namespace vega
{
	/** Provides methods to find out where the Ogre config files are stored
	    and where logs and settings files should be written to.
		@remarks
			In modern multi-user OS, a standard user account will often not
			have write access to the path where the SampleBrowser is stored.
			In order to still be able to store graphics settings and log
			output and for the user to overwrite the default Ogre config files, 
			this class tries to create a folder inside the user's home directory. 
			Specialised implementations for each individual platform must
			subclass this abstract interface.
	  */
	class iFileSystem
	{
	public:
		virtual ~iFileSystem() {}

		/** Search for the given config file in the user's home path. If it can't
		    be found there, the function falls back to the system-wide install
			path for Ogre config files. (Usually the same place where the
			SampleBrowser resides, or a special config path above that path.)
			@param
				The config file name (without path)
			@return
				The full path to the config file.
		 */
		virtual const wchar_t* getConfigFilePath(const wchar_t* filename) const = 0;

		/** Find a path where the given filename can be written to. This path 
			will usually be in the user's home directory. This function should
			be used for any output like logs and graphics settings.
			@param
				Name of the file.
			@return
				The full path to a writable location for the given filename.
		 */
		virtual const wchar_t* getWritablePath(const wchar_t* filename) const = 0;
	};
}