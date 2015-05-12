/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"


namespace vega
{
	//-------------------------------------------------------------------------------------
	Actor::Actor()
		:mStaticGeometry(nullptr),
		phBody(nullptr)
	{
		_switchCollision();
		GetEngine()->GetSceneMgr()->AddActorToList(this);
	}
	//-------------------------------------------------------------------------------------
	Actor::~Actor()
	{
		GetEngine()->GetSceneMgr()->DeleteActorFromList(this);
		if (mStaticGeometry)
			GetEngine()->mGSceneMgr->destroyStaticGeometry(mStaticGeometry);
	}
	//-------------------------------------------------------------------------------------
	void Actor::_createNode(Ogre::SceneNode* _parentNode, Ogre::Vector3 _mPosition, Ogre::Quaternion _mRotation, Ogre::Vector3 _mScale)
	{
		mPosition = _mPosition;
		mRotation = _mRotation;
		mScale = _mScale;

		if (_parentNode == NULL)
			mNode = GetEngine()->mGSceneMgr->getRootSceneNode()->createChildSceneNode(mName + "_Node", mPosition, mRotation);
		else
			mNode = _parentNode->createChildSceneNode(mName + "_Node", mPosition, mRotation);

		mNode->setScale(mScale);
		mNode->rotate(_mRotation);
		mNode->setInitialState();
	}
	//-------------------------------------------------------------------------------------
	void Actor::setPosition(const Ogre::Vector3& _pos)
	{
		mPosition = _pos;
		mNode->setPosition(mPosition);
		mNode->setInitialState();
	}
	//-------------------------------------------------------------------------------------
	void Actor::setPosition(float _x, float _y, float _z)	{
		setPosition(Ogre::Vector3(_x, _y, _z));
	}
	//-------------------------------------------------------------------------------------
	void Actor::setScale(float _x, float _y, float _z)	{
		setScale(Ogre::Vector3(_x, _y, _z));
	}
	//-------------------------------------------------------------------------------------
	void Actor::setScale(const Ogre::Vector3& _scale)
	{
		mScale = _scale;
		mNode->setScale(mScale);
	}
	//-------------------------------------------------------------------------------------
	void Actor::setYaw(const Ogre::Radian& _rot)	{
		mNode->yaw(_rot);
	}
	//-------------------------------------------------------------------------------------
	void Actor::setOrientation(const Ogre::Quaternion& _quat)
	{
		mRotation = _quat;
		mNode->setOrientation(mRotation);
	}
	//-------------------------------------------------------------------------------------
	void Actor::setCastShadows(bool _status)	{
		mEntity->setCastShadows(_status);
	}
	//-------------------------------------------------------------------------------------
	Ogre::Vector3& Actor::getPosition()
	{
		return mPosition;
	}
	//-------------------------------------------------------------------------------------
	std::string Actor::getName(){
		return mName;
	}
	//-------------------------------------------------------------------------------------
	std::string Actor::getFileName(){
		return mFileName;
	}
	//-------------------------------------------------------------------------------------
	void Actor::calculateSizeUsingAxisAlignedBox()
	{
		const Ogre::AxisAlignedBox& aab = mEntity->getBoundingBox();
		Ogre::Vector3 min = aab.getMinimum()*mScale;
		Ogre::Vector3 max = aab.getMaximum()*mScale;
		float paddingFactor = Ogre::MeshManager::getSingleton().getBoundsPaddingFactor();
		// adjust min & max to exclude the padding factor..
		Ogre::Vector3 newMin = min + (max - min)*paddingFactor;
		Ogre::Vector3 newMax = max + (min - max)*paddingFactor;
		// the size should be newMax-newMin
		mSize = newMax - newMin;
		Debug("BodySize:%f,%f,%f", mSize.x, mSize.y, mSize.z);
		// Получаем радиус
		mRadius = mSize.y / 2;
		Debug("mRadius:%f", mRadius);
	}
	//-------------------------------------------------------------------------------------
	void Actor::setLinearVelocity(const Ogre::Vector3 &_mLinearVelocity){
		mLinearVelocity = _mLinearVelocity;
	}
	//-------------------------------------------------------------------------------------
	void Actor::setLinearVelocity(float _x, float _y, float _z){
		mLinearVelocity = Ogre::Vector3(_x, _y, _z);
	}
	//-------------------------------------------------------------------------------------
	void Actor::setMass(float _mass){
		mMass = _mass;
	}
	//-------------------------------------------------------------------------------------
	void Actor::setCollisionModel(int _mCollisionModel) {
		mCollisionModel = CollisionModel(_mCollisionModel);
	}
	//-------------------------------------------------------------------------------------
	void Actor::setMaterialName(const char* _material) {
		mMaterialName = _material;
	}
	//-------------------------------------------------------------------------------------
	void Actor::_switchCollision()
	{
		iBasePhysics *system = GetEngine()->GetPhysics();
		if (system) {
			phBody = GetEngine()->GetPhysics()->addObject(this);
		}
	}
}