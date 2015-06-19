/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.6.0
* Copyright (C) 2009-2015 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "Engineprivate.h"
#include "SceneManager.h"
#include "camera/CameraFPS.h"
#include "camera/CCSCameraControlSystem.h"
#include "camera/CCSBasicCameraModes.h"
#include "camera/CCSFreeCameraMode.h"

namespace vega{
	/**
	*/
	CameraFPS::CameraFPS(bool _active)
		:mCameraCS(GetEngine()->GetSceneMgr()->getCameraManager()),
		camMode(new FirstPersonCameraMode(mCameraCS, Ogre::Vector3(0, 6, -20), Ogre::Radian(0), Ogre::Radian(Ogre::Degree(180)), Ogre::Radian(0)))
	{
		mName = "CameraFPS";
		m_actDesc = ActorDescription::AD_CAMERA;

		camMode->setCharacterVisible(false);
		mCameraCS->registerCameraMode("FirstPerson", camMode);
		mCamNode = mCameraCS->getCameraSceneNode();
		if (_active)
			Activate();
	}
	/**
	*/
	CameraFPS::~CameraFPS(){
		SAFE_DELETE(camMode);
	}
	/**
	*/
	void CameraFPS::setYaw(float _angle){
		//TODO		camMode->setYaw(_angle);
	}
	/**
	*/
	void CameraFPS::setPitch(float _angle)
	{
		//TODO		camMode->setPitch(_angle);
	}
	/**
	*/
	void CameraFPS::InjectMouseUp(int _id){
		OIS::MouseButtonID id = OIS::MouseButtonID(_id);
		if (id == OIS::MB_Left) mOrbiting = false;
		else if (id == OIS::MB_Right) mZooming = false;
	}
	/**
	*/
	void CameraFPS::InjectMouseDown(int  id){
		if (id == OIS::MB_Left) mOrbiting = true;
		else if (id == OIS::MB_Right) mZooming = true;
	}
	/**
	*/
	void CameraFPS::InjectMouseMove(const Ogre::Vector2& evt){
		//Code is correctly,but is not works correctly
		//https://github.com/zinnschlag/OpenEngine/blob/master/ogre/mouselook.cpp
		mCameraCS->getOgreCamera()->yaw(Ogre::Degree(-evt.x * 0.15f));
		mCameraCS->getOgreCamera()->pitch(Ogre::Degree(-evt.y * 0.15f));
		{
			// Angle of rotation around the X-axis.
			float pitchAngle = (2 * Ogre::Degree(Ogre::Math::ACos(mCameraCS->getOgreCamera()->getOrientation().w)).valueDegrees());

			// Just to determine the sign of the angle we pick up above, the
			// value itself does not interest us.
			float pitchAngleSign = mCameraCS->getOgreCamera()->getOrientation().x;

			// Limit the pitch between -90 degress and +90 degrees, Quake3-style.
			if (pitchAngle > 90.0f)
			{
				if (pitchAngleSign > 0){
					Debug("pitchAngleSign > 0 %f", pitchAngleSign);
					// Set orientation to 90 degrees on X-axis.
					mCameraCS->getOgreCamera()->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f),
						Ogre::Math::Sqrt(0.5f), 0, 0));
				}
				else if (pitchAngleSign < 0){
					Debug("pitchAngleSign < 0 %f", pitchAngleSign);
					// Set orientation to -90 degrees on X-axis.
					mCameraCS->getOgreCamera()->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f),
						-Ogre::Math::Sqrt(0.5f), 0, 0));
				}
			}
		}
	}
	/**
	*/
	void CameraFPS::ManualStop(){
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
	void CameraFPS::InjectKeyDown(int evt){
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
	void CameraFPS::InjectKeyUp(int evt){
		if (evt == OIS::KC_W || evt == OIS::KC_UP) mGoingForward = false;
		else if (evt == OIS::KC_S || evt == OIS::KC_DOWN) mGoingBack = false;
		else if (evt == OIS::KC_A || evt == OIS::KC_LEFT) mGoingLeft = false;
		else if (evt == OIS::KC_D || evt == OIS::KC_RIGHT) mGoingRight = false;
		//		else if (evt == OIS::KC_PGUP) mGoingUp = false;
		//		else if (evt == OIS::KC_PGDOWN) mGoingDown = false;
		else if (evt == OIS::KC_LSHIFT) mFastMove = false;
	}
	/**
	*/
	void CameraFPS::Update(float _time){
		// build our acceleration vector based on keyboard input composite
		Ogre::Vector3 accel = Ogre::Vector3::ZERO;
		if (mGoingForward) accel += mCameraCS->getOgreCamera()->getDirection();
		if (mGoingBack) accel -= mCameraCS->getOgreCamera()->getDirection();
		if (mGoingRight) accel += mCameraCS->getOgreCamera()->getRight();
		if (mGoingLeft) accel -= mCameraCS->getOgreCamera()->getRight();
		//		if (mGoingUp) accel += mCameraCS->getOgreCamera()->getUp();
		//		if (mGoingDown) accel -= mCameraCS->getOgreCamera()->getUp();

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
	void CameraFPS::Activate(){
		mCameraCS->setCurrentCameraMode(camMode);
		GetEngine()->GetSceneMgr()->SetActiveCamera(this);
		//Устанавливаем фиксированную ось
		mCameraCS->getOgreCamera()->setFixedYawAxis(true);
		mCamNode->setFixedYawAxis(true);
	}
}