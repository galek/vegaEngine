/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.6.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "SceneManager.h"
#include "ActorDynamicSky.h"
#include "../SkyX/SkyX.h"

namespace vega
{
	//-------------------------------------------------------------------------------------
	ActorDynamicSky::ActorDynamicSky()
		:mSkyX(nullptr), mBC(nullptr)
	{
		mName = "ActorDynamicSky";
		m_actDesc = ActorDescription::AD_SKY;

		mBC = new BasicController();
		mSkyX = new SkyX(GetEngine()->mGSceneMgr, GetEngine()->mGCamera, GetEngine()->mGRoot, GetEngine()->mGWindow, mBC);
		
		mSkyX->create();
		AddCloudLayer();
		
		auto ptr = GetEngine()->GetSceneMgr();
		ptr->InitForSkyWaterDepends();
		ptr->_ContainActorDynamicSky(this);
	}
	//-------------------------------------------------------------------------------------
	ActorDynamicSky::~ActorDynamicSky()	{
		mSkyX->remove();
		SAFE_DELETE(mSkyX);//mBC удалять не надо,т.к она удалеятся в mSkyX
	}
	//-------------------------------------------------------------------------------------
	void ActorDynamicSky::Update(float evt)
	{
		if (!isValid(__FUNCTION__))
		{
			return;
		}
		mSkyX->update(evt);
	}
	//-------------------------------------------------------------------------------------
	void ActorDynamicSky::AddCloudLayer()
	{
		if (!isValid(__FUNCTION__))
		{
			return;
		}
		mSkyX->getCloudsManager()->add(CloudLayer::Options());
	}

	//-------------------------------------------------------------------------------------
	bool ActorDynamicSky::isValid(const char* _funct) const
	{
		if (!mSkyX)
		{
			Warning("mSkyX is null,in function:%s", _funct);
			return false;
		}
		return true;
	}
}