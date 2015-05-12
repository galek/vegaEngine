/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once
#include "iCameraBase.h"

namespace vega
{
	class ENGINE_API CameraFPS :public iCameraBase{
	public:
		CameraFPS(bool _active = false);
		virtual ~CameraFPS();
		void setYaw(float _angle);
		void setPitch(float _angle);
		virtual void InjectMouseUp(const int id);
		virtual void InjectMouseDown(const int  id);
		virtual void InjectMouseWheel(const int evt){}
		virtual void InjectMouseMove(const Ogre::Vector2& evt);
		virtual void InjectKeyDown(const int evt);
		virtual void InjectKeyUp(const int evt);
		virtual void ManualStop();
		virtual void Update(float);
		virtual void Activate();
		void moveCamera();
	private:
		class CameraControlSystem*mCameraCS;
		class FirstPersonCameraMode* camMode;
		Ogre::SceneNode*mCamNode;
	};
}