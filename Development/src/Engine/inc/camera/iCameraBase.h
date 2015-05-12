/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

namespace vega
{
	class ENGINE_API iCameraBase :public Actor{
	public:
		iCameraBase();
		virtual ~iCameraBase();
		virtual void InjectMouseUp(const int id) = 0;
		virtual void InjectMouseDown(const int id) = 0;
		virtual void InjectMouseWheel(const int evt) = 0;
		virtual void InjectMouseMove(const Ogre::Vector2& evt) = 0;
		virtual void InjectKeyDown(const int evt) = 0;
		virtual void InjectKeyUp(const int evt) = 0;
		virtual void ManualStop() = 0;
		virtual void Update(float) = 0;
		void Activate();
	protected:
		bool mGoingForward;
		bool mGoingBack;
		bool mGoingLeft;
		bool mGoingRight;
		bool mGoingUp;
		bool mGoingDown;
		bool mFastMove;
		bool mOrbiting;
		bool mZooming;
		float mTopSpeed;
		Ogre::Vector3 mVelocity;
	};
}