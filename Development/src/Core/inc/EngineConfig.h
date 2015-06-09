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
	struct EngineConfig
	{
		EngineConfig() :mGlobalIllumination(false), mSSAO(false), mHDR(false), mDOF(false),
			mGodRays(false), mPrecacheResources(false),
			mShadowFarDistance(100), mShadowTextureSize(100),
			mFarClipDistance(100),
			mAntiAlliasing(AA_NONE), 
			mRenderAPI(RenderAPI::RENDER_NONE),
			mResolution("1024 x 768"),
			mFullscreen("No"),
			mCompany("Vega Group"), mProjectName("DemoProject")
		{}

		//Resolution of Render
		std::string mResolution, mFullscreen;

		//Render
		bool mGlobalIllumination,mSSAO,mHDR,mDOF,mGodRays;

		float mShadowFarDistance, mShadowTextureSize,mFarClipDistance;
		//Core
		bool mPrecacheResources;
		std::string mCompany, mProjectName;

		enum AntiAlliasing
		{
			AA_NONE = 0,
			AA_SMAA = 1,
			AA_SSAA = 2,
			AA_FXAA = 3
		};
		AntiAlliasing mAntiAlliasing;

		enum class RenderAPI
		{
			RENDER_NONE = -1,
			RENDER_OGL,
			RENDER_DX9,
			RENDER_DX11,
			RENDER_COUNT
		};
		RenderAPI mRenderAPI;
	};
}