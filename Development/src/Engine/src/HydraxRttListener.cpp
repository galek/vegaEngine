/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.6.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"

#include "HydraxRttListener.h"
//SkyX
#include "../SkyX/SkyX.h"

namespace vega
{
	HydraxRttListener::HydraxRttListener()
	{
		mSkyX = nullptr;
		mHydrax = nullptr;
	}
	//----------------------------------------------------------------------------

	HydraxRttListener::~HydraxRttListener()
	{
		SAFE_DELETE(mSkyX);
		SAFE_DELETE(mHydrax);
	}

	//----------------------------------------------------------------------------
	HydraxRttListener::HydraxRttListener(class Hydrax*_mH, class SkyX*_mSkyX)
		:mSkyX(_mSkyX),
		mHydrax(_mH)
	{
	}

	//----------------------------------------------------------------------------
	void HydraxRttListener::preRenderTargetUpdate(const RttManager::RttType& Rtt)
	{
		TODO("UNDERWATER Post process");
		// If needed in any case...
		//bool underwater = mHydrax->_isCurrentFrameUnderwater();

		if (!mSkyX)
			return;

		switch (Rtt)
		{
		case RttManager::RTT_REFLECTION:
			mSkyX->setStarfieldEnabled(false);// No stars in the reflection map
			break;

		case RttManager::RTT_REFRACTION:break;

		case RttManager::RTT_DEPTH: case RttManager::RTT_DEPTH_REFLECTION:
			// Hide SkyX components in depth maps
			mSkyX->getMeshManager()->getEntity()->setVisible(false);
			mSkyX->getMoonManager()->getMoonBillboard()->setVisible(false);
			break;
		}
	}

	//----------------------------------------------------------------------------
	void HydraxRttListener::postRenderTargetUpdate(const RttManager::RttType& Rtt)
	{
		bool underwater = mHydrax->_isCurrentFrameUnderwater();

		if (!mSkyX)
			return;

		switch (Rtt)
		{
		case RttManager::RTT_REFLECTION:
			mSkyX->setStarfieldEnabled(true);
			break;

		case RttManager::RTT_REFRACTION:
			break;

		case RttManager::RTT_DEPTH:
		case RttManager::RTT_DEPTH_REFLECTION:
			mSkyX->getMeshManager()->getEntity()->setVisible(true);
			mSkyX->getMoonManager()->getMoonBillboard()->setVisible(true);
			break;
		}
	}
	
	//----------------------------------------------------------------------------
	void HydraxRttListener::SetNull()
	{
		mSkyX = nullptr;
		mHydrax = nullptr;
	}
}