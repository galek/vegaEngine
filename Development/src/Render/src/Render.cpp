/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "RenderPrivate.h"
#include "VideoPlayer.h"
#include "PostEffects/PostEffects.h"

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
		//Создаем дополнительные модули в рендере
		externals = new Externals(this);
	}

	/**
	*/
	bool Render::PlayVideo(std::string _name)	{
		return externals->mVideoPlayer->playVideo(_name, true);
	}

	/**
	*/
	void Render::EnableDisableAA(bool _s){
		int r = mEngineConfig->mAntiAlliasing;
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
		}
	}

	/**
	*/
	VideoPlayer* Render::GetPlayer() {
		return externals->mVideoPlayer;
	}

	/**
	*/
	void Render::_LoadShaders()
	{
		Debug("Render::_LoadShaders()");
		Ogre::ResourceGroupManager*ptr = Ogre::ResourceGroupManager::getSingletonPtr();
		if (!ptr)
			ErrorFunction(true, "[Render::_LoadShaders]is not exist ResourceGroupManager!", __FILE__, __LINE__);
		//Остальные ресурсы
		//Shaders Zip's
		Ogre::StringVectorPtr gameArchives = ptr->findResourceNames("Shaders", "*.shaders", false);
		for (unsigned int i = 0; i < gameArchives->size(); i++)
		{
			ptr->addResourceLocation("..\\Engine\\Shaders\\" + (*gameArchives)[i], "Zip", "Shaders", true);
			ptr->addResourceLocation("..\\Engine\\D3D11Shaders\\" + (*gameArchives)[i], "Zip", "Shaders", true);
		}
		ptr->addResourceLocation("..\\Engine\\D3D11Shaders\\", "FileSystem", "Shaders", true);

		ptr->initialiseResourceGroup("Shaders");

		//DEBUG
#ifdef _DEBUG
		gameArchives = Ogre::ResourceGroupManager::getSingleton().listResourceNames("Shaders");
		for (unsigned int i = 0; i < gameArchives->size(); i++)
			Debug("Shader File %s,number %i of shader archives", ((*gameArchives)[i]).c_str(), i);
#endif
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
				mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE);
				mSceneMgr->setShadowTextureCasterMaterial("Examples/Instancing/VTF/shadow_caster_dq_two_weights");

				mSceneMgr->setShadowTextureCount(1);
				mSceneMgr->setShadowFarDistance(mEngineConfig->mShadowFarDistance);
				mSceneMgr->setShadowDirectionalLightExtrusionDistance(mEngineConfig->mFarClipDistance);
			}
			firstStart = false;
		}
	}

	/**
	*/
	void Render::Update(float _evt)	{
		externals->Update(_evt);
	}

	/**
	*/
	Render::~Render()	{
	}

	/**
	*/
	void Render::_InitPostEffects()	{
		/// Create a couple of hard coded postfilter effects as an example of how to do it but the preferred method is to use compositor scripts.
		_CreatePostEffects();
		_EnablePostEffects();
	}

	/**
	*/
	void Render::PostEffectSetStatus(std::string _name, bool _status)	{
#ifdef __EXPEREMENTAL__
		if (_name == "SSAO"){
			mSystem->setSSAO(_status);
			Debug("[PostEffects]Enabled SSAO");
		}
		else
#endif
			if (_name == "HDR")
			{
				if (_status)
					CompositorManager::getSingleton().addCompositor(mViewport, _name, 0);
				else
					CompositorManager::getSingleton().removeCompositor(mViewport, _name);
				CompositorManager::getSingleton().setCompositorEnabled(mViewport, _name, _status);
			}
			else{
				if (_status)
					CompositorManager::getSingleton().addCompositor(mViewport, _name);
				else
					CompositorManager::getSingleton().removeCompositor(mViewport, _name);
				CompositorManager::getSingleton().setCompositorEnabled(mViewport, _name, _status);
			}
	}

	/**
	*/
	void Render::_EnablePostEffects()	{
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