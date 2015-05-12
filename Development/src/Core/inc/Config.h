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
	class CORE_API Config
	{
	public:
		//various getters used by class clients to get configuration directives
		int getValueAsInt(const char* key);
		float getValueAsFloat(const char* key,float _default=0);
		std::string getValueAsString(const char* key,std::string _default="");
		Ogre::Vector3 getValueAsVector3(const char* key,Ogre::Vector3 _default=Ogre::Vector3::ZERO);
		bool getValueAsBool(const char* key,bool _default=false);
		Ogre::ColourValue getValueAsColourValue(const char* key,Ogre::ColourValue _default=Ogre::ColourValue::ZERO);
		bool getKeyExists(const char* key);

		Config(const char* _filename="..\\Engine\\Configs\\BaseEngine.ini");
		~Config();
		

	private:
		Ogre::ConfigFile m_ConfigFile;
		//this is where our configuration data is stored
		std::map<std::string, std::string> m_Configs;
	};
}