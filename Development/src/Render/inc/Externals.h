/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once
#include "renderdllex.h"

namespace vega
{
	class WaterSystem;
	class RENDER_API Externals
	{
	public:
		explicit Externals(Render*_render);
		~Externals();

		// LWater
		WaterSystem* CreateWater(const std::string &_profile);

		void Update(float evt);

		WaterSystem*waterEx;
		class VideoPlayer* mVideoPlayer;
	private:
		Render*mRenderFactory;
	};
}