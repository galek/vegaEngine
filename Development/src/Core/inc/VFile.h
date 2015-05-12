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
	class CORE_API VFile
	{
	public:
		/*
		**/
		VFile(){};
		/*
		**/
		~VFile(){};
		/*
		**/
		const char* LoadFileHowCChar(const char *_mFilename);
		/*
		**/
		std::string LoadFileHowString(const char *_mFilename);
		/*
		**/
		size_t LoadFileAndGetSize(const char *_mFilename);
		/*
		**/
		const char* GetHowCChar();
		/*
		**/
		size_t GetSize();
	private:
		Ogre::DataStreamPtr pStream;
	};
}