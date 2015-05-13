/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
-----------------------------------------------------------------------------
*/

#ifndef __SHAREDDATA_H
#define __SHAREDDATA_H

#include "Ogre.h"
#include "DeferredShading.h"

namespace vega
{
	class SharedData : public Ogre::Singleton < SharedData > {

	public:

		SharedData()
			: iRoot(nullptr),
			iCamera(nullptr),
			iWindow(nullptr),
			iSceneMgr(nullptr),
			mMLAnimState(nullptr)
		{
			iActivate = false;
		}

		~SharedData() {}

		// shared data across the application
		Ogre::Real iLastFrameTime;
		Ogre::Root *iRoot;
		Ogre::Camera *iCamera;
		Ogre::RenderWindow *iWindow;
		Ogre::SceneManager	*iSceneMgr;

		DeferredShadingSystem *iSystem;
		bool iActivate;
		bool iGlobalActivate;

		// Animation state for light swarm
		Ogre::AnimationState* mMLAnimState;
		
		Ogre::vector<Ogre::Node*>::type mLightNodes;

	};
}
#endif