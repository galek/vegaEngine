/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
//”ниверсальный класс рендера
#pragma once

#ifndef RENDER_API
#define RENDER_API __declspec(dllimport)
#endif

namespace vega
{
	class Externals;
	class VideoPlayer;

	class RENDER_API Render
	{
	public:
		Render( CoreSystems* _manager);
		~Render();

		void Initialize();
		void Update(float _evt);


		void PostEffectSetStatus(std::string _name, bool _status);
		bool PlayVideo(std::string _name);
		VideoPlayer* GetPlayer();
		void EnableDisableAA(bool _s);
	private:
		void _InitPostEffects();
		void _CreatePostEffects();
		void _EnablePostEffects();

		//Loading Shaders
		void _LoadShaders();
	public:
		Ogre::Root *mRoot;
		Ogre::SceneManager* mSceneMgr;
		Ogre::RenderWindow* mWindow;
		Ogre::Camera*mCamera;
		Ogre::Viewport*mViewport;

		Externals*externals;
		EngineConfig*mEngineConfig;
	private:
		std::vector<std::string>mCompositorNames;
	};
}