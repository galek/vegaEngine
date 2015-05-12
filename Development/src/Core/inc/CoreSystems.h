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

	class CORE_API CoreSystems:public SubSystemsManager
	{
	public:
		CoreSystems();
		~CoreSystems();
		
		void InitRenderer();
		//Sub-systems
		Config *mConfig;
		EngineConfig *mEngineConfig;
		FileSystem* mFS;
		CollisionTools* mRaycast;
		Scripting* mScript;
		enum EngineState{
			ES_BUILDING_GEOMETRY,
			ES_BUILDING_LIGHT,
			ES_BUILDING_PATHES,
			ES_LOADING,
			ES_PAUSE,
			ES_PLAY
		};
		EngineState mEngineState;
		void WriteInfoAboutBuild();
		virtual void Release();
	protected:
		void InitD3D9();
		void InitOGL();
		virtual void Initialize();
		void GetDataFromSettingsFile();
	private:
		ScriptSerializerManager*mShaderCache;
	};
}