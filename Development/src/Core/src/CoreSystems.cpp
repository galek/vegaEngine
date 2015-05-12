/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"
#include "Scripting.h"
#include "Common.h"

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
		, mEngineState(ES_LOADING)
	{
		ComputeBuildId();
#ifndef _DEVELOP
			if (!InitAdditions())
				exit(0);
#else
			InitAdditions();
#endif
		Initialize();
	}
	//-------------------------------------------------------------------------------------
	void CoreSystems::GetDataFromSettingsFile()
	{
		mEngineConfig->mGlobalIllumination = mConfig->getValueAsBool("Render/R_GI", false);
		mEngineConfig->mSSAO = mConfig->getValueAsBool("Render/R_SSAO", false);
		mEngineConfig->mHDR = mConfig->getValueAsBool("Render/R_HDR", false);
		mEngineConfig->mDOF = mConfig->getValueAsBool("Render/R_DOF", false);
		mEngineConfig->mAntiAlliasing = EngineConfig::AntiAlliasing(mConfig->getValueAsInt("Render/R_AA"));
		mEngineConfig->mRenderAPI = EngineConfig::RenderAPI(mConfig->getValueAsInt("Render/R_API"));
		mEngineConfig->mGodRays = mConfig->getValueAsBool("Render/R_GodRays", false);
		mEngineConfig->mResolution = mConfig->getValueAsString("Render/R_Resolution", "1024 x 768");
		mEngineConfig->mFullscreen = mConfig->getValueAsString("Render/R_Fullscreen", "Yes");

		mEngineConfig->mPrecacheResources = mConfig->getValueAsBool("Resources/PreCache", false);

		mEngineConfig->mShadowFarDistance = mConfig->getValueAsFloat("Render/ShadowFarDistance", 150);
		mEngineConfig->mShadowTextureSize = mConfig->getValueAsFloat("Render/ShadowTextureSize", 512);
		mEngineConfig->mFarClipDistance = mConfig->getValueAsFloat("Render/FarClipDistance", 512);
		mEngineConfig->mCompany = mConfig->getValueAsString("Licensing/CompanyName", "Vega Group");
		mEngineConfig->mProjectName = mConfig->getValueAsString("Licensing/ProjectName", "DemoProject");
	}
	//-------------------------------------------------------------------------------------
	void CoreSystems::Initialize()
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
		SAFE_DELETE(mFS);
//crash in editor		SAFE_DELETE(mScript);
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
		renderSystem->setConfigOption("Full Screen", mEngineConfig->mFullscreen);
		renderSystem->setConfigOption("Video Mode", mEngineConfig->mResolution += " @ 32-bit colour");
		renderSystem->setConfigOption("FSAA", "0");
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
		renderSystem->setConfigOption("Fixed Pipeline Enabled", "Yes");
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
		else{
			SeriousWarning(true, "Render not selected. Shutdown");
			exit(0);
		}
	}	
}