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
	Window::~Window()
	{
		//Remove ourself as a Window listener
		Ogre::WindowEventUtilities::removeWindowEventListener(GetEngine()->mGWindow, this);
		windowClosed(GetEngine()->mGWindow);
	}

	void Window::Listener()
	{
		//Register as a Window listener
		Ogre::WindowEventUtilities::addWindowEventListener(GetEngine()->mGWindow, this);
	}
}