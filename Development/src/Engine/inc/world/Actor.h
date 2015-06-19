/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#ifndef ACTOR_H
#define ACTOR_H


namespace vega
{
	struct ENGINE_API Actor :public BaseActorInterface
	{
		Actor();
		virtual ~Actor();

		// !@Get Entity Name
		std::string getName();
		// !@Get File Name
		std::string getFileName();
		// !@Set Position of Actor
		void setPosition(const Ogre::Vector3& _pos);
		// !@Set Position of Actor
		void setPosition(float _x, float _y, float _z);
		// !@Get Position of Actor
		Ogre::Vector3& getPosition();
		// !@Set Material
		void setMaterialName(const char* _material);
		// !@Set Orintation
		void setOrientation(const Ogre::Quaternion& _quat);
		// !@Set Scale
		void setScale(const Ogre::Vector3& _scale);
		// !@Set Scale
		void setScale(float _x, float _y, float _z);
		// !@Set Casting Shadows
		void setCastShadows(bool _status);
		// !@Set Linear Velocity
		void setLinearVelocity(const Ogre::Vector3 &_mLinearVelocity);
		// !@Set Linear Velocity
		void setLinearVelocity(float _x, float _y, float _z);
		// !@Set Mass
		void setMass(float _mass);
		// !@Set Collision Model
		void setCollisionModel(int _mCollisionModel);
		// !@Calculate real mesh size using AxisAlignedBox(AABB)
		void calculateSizeUsingAxisAlignedBox();
		// !@Set Yaw Angle
		void setYaw(const Ogre::Radian& _rot);

		// !@Get SceneNode
		EFORCEINLINE Ogre::SceneNode* const getNode() {
			return mNode;
		}
		// !@Get Entity
		EFORCEINLINE Ogre::Entity* const getEntity() {
			return mEntity;
		}

		// !@Showing Bounding Box
		EFORCEINLINE void ShowBoundingBox(bool _value) { if (!mNode) return; mNode->showBoundingBox(_value); }
	protected:
		// !@CreateNode
		void _createNode(Ogre::SceneNode* _parentNode = NULL, Ogre::Vector3 _mPosition = Ogre::Vector3(0, 0, 0), Ogre::Quaternion _mRotation = Ogre::Quaternion(1, 0, 0, 0), Ogre::Vector3 _mScale = Ogre::Vector3(1, 1, 1));
		// !@_switchCollision
		void _switchCollision();

		void destroyAllAttachedMovableObjects(Ogre::SceneNode* node);

		void destroySceneNode(Ogre::SceneNode* node);
	protected:
		Ogre::StaticGeometry *mStaticGeometry;
		IBody*phBody;
	};
}
#endif//ACTOR_H