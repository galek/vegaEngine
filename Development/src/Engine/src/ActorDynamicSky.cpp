/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
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
		mBC = new BasicController();
		mSkyX = new SkyX(GetEngine()->mGSceneMgr, GetEngine()->mGCamera, GetEngine()->mGRoot, GetEngine()->mGWindow, mBC);
		mSkyX->create();
		AddCloudLayer();

		GetEngine()->GetSceneMgr()->_ContainActorDynamicSky(this);
	}
	//-------------------------------------------------------------------------------------
	ActorDynamicSky::~ActorDynamicSky()	{
		SAFE_DELETE(mSkyX);//mBc удалять не надо,т.к она удалеятся в mSkyX
	}
	//-------------------------------------------------------------------------------------
	void ActorDynamicSky::Update(float evt){
		mSkyX->update(evt);
	}
	//-------------------------------------------------------------------------------------
	void ActorDynamicSky::AddCloudLayer(){
		mSkyX->getCloudsManager()->add(CloudLayer::Options());
	}
}