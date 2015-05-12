/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "Updater.h"

namespace vega
{
	bool Updater::frameRenderingQueued(const Ogre::FrameEvent& evt){
		mUpdatedTime = evt.timeSinceLastFrame;
		return engine->Update(evt);
	}
}