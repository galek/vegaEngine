/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "Engineprivate.h"
#include "camera/CameraFree.h"
#include "camera/CCSCameraControlSystem.h"
#include "camera/CCSBasicCameraModes.h"
#include "camera/CCSFreeCameraMode.h"

namespace vega{
	/**
	*/
	CameraFree::CameraFree(bool _active)
		:mCameraCS(GetEngine()->GetSceneMgr()->getCameraManager()),
		camMode(new FreeCameraMode(mCameraCS, Ogre::Vector3::ZERO, Ogre::Degree(225), Ogre::Degree(10)
		, FreeCameraMode::CURRENT_STATE))
	{
		mCameraCS->registerCameraMode("Free", camMode);
		camMode->setMoveFactor(30);
		if (_active)
			Activate();
	}
	/**
	*/
	CameraFree::~CameraFree(){
		SAFE_DELETE(camMode);
	}
	/**
	*/
	void CameraFree::setYaw(float _angle){
		camMode->setYaw(_angle);
	}
	/**
	*/
	void CameraFree::setPitch(float _angle)	{
		camMode->setPitch(_angle);
	}
	/**
	*/
	void CameraFree::InjectMouseUp(int _id){
		OIS::MouseButtonID id = OIS::MouseButtonID(_id);
		if (id == OIS::MB_Left) mOrbiting = false;
		else if (id == OIS::MB_Right) mZooming = false;
	}
	/**
	*/
	void CameraFree::InjectMouseDown(int  id){
		if (id == OIS::MB_Left) mOrbiting = true;
		else if (id == OIS::MB_Right) mZooming = true;
	}
	/**
	*/
	void CameraFree::InjectMouseMove(const Ogre::Vector2& evt){
		mCameraCS->getOgreCamera()->yaw(Ogre::Degree(-evt.x * 0.15f));
		mCameraCS->getOgreCamera()->pitch(Ogre::Degree(-evt.y * 0.15f));
	}
	/**
	*/
	void CameraFree::ManualStop(){
		mGoingForward = false;
		mGoingBack = false;
		mGoingLeft = false;
		mGoingRight = false;
		mGoingUp = false;
		mGoingDown = false;
		mVelocity = Ogre::Vector3::ZERO;
	}
	/**
	*/
	void CameraFree::InjectKeyDown(int evt){
		if (evt == OIS::KC_W || evt == OIS::KC_UP) mGoingForward = true;
		else if (evt == OIS::KC_S || evt == OIS::KC_DOWN) mGoingBack = true;
		else if (evt == OIS::KC_A || evt == OIS::KC_LEFT) mGoingLeft = true;
		else if (evt == OIS::KC_D || evt == OIS::KC_RIGHT) mGoingRight = true;
		else if (evt == OIS::KC_PGUP) mGoingUp = true;
		else if (evt == OIS::KC_PGDOWN) mGoingDown = true;
		else if (evt == OIS::KC_LSHIFT) mFastMove = true;
	}
	/**
	*/
	void CameraFree::InjectKeyUp(int evt){
		if (evt == OIS::KC_W || evt == OIS::KC_UP) mGoingForward = false;
		else if (evt == OIS::KC_S || evt == OIS::KC_DOWN) mGoingBack = false;
		else if (evt == OIS::KC_A || evt == OIS::KC_LEFT) mGoingLeft = false;
		else if (evt == OIS::KC_D || evt == OIS::KC_RIGHT) mGoingRight = false;
		else if (evt == OIS::KC_PGUP) mGoingUp = false;
		else if (evt == OIS::KC_PGDOWN) mGoingDown = false;
		else if (evt == OIS::KC_LSHIFT) mFastMove = false;
	}
	/**
	*/
	void CameraFree::Update(float _time){
		// build our acceleration vector based on keyboard input composite
		Ogre::Vector3 accel = Ogre::Vector3::ZERO;
		if (mGoingForward) accel += mCameraCS->getOgreCamera()->getDirection();
		if (mGoingBack) accel -= mCameraCS->getOgreCamera()->getDirection();
		if (mGoingRight) accel += mCameraCS->getOgreCamera()->getRight();
		if (mGoingLeft) accel -= mCameraCS->getOgreCamera()->getRight();
		if (mGoingUp) accel += mCameraCS->getOgreCamera()->getUp();
		if (mGoingDown) accel -= mCameraCS->getOgreCamera()->getUp();

		// if accelerating, try to reach top speed in a certain time
		float topSpeed = mFastMove ? mTopSpeed * 20 : mTopSpeed;
		if (accel.squaredLength() != 0)
		{
			accel.normalise();
			mVelocity += accel * topSpeed * _time * 10;
		}
		// if not accelerating, try to stop in a certain time
		else mVelocity -= mVelocity * _time * 10;

		float tooSmall = std::numeric_limits<float>::epsilon();

		// keep camera velocity below top speed and above epsilon
		if (mVelocity.squaredLength() > topSpeed * topSpeed)
		{
			mVelocity.normalise();
			mVelocity *= topSpeed;
		}
		else if (mVelocity.squaredLength() < tooSmall * tooSmall)
			mVelocity = Ogre::Vector3::ZERO;

		if (mVelocity != Ogre::Vector3::ZERO) mCameraCS->getOgreCamera()->move(mVelocity * _time);
	}
	/*
	**/
	void CameraFree::Activate(){
		mCameraCS->setCurrentCameraMode(camMode);
		GetEngine()->GetSceneMgr()->SetActiveCamera(this);
	}
}