/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.6.0
* Copyright (C) 2009-2015 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"
#include "Scripting.h"
#include "Common.h"
#include "BackgroundLoader.h"

namespace vega
{
	//-------------------------------------------------------------------------------------
	void ComputeBuildId();
	void DestroyAdditions();
	bool InitAdditions();
	//-------------------------------------------------------------------------------------
	CoreSystems::CoreSystems()
		: mFS(nullptr)
		, mConfig(nullptr)
		, mEngineConfig(nullptr)
		, mShaderCache(nullptr)
		, mRaycast(nullptr)
		, mScript(nullptr)
		, mResBL(nullptr)
		, mEngineState(ES_LOADING)
	{
		ComputeBuildId();
#ifndef _DEVELOP
		if (!InitAdditions())
			exit(0);
#else
		InitAdditions();
#endif
		PreInitialize();
	}

	//-------------------------------------------------------------------------------------
	void CoreSystems::_ParseConfig()
	{
		mEngineConfig->mGlobalIllumination = mConfig->GetValueAsBool("Render/R_GI", false);
		mEngineConfig->mSSAO = mConfig->GetValueAsBool("Render/R_SSAO", false);
		mEngineConfig->mHDR = mConfig->GetValueAsBool("Render/R_HDR", false);
		mEngineConfig->mDOF = mConfig->GetValueAsBool("Render/R_DOF", false);
		mEngineConfig->mAntiAlliasing = EngineConfig::AntiAlliasing(mConfig->GetValueAsInt("Render/R_AA"));
		mEngineConfig->mRenderAPI = EngineConfig::RenderAPI(mConfig->GetValueAsInt("Render/R_API"));
		mEngineConfig->mGodRays = mConfig->GetValueAsBool("Render/R_GodRays", false);
		mEngineConfig->mResolution = mConfig->GetValueAsString("Render/R_Resolution", "1024 x 768");
		mEngineConfig->mFullscreen = mConfig->GetValueAsString("Render/R_Fullscreen", "Yes");

		mEngineConfig->mPrecacheResources = mConfig->GetValueAsBool("Resources/PreCache", false);

		mEngineConfig->mShadowFarDistance = mConfig->GetValueAsFloat("Render/ShadowFarDistance", 150);
		mEngineConfig->mShadowTextureSize = mConfig->GetValueAsFloat("Render/ShadowTextureSize", 512);
		mEngineConfig->mFarClipDistance = mConfig->GetValueAsFloat("Render/FarClipDistance", 512);
		mEngineConfig->mCompany = mConfig->GetValueAsString("Licensing/CompanyName", "Vega Group");
		mEngineConfig->mProjectName = mConfig->GetValueAsString("Licensing/ProjectName", "DemoProject");
	}

	//-------------------------------------------------------------------------------------
	void CoreSystems::PreInitialize()
	{
		mConfig = new Config();

		mEngineConfig = new EngineConfig();

		mScript = new Scripting();
		GetDataFromSettingsFile();

		mFS = new FileSystem(mEngineConfig);
	}

	//-------------------------------------------------------------------------------------
	CoreSystems::~CoreSystems()	{
		Release();
	}

	//-------------------------------------------------------------------------------------
	void CoreSystems::Release(){
		LogPrintf("CoreSystems::Release");
		//SAFE_DELETE(mResBL);
		//SAFE_DELETE(mScript);
		SAFE_DELETE(mFS);
		SAFE_DELETE(mEngineConfig);
		SAFE_DELETE(mConfig);
		DestroyAdditions();
	}

	//-------------------------------------------------------------------------------------
	void CoreSystems::WriteInfoAboutBuild(){
		LogPrintf("--------------------------------------------------------------");
		LogPrintf("'VCore' build: %d, %s", build_id, build_date);
		LogPrintf("--------------------------------------------------------------");
	}

	//-------------------------------------------------------------------------------------
	void CoreSystems::InitD3D9(){
		Ogre::RenderSystem *renderSystem = mGRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem");
		renderSystem->setConfigOption("Allow DirectX9Ex", "Yes");
		renderSystem->setConfigOption("Full Screen", mEngineConfig->mFullscreen);
		renderSystem->setConfigOption("Video Mode", mEngineConfig->mResolution += " @ 32-bit colour");
		renderSystem->setConfigOption("Use Multihead", "Yes");
		renderSystem->setConfigOption("Backbuffer Count", "Auto");
		renderSystem->setConfigOption("FSAA", "0");
		renderSystem->setConfigOption("Resource Creation Policy", "Create on active device");
		renderSystem->setConfigOption("Multi device memory hint", "Auto hardware buffers management");
		renderSystem->setConfigOption("Fixed Pipeline Enabled", "Yes");
		renderSystem->setConfigOption("VSync", "No");
		renderSystem->setConfigOption("sRGB Gamma Conversion", "No");

		mGRoot->setRenderSystem(renderSystem);
	}

	//-------------------------------------------------------------------------------------
	void CoreSystems::InitOGL(){
		Ogre::RenderSystem *renderSystem = mGRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
		renderSystem->setConfigOption("Full Screen", mEngineConfig->mFullscreen);
		renderSystem->setConfigOption("Video Mode", mEngineConfig->mResolution += " @ 32-bit colour");
		renderSystem->setConfigOption("Fixed Pipeline Enabled", "No");
		renderSystem->setConfigOption("VSync", "No");
		renderSystem->setConfigOption("sRGB Gamma Conversion", "No");

		mGRoot->setRenderSystem(renderSystem);
	}

	//-------------------------------------------------------------------------------------
	void CoreSystems::InitRenderer(){
		if (mEngineConfig->mRenderAPI == EngineConfig::RENDER_OGL)
			InitOGL();
		else if (mEngineConfig->mRenderAPI == EngineConfig::RENDER_DX9)
			InitD3D9();
		else
		{
			ErrorF("Render not selected. Shutdown");
		}
	}

	//-------------------------------------------------------------------------------------
	void CoreSystems::BackgroundLoad() {
		static bool firstStart = true;
		if (firstStart)
		{
			mResBL = new ResourceGroupBackgroundLoader(this, 1);
			firstStart = false;
		}

		if (!mResBL)
		{
			ErrorF("mResBL is nullptr");
			return;
		}
		mResBL->Loading();
	}
}