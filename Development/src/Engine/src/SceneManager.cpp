/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "SceneManager.h"

#include "camera/CCSCameraControlSystem.h"
#include "camera/CCSBasicCameraModes.h"
#include "camera/CCSFreeCameraMode.h"
#include "camera/iCameraBase.h"
#include "HydraxRttListener.h"
#include "ActorDynamicSky.h"
#include "ActorWater.h"
using namespace Ogre;

namespace vega
{
	//-----------------------------------------------------------------------------
	SceneManager::SceneManager(EngineGlobals *_engine)
		:engine(_engine)
		/*, levelloader(nullptr)*/
		, mCameraCS(nullptr)
		, mCurrentCamera(nullptr)
		, mHRTTL(nullptr)
		, mWater(nullptr)
		, mDSky(nullptr)
	{
		/*levelloader = new LevelLoader();*/
		// Create the camera system using the previously created ogre camera.
		mHRTTL = new HydraxRttListener();
		mCameraCS = new CameraControlSystem(_engine->mGSceneMgr, "CameraControlSystem", _engine->mGCamera);
	}

	//-----------------------------------------------------------------------------
	SceneManager::~SceneManager() {
		CleanScene();
		SAFE_DELETE(mCameraCS);
		SAFE_DELETE(mHRTTL);
	}

	//-----------------------------------------------------------------------------
	void SceneManager::_UpdateCameraMove(float _time)
	{
		for (int i = 0; i < mSceneCameras.size(); i++) {
			if (mSceneCameras[i] == mCurrentCamera)
				mSceneCameras[i]->Update(_time);
		}
	}

	//-----------------------------------------------------------------------------
	void SceneManager::CleanScene()
	{
		if (engine->isEditor())
		{
			PrintfSceneContain();
		}

		if (mSceneActors.empty())
			return;

		for (auto it = mSceneActors.begin(); it != mSceneActors.end(); ++it)
		{
			SAFE_DELETE(*it);
		}
		mSceneActors.clear();

		engine->mGSceneMgr->CompleteClearScene();
		mHRTTL->SetNull();
	}

	//-----------------------------------------------------------------------------
	void SceneManager::PrintfSceneContain()
	{
		Debug("mSceneActors contains %i,list:", mSceneActors.size());
		for (auto it = mSceneActors.begin(); it != mSceneActors.end(); ++it)
			Debug("%s ", (*it)->getName().c_str());
	}
	//-----------------------------------------------------------------------------
	void SceneManager::LoadLevel(const char* _mFileName, bool _newLoading)
	{
		/*if (!levelloader)
			return;

			if (_newLoading)
			CleanScene();

			levelloader->Load(_mFileName, engine->mGWindow);*/
	}

	//-----------------------------------------------------------------------------
	void SceneManager::Update(float _time)
	{
		if (!mCameraCS)
			return;
		mCameraCS->update(_time);
		_UpdateCameraMove(_time);
		_UpdateSkyWater(_time);
	}

	//-----------------------------------------------------------------------------
	void SceneManager::ManualStop()
	{
		for (int i = 0; i < mSceneCameras.size(); i++) {
			if (mSceneCameras[i] == mCurrentCamera)
				mSceneCameras[i]->ManualStop();
		}
	}

	//-----------------------------------------------------------------------------
	void SceneManager::InjectMouseMove(const Ogre::Vector2& evt)
	{
		for (int i = 0; i < mSceneCameras.size(); i++) {
			if (mSceneCameras[i] == mCurrentCamera)
				mSceneCameras[i]->InjectMouseMove(evt);
		}
	}

	//-----------------------------------------------------------------------------
	void SceneManager::InjectMouseDown(int id)
	{
		for (int i = 0; i < mSceneCameras.size(); i++) {
			if (mSceneCameras[i] == mCurrentCamera)
				mSceneCameras[i]->InjectMouseDown(id);
		}
	}

	//-----------------------------------------------------------------------------
	void SceneManager::InjectMouseUp(int _id)
	{
		OIS::MouseButtonID id = (OIS::MouseButtonID)_id;
		for (int i = 0; i < mSceneCameras.size(); i++) {
			if (mSceneCameras[i] == mCurrentCamera)
				mSceneCameras[i]->InjectMouseUp(id);
		}
	}

	//-----------------------------------------------------------------------------
	void  SceneManager::InjectKeyDown(const int evt)
	{
		for (int i = 0; i < mSceneCameras.size(); i++) {
			if (mSceneCameras[i] == mCurrentCamera)
				mSceneCameras[i]->InjectKeyDown(evt);
		}
	}

	//-----------------------------------------------------------------------------
	void  SceneManager::InjectKeyUp(const int evt)
	{
		for (int i = 0; i < mSceneCameras.size(); i++) {
			if (mSceneCameras[i] == mCurrentCamera)
				mSceneCameras[i]->InjectKeyUp(evt);
		}
	}

	//-----------------------------------------------------------------------------
	void  SceneManager::SetAmbientLight(float r, float g, float b) {
		GetEngine()->mGSceneMgr->setAmbientLight(Ogre::ColourValue(r, g, b));
	}

	//-----------------------------------------------------------------------------
	void SceneManager::AddActorToList(Actor* _actor) {
		mSceneActors.push_back(_actor);
	}

	//-----------------------------------------------------------------------------
	void SceneManager::DeleteActorFromList(Actor*_actor) {

	}

	//-----------------------------------------------------------------------------
	void SceneManager::AddActorCameraToList(iCameraBase* _actor) {
		mSceneCameras.push_back(_actor);
	}

	//-----------------------------------------------------------------------------
	void SceneManager::SetActiveCamera(iCameraBase*_actor) {
		mCurrentCamera = _actor;//Just delete from list,without calling destructor of Actor
	}

	//-----------------------------------------------------------------------------
	void SceneManager::_ContainActorDynamicSky(ActorDynamicSky*_actor)
	{
		if (!mHRTTL)
			return;
		mDSky = _actor;
		mHRTTL->SetSky(_actor->GetSkyX());
	}

	//-----------------------------------------------------------------------------
	void SceneManager::_ContainActorWater(ActorWater*_actor)
	{
		if (!mHRTTL)
			return;
		mWater = _actor;
		mHRTTL->SetWater(_actor->GetHydrax());
		mWater->GetHydrax()->getRttManager()->addRttListener(mHRTTL);
	}

	//-----------------------------------------------------------------------------
	void SceneManager::_UpdateSkyWater(float _time) {
		if (mWater)
			mWater->Update(_time);
		if (mDSky)
			mDSky->Update(_time);
	}

	//-----------------------------------------------------------------------------
	void SceneManager::InitForSkyWaterDepends() {
		if (!mHRTTL)
			mHRTTL = new HydraxRttListener();
	}
}