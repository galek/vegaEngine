/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

//LWater
#include "../Hydrax/include/Hydrax.h"
#include "../Hydrax/include/Noise/FFT/FFT.h"
#include "../Hydrax/include/Noise/Perlin/Perlin.h"
#include "../Hydrax/include/Noise/Noise.h"
#include "../Hydrax/include/Modules/ProjectedGrid/ProjectedGrid.h"
#include "../Hydrax/include/Modules/RadialGrid/RadialGrid.h"
#include "../Hydrax/include/Modules/SimpleGrid/SimpleGrid.h"

namespace vega
{
	class HydraxRttListener : public RttManager::RttListener
	{
	public:
		HydraxRttListener(){}
		HydraxRttListener(class Hydrax*_mH, class SkyX*_mSkyX);
		~HydraxRttListener(){}
		void preRenderTargetUpdate(const RttManager::RttType& Rtt);
		void postRenderTargetUpdate(const RttManager::RttType& Rtt);
		void SetSky(SkyX*_mSkyX){ mSkyX = _mSkyX; };
		void SetWater(Hydrax*_mHydrax){ mHydrax = _mHydrax; }
	private:
		class Hydrax*mHydrax;
		class SkyX*mSkyX;
	};

}