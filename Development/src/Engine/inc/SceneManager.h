/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

#ifndef _CameraControlSystem_H_
#include "camera\CCSCameraControlSystem.h"
#endif
#include "inc\VVector.h"

namespace vega
{
	struct Actor;
	class CameraControlSystem;
	class LevelLoader;
	class iCameraBase;
	class ActorDynamicSky;
	class ActorWater;
	class HydraxRttListener;
	struct EngineGlobals;
	/**
	*/
	class ENGINE_API SceneManager
	{
	public:
		SceneManager(EngineGlobals *_engine);
		~SceneManager();
		// !@Loading level,bool-new loading or extending exist
		void LoadLevel(const char* _mFileName,bool _newLoading);
		void CleanScene();
		void Update(float _time);
		void InjectMouseMove(const Ogre::Vector2 &_arg);
		void InjectMouseUp(const int id);
		void InjectMouseDown(const int  id);
		void InjectKeyDown(const int _arg );
		void InjectKeyUp(const int _arg);
		void ManualStop();
		inline CameraControlSystem*getCameraManager(){ return mCameraCS; }
		void SetAmbientLight(float r, float g, float b);
		void AddActorToList(Actor* _actor);
		void DeleteActorFromList(Actor*_actor);
		void DeleteActor(Actor*_actor);
		void AddActorCameraToList(iCameraBase* _actor);
		void DeleteCameraFromList(iCameraBase*_actor);
		void DeleteCameraActor(iCameraBase*_actor);
		void SetActiveCamera(iCameraBase*_actor);
		void _ContainActorDynamicSky(ActorDynamicSky*sky);
		__inline ActorDynamicSky*GetActorDynamicSkyFromContain(){ return mDSky; }
		void _ContainActorWater(ActorWater*sky);
		__inline ActorWater*GetActorWaterFromContain(){ return mWater; }
		__inline iCameraBase*GetActiveCamera(){ return mCurrentCamera; }
	private:
		void _CreateCameras();
		void _UpdateCameraMove(float _time);
		void _UpdateSkyWater(float _time);
		void _InitForSkyWaterDepends();
	private:
		LevelLoader* levelloader;
		CameraControlSystem* mCameraCS;
		iCameraBase*mCurrentCamera;
		ActorDynamicSky*mDSky;
		ActorWater*mWater;
		HydraxRttListener*mHRTTL;
		EngineGlobals *engine;
	private:
		VVector<Actor*>mSceneActors;
		VVector<iCameraBase*>mSceneCameras;
	};
}