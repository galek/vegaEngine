/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

#include <random>

namespace vega
{
	struct BaseActorInterface
	{
		BaseActorInterface();
		~BaseActorInterface();

		enum class ActorDescription
		{
			AD_BASE = 0,
			AD_ACTORBASE,
			AD_LIGHT,
			AD_CAMERA,
			AD_MESH,
			AD_SKY,
			AD_VEGETATION,
			AD_WATER,
			AD_EMITTER_SOUND,
			AD_EMITTER_PARTICLE,
			AD_NUM
		};
		ActorDescription m_actDesc;

		// !@Got Description as string
		std::string GetDescriptionAsString();
		EFORCEINLINE ActorDescription GetDescriptionAsEnum() { return m_actDesc; }
		
		// !@Entity Node
		Ogre::SceneNode* mNode;
		// !@Entity
		Ogre::Entity* mEntity;
		// !@Mesh Position
		Ogre::Vector3 mPosition;
		// !@Mesh Rotation
		Ogre::Quaternion mRotation;
		// !@Mesh Scale
		Ogre::Vector3 mScale;
		// !@Linear Velocity
		Ogre::Vector3 mLinearVelocity;
		// !@Mesh Size
		Ogre::Vector3 mSize;
		// !@Actor Mass
		float mMass;
		// !@Radius for Capsule,Sphere collisions
		float mRadius;
		// !@Collision Models
		enum CollisionModel
		{
			ACTORMESH_COLLISION_NONE = 0,
			ACTORMESH_COLLISION_STATIC,//1
			ACTORMESH_COLLISION_PRBOX,//2
			ACTORMESH_COLLISION_PRSPHERE,//3
			ACTORMESH_COLLISION_PRCAPSULE,//4
			ACTORMESH_COLLISION_CONVEX,//5
			ACTORMESH_COLLISION_RAGGDOL,//6
			ACTORMESH_COLLISION_TRIANGLE,//7
			ACTORMESH_COUNT,//8

		};
		CollisionModel mCollisionModel;

		// !@Entity Name
		std::string mName;
		// !@Mesh Filename for loading
		std::string mFileName;
		// !@Material Name
		std::string mMaterialName;
		short mCollisionGroup;
		short mCollisionMask;
	};
};