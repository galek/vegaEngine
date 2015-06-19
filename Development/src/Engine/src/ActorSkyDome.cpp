/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "ActorSkyDome.h"

namespace vega
{
	/**
	*/
	ActorSkyDome::ActorSkyDome(std::string  _material)
	{
		mName = "ActorSkyDome_" + _material;//for Actor Base
		m_actDesc = ActorDescription::AD_SKY;

		if(!GetEngine()->mGSceneMgr->isSkyDomeEnabled())
			GetEngine()->mGSceneMgr->setSkyDome(true, _material);
	}
	/**
	*/
	ActorSkyDome::~ActorSkyDome()
	{
		if(GetEngine()->mGSceneMgr->isSkyDomeEnabled())
			GetEngine()->mGSceneMgr->destroySceneNode(GetEngine()->mGSceneMgr->getSkyDomeNode());
	}
}