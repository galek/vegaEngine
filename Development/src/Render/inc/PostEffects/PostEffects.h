/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

#include "ListenerFactoryLogic.h"

namespace vega
{

	//The compositor logic for the heat vision compositor
	class HeatVisionLogic : public ListenerFactoryLogic
	{
	protected:
		/** @copydoc ListenerFactoryLogic::createListener */
		virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance);
	};

	//The compositor logic for the hdr compositor
	class HDRLogic : public ListenerFactoryLogic
	{
	protected:
		/** @copydoc ListenerFactoryLogic::createListener */
		virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance);
	};

	//The compositor logic for the gaussian blur compositor
	class GaussianBlurLogic : public ListenerFactoryLogic
	{
	protected:
		/** @copydoc ListenerFactoryLogic::createListener */
		virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance);
	};

	class CameraEffectsLogic : public ListenerFactoryLogic
	{
	protected:
		/** @copydoc ListenerFactoryLogic::createListener */
		virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance);
	};
}