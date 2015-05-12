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
	class Render;
	class iGame;
	struct iInterp;
	class StaticPluginLoader;
	class Window;
	class EnginePlugins;
	class Input;
	class Console;
	class AudioManager;
	class GUI;
	class SceneManager;
	class Physics;
	class Updater;
	class AI;

	struct ENGINE_API EngineGlobals :public CoreSystems
	{
		EngineGlobals(void);
		EngineGlobals(const EngineGlobals &gl);
		virtual ~EngineGlobals(void);
		virtual void Initialize(void);
		void StartupOgre(void);
		void Update(float _time);
		virtual void Release(void);
		__inline bool isLaunched() const { return mShutDown; }
		// !@Engine Shoutdown
		void EngineShoutdown(void);
		void Go(void);
		void SetGame(iGame*_game);

		void RunScript(const char*_filename);

		//OGRE
		// !@Creating Listeners for Update cicle
		void CreateFrameListener(void);
		// !@Update Cicle
		bool Update(const Ogre::FrameEvent& evt);
		// !@Creation Ogre 
		bool Setup(void);
		// !@Configure Engine
		bool Configure(void);
		// !@Choosing SceneManager from loaded addon
		void ChooseSceneManager(void);
		// !@Creating Fly and Standart Camera
		void CreateCamera(void);
		// !@Creating Main Viewport
		void CreateViewports(void);
		// !@Adding All Zip's,user dir's and directories(using masks)
		void SetupResources(void);
		// !@Loading Resources
		void LoadResources(void);
		// !@Creating classes for struct engine
		void PreInitSystems(void);
		// !@Initiliasing systems
		void StartupSystems(void);
		// !@Precache
		void PrecacheResources(void);
		// !@Set Pause
		void SetPause(bool _a);
		// !@Run Console Command
		void RunConsoleCommand(const char*_str);
		// !@Attempt Config Params
		void AttemptConfig(void);
		// !@For loading dll-modules from Modules.ini 
		void LoadEngineModules(void);
		// !@Get Current state as const char* 
		const char* GetCurrentState();
	public:
		__inline Window* GetWindow()  const { return window; }
		__inline EnginePlugins* GetPlugins()  const { return plugins; }
		__inline Input* GetInput()  const { return input; }
		__inline Console* GetConsole()  const { return console; }
		__inline Render* GetRender()  const { return render; }
		__inline iAudioDevice* GetAudio()  const { return audio; }
		__inline GUI* GetGui()  const { return gui; }
		__inline SceneManager* GetSceneMgr()  const { return sceneManager; }
		__inline iBasePhysics* GetPhysics()  const { return physics; }
		__inline iGame* GetGame()  const { return game; }
		__inline StaticPluginLoader* GetOgrePluginLoader()  const { return mOgrePluginLoader; }
		__inline Updater* GetUpdater()  const { return updater; }
		__inline AI* GetAI()  const { return ai; }
		__inline bool isEditor()  const { return editorL; }
	public://protected:
		Window*				              window;				// Ogre window listener
		EnginePlugins* 		              plugins;				// plugins
		Input*				              input;				// input
		Console*			              console;
		Render*                           render;				// render
		iAudioDevice*                     audio;			    // audio
		GUI*			                  gui;					// GUI
		SceneManager*		              sceneManager;			// Level Loader
		iBasePhysics*                     physics;		        // Physics
		iGame*                            game;					// Game
		StaticPluginLoader*               mOgrePluginLoader;
		Updater*                          updater;
		AI*                               ai;
		bool editorL;
	private:
		bool mShutDown;
	};
	ENGINE_API EngineGlobals*GetEngine();
	ENGINE_API void SetEngine(EngineGlobals&_engine);
}