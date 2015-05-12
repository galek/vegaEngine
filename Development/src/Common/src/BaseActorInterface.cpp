/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
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
		mName("BaseActorInterface_%f", std::rand()),
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
	{}
	//-------------------------------------------------------------------------------------
	BaseActorInterface::~BaseActorInterface()
	{
		if (mNode)
			delete(mNode);
		
		if (mEntity)
			delete(mEntity);
	}
}