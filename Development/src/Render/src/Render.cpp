/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.6.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "RenderPrivate.h"

#include "VideoPlayer.h"
#include "PostEffects/PostEffects.h"
#include "SharedData.h"

using namespace Ogre;
#pragma message("NICK-REFACTOR IT 2015")

namespace vega
{

	//-------------------------------------------------------------------------------------
	Render::Render(CoreSystems* _manager)
		: mRoot(_manager->mGRoot)
		, mCamera(_manager->mGCamera)
		, mWindow(_manager->mGWindow)
		, mSceneMgr(_manager->mGSceneMgr)
		, mViewport(_manager->mGViewport)
		, mEngineConfig(_manager->mEngineConfig)
	{
		Debug("[render]Render::Render");

		mSystem = nullptr;
		externals = nullptr;
	}

	/**
	*/
	bool Render::PlayVideo(const char* _name) {
		if (!externals)
			return false;
		return externals->mVideoPlayer->playVideo(_name, true);
	}

	/**
	*/
	void Render::EnableDisableAA(bool _s) {
		/*int r = mEngineConfig->mAntiAlliasing;
		switch (r){
		case 1:
		PostEffectSetStatus("SMAA", _s);
		Debug("[AA]_EnableDisableAA SMAA-Render %i", _s);
		break;
		case 2:
		PostEffectSetStatus("SSAA", _s);
		Debug("[AA]_EnableDisableAA SSAA-Render %i", _s);
		break;
		case 3:
		PostEffectSetStatus("FXAA", _s);
		Debug("[AA]_EnableDisableAA FXAA-Render %i", _s);
		break;
		}*/
	}

	/**
	*/
	VideoPlayer* Render::GetPlayer()
	{
		if (!externals)
			return nullptr;

		return externals->mVideoPlayer;
	}

	/**
	*/
	void Render::_LoadShaders()
	{
		Debug("Render::_LoadShaders()");
		Ogre::ResourceGroupManager*ptr = Ogre::ResourceGroupManager::getSingletonPtr();
		if (!ptr)
		{
			ErrorFunction(true, "[Render::_LoadShaders]is not exist ResourceGroupManager!", __FILE__, __LINE__);
			return;
		}

		std::string path = "..//Engine//Shaders//";
		ptr->addResourceLocation(path, "FileSystem", "Shaders", true);
		path = "..//Engine//Shaders//programs//Cg//";
		ptr->addResourceLocation(path, "FileSystem", "Shaders", true);
		path = "..//Engine//Shaders//programs//GLSL//";
		ptr->addResourceLocation(path, "FileSystem", "Shaders", true);
		path = "..//Engine//Shaders//programs//GLSL150//";
		ptr->addResourceLocation(path, "FileSystem", "Shaders", true);
		path = "..//Engine//Shaders//programs//GLSL400//";
		ptr->addResourceLocation(path, "FileSystem", "Shaders", true);
		path = "..//Engine//Shaders//programs//GLSLES//";
		ptr->addResourceLocation(path, "FileSystem", "Shaders", true);
		path = "..//Engine//Shaders//programs//HLSL//";
		ptr->addResourceLocation(path, "FileSystem", "Shaders", true);
		path = "..//Engine//Shaders//programs//";
		ptr->addResourceLocation(path, "FileSystem", "Shaders", true);

		ptr->initialiseResourceGroup("Shaders");
	}

	/**
	*/
	void Render::Initialize()
	{
		Debug("Render::Initialize()");
		//Иницилизировали все дополнительные параметры,грузим ресурсы
		_LoadShaders();

		static bool firstStart = true;
		if (firstStart)
		{
			_InitPostEffects();
			{
				mSystem = new DeferredShadingSystem(mViewport, mSceneMgr, mCamera);
				shData = new SharedData();
				shData->iSystem = mSystem;
				mSystem->initialize();
				Debug("Activation DeferredShading");
				mSystem->setActive(true);

				// safely setup application's (not postfilter!) shared data
				shData->iCamera = mCamera;
				shData->iRoot = mRoot;
				shData->iWindow = mWindow;
				shData->iSceneMgr = mSceneMgr;
				shData->iActivate = true;
				shData->iGlobalActivate = true;
			}

			{
				//Создаем дополнительные модули в рендере
				externals = new Externals(this);
			}
			firstStart = false;
		}
	}

	/**
	*/
	void Render::Update(float _evt)
	{
		if (externals)
			externals->Update(_evt);
		shData->iLastFrameTime = _evt;

		if (shData->mMLAnimState)
			shData->mMLAnimState->addTime(_evt);
	}

	/**
	*/
	Render::~Render()
	{
		//TEST
		/*	SAFE_DELETE(externals);
			SAFE_DELETE(shData);
			SAFE_DELETE(mSystem);*/
	}

	/**
	*/
	void Render::_InitPostEffects()
	{
		/// Create a couple of hard coded postfilter effects as an example of how to do it but the preferred method is to use compositor scripts.
		_CreatePostEffects();
		_EnablePostEffects();
	}

	/**
	*/
	void Render::PostEffectSetStatus(const char* _name, bool _status) {
		if (strcmp(_name, "SSAO") == 0)
		{
			Debug("[PostEffects]Enabled SSAO");
			if (!mSystem)
			{
				ErrorF("Deferred Shading is not active");
			}
			mSystem->setSSAO(_status);
		}
		else
		{
			if (strcmp(_name, "HDR") == 0)
			{
				if (_status)
					CompositorManager::getSingleton().addCompositor(mViewport, _name, 0);
				else
					CompositorManager::getSingleton().removeCompositor(mViewport, _name);
				CompositorManager::getSingleton().setCompositorEnabled(mViewport, _name, _status);
			}
			else
			{
				if (_status)
					CompositorManager::getSingleton().addCompositor(mViewport, _name);
				else
					CompositorManager::getSingleton().removeCompositor(mViewport, _name);
				CompositorManager::getSingleton().setCompositorEnabled(mViewport, _name, _status);
			}
		}
	}

	/**
	*/
	void Render::_EnablePostEffects() {
		int r = 0;
		r = mEngineConfig->mSSAO;
		if (r)
			PostEffectSetStatus("SSAO", r);
		EnableDisableAA(true);
		r = mEngineConfig->mHDR;
		if (r)
			PostEffectSetStatus("HDR", r);
	}

	/**
	*/
	void Render::_CreatePostEffects()
	{
		Ogre::CompositorManager& compMgr = Ogre::CompositorManager::getSingleton();
		compMgr.registerCompositorLogic("GaussianBlur", new GaussianBlurLogic);
		compMgr.registerCompositorLogic("HDR", new HDRLogic);
		compMgr.registerCompositorLogic("HeatVision", new HeatVisionLogic);
	}
}