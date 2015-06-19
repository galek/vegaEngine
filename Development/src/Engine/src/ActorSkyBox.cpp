/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "ActorSkyBox.h"

namespace vega
{
	/**
	*/
	ActorSkyBox::ActorSkyBox(std::string _material)
	{
		mName = "ActorSkyBox_" + _material;//for Actor Base
		m_actDesc = ActorDescription::AD_SKY;

		if (!GetEngine()->mGSceneMgr->isSkyBoxEnabled())
			GetEngine()->mGSceneMgr->setSkyBox(true, _material, 500);
	}
	/**
	*/
	ActorSkyBox::~ActorSkyBox()
	{
		if (GetEngine()->mGSceneMgr->isSkyBoxEnabled())
			GetEngine()->mGSceneMgr->destroySceneNode(GetEngine()->mGSceneMgr->getSkyBoxNode());
	}
}