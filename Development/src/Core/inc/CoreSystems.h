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
	class CollisionTools;
	class Config;
	struct EngineConfig;
	class FileSystem;
	class ScriptSerializerManager;
	class Scripting;
	class ResourceGroupBackgroundLoader;

	class CORE_API CoreSystems:public SubSystemsManager
	{
	public:
		CoreSystems();
		virtual ~CoreSystems();
	protected:
		void Release();
		void WriteInfoAboutBuild();
		void InitRenderer();
		void BackgroundLoadProgress();
	public:
		//Sub-systems
		Config *mConfig;
		EngineConfig *mEngineConfig;
		FileSystem* mFS;
		CollisionTools* mRaycast;
		Scripting* mScript;
		ResourceGroupBackgroundLoader* mResBL;

		enum class EngineState
		{
			ES_BUILDING_GEOMETRY,
			ES_BUILDING_LIGHT,
			ES_BUILDING_PATHES,
			ES_LOADING,
			ES_PAUSE,
			ES_PLAY,
			ES_COUNT
		};
		EngineState mEngineState;
	protected:
		void InitD3D9();
		void InitOGL();
		/*
		Critical core systems
		Not depend from Ogre::Root
		*/
		void PreInitialize();
		EFORCEINLINE void GetDataFromSettingsFile() { _ParseConfig(); }
	private:
		void _ParseConfig();
	private:
		ScriptSerializerManager*mShaderCache;
	};
}