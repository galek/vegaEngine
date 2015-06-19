/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.6.0
* Copyright (C) 2009-2015 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "IEngine.h"
#include "BaseActorInterface.h"

namespace vega
{
	//-------------------------------------------------------------------------------------
	BaseActorInterface::BaseActorInterface() :mNode(nullptr),
		mEntity(nullptr),
		mFileName("Error.mesh"),
		mMaterialName("DefMat"),
		mPosition(Ogre::Vector3::ZERO),
		mRotation(Ogre::Quaternion::ZERO),
		mScale(Ogre::Vector3(1.0f, 1.0f, 1.0f)),
		mSize(Ogre::Vector3(1.0f, 1.0f, 1.0f)),
		mMass(10.0f),
		mRadius(10.0f),
		mCollisionModel(ACTORMESH_COLLISION_NONE),
		mCollisionGroup(0),
		mCollisionMask(
		ACTORMESH_COLLISION_STATIC |
		ACTORMESH_COLLISION_PRBOX |
		ACTORMESH_COLLISION_PRSPHERE |
		ACTORMESH_COLLISION_PRCAPSULE |
		ACTORMESH_COLLISION_CONVEX |
		ACTORMESH_COLLISION_RAGGDOL |
		ACTORMESH_COLLISION_TRIANGLE)
	{
		mName = "BaseActorInterface_";
		// by a random integer in the range [0-10000].
		mName += std::to_string(std::rand() % 10000);
		m_actDesc = ActorDescription::AD_BASE;
	}

	//-------------------------------------------------------------------------------------
	BaseActorInterface::~BaseActorInterface()
	{
	}

	//-------------------------------------------------------------------------------------
	std::string BaseActorInterface::GetDescriptionAsString()
	{
		if (m_actDesc == ActorDescription::AD_BASE)
			return "AD_BASE";
		else if (m_actDesc == ActorDescription::AD_ACTORBASE)
			return "AD_ACTORBASE";
		else if (m_actDesc == ActorDescription::AD_LIGHT)
			return "AD_LIGHT";
		else if (m_actDesc == ActorDescription::AD_CAMERA)
			return "AD_CAMERA";
		else if (m_actDesc == ActorDescription::AD_MESH)
			return "AD_MESH";
		else if (m_actDesc == ActorDescription::AD_SKY)
			return "AD_SKY";
		else if (m_actDesc == ActorDescription::AD_VEGETATION)
			return "AD_VEGETATION";
		else if (m_actDesc == ActorDescription::AD_WATER)
			return "AD_WATER";
		else if (m_actDesc == ActorDescription::AD_EMITTER_SOUND)
			return "AD_EMITTER_SOUND";
		else if (m_actDesc == ActorDescription::AD_EMITTER_PARTICLE)
			return "AD_EMITTER_PARTICLE";

		return "AD_NUM";
	}
}