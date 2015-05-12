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
	struct SubSystemsManager
	{
		SubSystemsManager();
		virtual ~SubSystemsManager();
		//In public for plugins acess
		Ogre::SceneManager* mGSceneMgr;
		Ogre::Root *mGRoot;

		Ogre::Camera* mGCamera;				// Static Camera controller	
		Ogre::Viewport* mGViewport;			// Create one viewport, entire window
		Ogre::RenderWindow* mGWindow;
	};
};