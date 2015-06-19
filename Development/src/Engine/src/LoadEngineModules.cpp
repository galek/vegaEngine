/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"


//Modules
#include "../../BulletPhysics/inc/VBulletPhysics.h"
#include "../../AudioAL/inc/AudioAL.h"

namespace vega
{
	/**
	*/
	void EngineGlobals::LoadEngineModules()
	{
#ifdef ENGINE_DLL_LOADING_PLUGINS
		// a config file we'll use to retrieve some settings
		Ogre::ConfigFile cfgFile;
		cfgFile.load("..\\Engine\\Configs\\Modules.ini");

		Ogre::ConfigFile::SectionIterator seci = cfgFile.getSectionIterator();
		while (seci.hasMoreElements())
		{
			std::string secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				std::string name = i->first;
				std::string libraryName = i->second;

				const char* finalStr = libraryName.c_str();
				if (name == "Module")//Делаем обязательно проверку
					plugins->LoadEngineModule(finalStr);

			}
		}
#else
		physics = new VBulletPhysics(this);
#if ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
		audio = new SoundManager();
#endif
#endif
	}
}