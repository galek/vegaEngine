/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "RenderPrivate.h"

#include "VideoPlayer.h"

namespace vega
{
	Externals::Externals(Render*_render)
		:waterEx(nullptr),
		mVideoPlayer(nullptr),
		mRenderFactory(_render)
	{
		mVideoPlayer = new VideoPlayer(_render);
	}

	Externals::~Externals()
	{
		SAFE_DELETE(waterEx);
		SAFE_DELETE(mVideoPlayer);
	}
		
	void Externals::Update(float evt)
	{
		if (mVideoPlayer)
			mVideoPlayer->Update();
	}

}