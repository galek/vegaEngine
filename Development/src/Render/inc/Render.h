/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.6.0
* Copyright (C) 2009-2015 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

namespace vega
{
	class Externals;
	class VideoPlayer;
	class SharedData;
	class DeferredShadingSystem;

	class RENDER_API Render
	{
	public:
		Render(CoreSystems* _manager);
		~Render();

		void Initialize();
		void Update(float _evt);


		void PostEffectSetStatus(std::string _name, bool _status);
		bool PlayVideo(std::string _name);
		VideoPlayer* GetPlayer();
		void EnableDisableAA(bool _s);

		inline Ogre::SceneManager*GetSceneMgr() { return mSceneMgr; }
		inline Ogre::RenderWindow*GetWindowMgr() { return mWindow; }
		inline Ogre::Camera*GetCamera() { return mCamera; }
		inline Ogre::Viewport*GetViewport() { return mViewport; }
	private:
		void _InitPostEffects();
		void _CreatePostEffects();
		void _EnablePostEffects();

		//Loading Shaders
		void _LoadShaders();
	private:
		Ogre::Root *mRoot;
		Ogre::SceneManager* mSceneMgr;
		Ogre::RenderWindow* mWindow;
		Ogre::Camera*mCamera;
		Ogre::Viewport*mViewport;

		Externals*externals;
		EngineConfig*mEngineConfig;
		DeferredShadingSystem* mSystem;
		SharedData* shData;
	private:
		std::vector<std::string>mCompositorNames;
	};
}