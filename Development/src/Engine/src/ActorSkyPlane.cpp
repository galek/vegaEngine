/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "ActorSkyPlane.h"

namespace vega
{
	/**
	*/
	ActorSkyPlane::ActorSkyPlane(std::string  _material)
	{
		mName = "ActorSkyPlane_" + _material;//for Actor Base
		m_actDesc = ActorDescription::AD_SKY;

		if (!GetEngine()->mGSceneMgr->isSkyPlaneEnabled())
			GetEngine()->mGSceneMgr->setSkyPlane(true, Ogre::Plane(0, -1, 0, 5000), _material);
	}
	/**
	*/
	ActorSkyPlane::ActorSkyPlane(std::string  _material, const Ogre::Vector4 &_vec)
	{
		mName = "ActorSkyPlane_" + _material;//for Actor Base
		m_actDesc = ActorDescription::AD_SKY;

		if (!GetEngine()->mGSceneMgr->isSkyPlaneEnabled())
			GetEngine()->mGSceneMgr->setSkyPlane(true, Ogre::Plane(_vec.x, _vec.y, _vec.z, _vec.w), _material);
	}
	/**
	*/
	ActorSkyPlane::ActorSkyPlane(std::string  _material, float _a, float _b, float _c, float _d)
	{
		mName = "ActorSkyPlane_" + _material;//for Actor Base
		m_actDesc = ActorDescription::AD_SKY;

		if (!GetEngine()->mGSceneMgr->isSkyPlaneEnabled())
			GetEngine()->mGSceneMgr->setSkyPlane(true, Ogre::Plane(_a, _b, _c, _d), _material);
	}

	/**
	*/
	ActorSkyPlane::~ActorSkyPlane()
	{
		if (GetEngine()->mGSceneMgr->isSkyPlaneEnabled())
			GetEngine()->mGSceneMgr->destroySceneNode(GetEngine()->mGSceneMgr->getSkyPlaneNode());
	}
}