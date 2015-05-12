/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "Engineprivate.h"
#include "camera/iCameraBase.h"

namespace vega
{
	/**
	*/
	iCameraBase::iCameraBase()
		:mTopSpeed(150), mVelocity(Ogre::Vector3::ZERO),
		mGoingForward(false), mGoingBack(false), mGoingLeft(false),
		mGoingRight(false), mGoingUp(false), mGoingDown(false),
		mFastMove(false), mOrbiting(false), mZooming(false)
	{
		GetEngine()->GetSceneMgr()->AddActorCameraToList(this);
	}
	/**
	*/
	iCameraBase::~iCameraBase(){
		GetEngine()->GetSceneMgr()->DeleteCameraFromList(this);
		TODO("Crash");
	}
	/**
	*/
	void iCameraBase::Activate(){
		GetEngine()->GetSceneMgr()->SetActiveCamera(this);
	}
}