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

	/**
	*/
	Config::Config(const char* _filename)
	{
		m_ConfigFile.load(_filename, "=", true);

		Ogre::ConfigFile::SectionIterator seci = m_ConfigFile.getSectionIterator();
		std::string sectionName;
		std::string keyName;
		std::string valueName;

		while (seci.hasMoreElements())
		{
			sectionName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				keyName = i->first;
				valueName = i->second;
				m_Configs.insert(std::pair<std::string, std::string>(sectionName + "/" + keyName, valueName));
			}
		}

	}

	/**
	*/
	bool Config::GetKeyExists(const char* key) const
	{
		if (m_Configs.count(key) > 0)
		{
			return true;
		}
		return false;
	}
	/**
	*/
	const std::string& Config::GetValueAsString(const char* key, const std::string &_default)
	{
		if (GetKeyExists(key) == true)
			return m_Configs[key];
		else{
			std::string x = ("Configuration key: ");
			x += key;
			x += " not found";
			x += "Config::getValue";
			//Logger currently not initilaised
			ErrorFunction(false, x.c_str(), __FILE__, __LINE__);
			return _default;
		}


	}
	/**
	*/
	int Config::GetValueAsInt(const char* key)	{
		if (!GetValueAsString(key).empty())
			return atoi(GetValueAsString(key).c_str());
		else return 0;
	}
	/**
	*/
	float Config::GetValueAsFloat(const char* key, float _default)	{
		return Ogre::StringConverter::parseReal(GetValueAsString(key), _default);
	}
	/**
	*/
	bool Config::GetValueAsBool(const char* key, bool _default)	{
		return Ogre::StringConverter::parseBool(GetValueAsString(key), _default);
	}
	/**
	*/
	Ogre::Vector3 Config::GetValueAsVector3(const char* key, const Ogre::Vector3 &_default) {
		return Ogre::StringConverter::parseVector3(GetValueAsString(key), _default);
	}
	/**
	*/
	Ogre::ColourValue Config::GetValueAsColourValue(const char* key, const Ogre::ColourValue &_default) {
		return Ogre::StringConverter::parseColourValue(GetValueAsString(key), _default);
	}
}