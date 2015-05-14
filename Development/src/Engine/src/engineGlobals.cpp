/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "Console.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "SceneManager.h"
#include "EnginePlugins.h"
#include "GUI/GUI.h"
#include "EngineScriptingEx.h"
#include "../../Core/inc/OgreStaticPluginLoader.h"
#include "../../Common/inc/Raycast.h"
#include "Updater.h"

namespace vega
{
	//-------------------------------------------------------------------------------------
	EngineGlobals::EngineGlobals()
		:window(nullptr)
		, plugins(nullptr)
		, input(nullptr)
		, console(nullptr)
		, render(nullptr)
		, audio(nullptr)
		, gui(nullptr)
		, sceneManager(nullptr)
		, physics(nullptr)
		, mOgrePluginLoader(nullptr)
		, updater(nullptr)
		, ai(nullptr)
		, mShutDown(false)
		, editorL(false)
	{
	}
	//-------------------------------------------------------------------------------------
	EngineGlobals::EngineGlobals(const EngineGlobals &gl)
	{
		window = gl.window;
		plugins = gl.plugins;
		input = gl.input;
		console = gl.console;
		render = gl.render;
		audio = gl.audio;
		gui = gl.gui;
		sceneManager = gl.sceneManager;
		physics = gl.physics;
		game = gl.game;
		mOgrePluginLoader = gl.mOgrePluginLoader;
		updater = gl.updater;
		ai = gl.ai;
		editorL = gl.editorL;
	}
	//-------------------------------------------------------------------------------------
	EngineGlobals::~EngineGlobals()	{
		Release();
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::PreInitSystems()
	{
		WriteInfoAboutBuild();
		Debug("[PreInit]Updater");
		updater = new Updater(this);
		Debug("[PreInit]CollisionTools");
		mRaycast = new CollisionTools(this);
		Debug("[PreInit]Plugins");
		plugins = new EnginePlugins();
		Debug("[PreInit]loading Plugins");
		LoadEngineModules();
		Debug("[PreInit]Window");
		window = new Window();
		Debug("[PreInit]Input");
		input = new Input();
		Debug("[PreInit]Console");
		console = new Console();
		Debug("[PreInit]Render");
		render = new Render(this);
		Debug("[PreInit]GUI");
		gui = new GUI();
		Debug("[PreInit]LevelManager");
		sceneManager = new SceneManager(this);
		Debug("[PreInit]AI");
		ai = new AI(mGSceneMgr);
		Debug("[PreInit]Attempt Config");
		AttemptConfig();
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::Initialize()
	{
		Debug("[StartupSystems]physics");
		if (physics)
			physics->initialize();
		else
			Warning("[StartupSystems]physics not Initialize");
		Debug("[StartupSystems]render");
		if (render)
			render->Initialize();
		else
			Warning("[StartupSystems]render not Initialize");
		Debug("[StartupSystems]gui");
		if (gui)
			gui->Initialize();
		else
			Warning("[StartupSystems]gui not Initialize");
		if (console)
			console->Initialize();
		else
			Warning("[StartupSystems]console not Initialize");

		if (mScript){
			mScript->Initialize();
			_RenderAddScriptFunctionaluty(mScript);
			_EngineAddScriptFunctionaluty(mScript);
		}
		else
			Warning("[StartupSystems]scripting not Initialize");

		Debug("[StartupSystems]iGame");
		if (game)
			game->PreInitialize();
		else
			Debug("[StartupSystems]iGame not Initialize");

		//”казываем состо€ние,что мы уже готовы играть
		mEngineState = ES_PLAY;
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::SetGame(iGame*_game){
		game = _game;
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::Update(float _time)
	{
		if (input)
			input->frameRenderingQueued();
		else
			Debug("[StartupSystems]input not updated");

		if (mEngineState == ES_PLAY)
		{
			if (audio)
				audio->updateSounds();
			else
				Debug("[StartupSystems]audio not updated");

			if (physics)
				physics->update(_time);
			else
				Debug("[StartupSystems]physics not updated");

			if (render)
				render->Update(_time);
			else
				Debug("[StartupSystems]render not updated");

			//Updating Camera
			if (sceneManager)
				sceneManager->Update(_time);
			else
				Debug("[StartupSystems]levelloader not updated");

			if (game)
				game->Update(_time);
			else
				Debug("[StartupSystems]iGame not updated");
		}

		if (gui)
			gui->Update(_time);
		else
			Debug("[StartupSystems]gui not updated");
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::StartupOgre()
	{
#ifdef _DEVELOP
		const char* mLogPath = "Log.log";
#else
		const char* mLogPath = "..\\Engine\\Logs\\Log.log";
#endif
		// Init Ogre
		mGRoot = new Ogre::Root(Ogre::BLANKSTRING,
			Ogre::BLANKSTRING,
			mLogPath);
		SetLoggingLevel(3);//Disables ogreLog trash messages
		mOgrePluginLoader = new StaticPluginLoader(mGRoot);
		mOgrePluginLoader->load();
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::EngineShoutdown()	{
		Debug("EngineGlobals::EngineShoutdown");
		mShutDown = (!(isLaunched()));
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::Release()	{
		Debug("EngineGlobals::Release");
		SAFE_DELETE(game);
		SAFE_DELETE(sceneManager);
		SAFE_DELETE(gui);
		SAFE_DELETE(audio);
		SAFE_DELETE(physics);
		SAFE_DELETE(render);
		SAFE_DELETE(input);
		SAFE_DELETE(mRaycast);
		SAFE_DELETE(window);
		SAFE_DELETE(plugins);
		SAFE_DELETE(mOgrePluginLoader);
		//TODO
		//delete ai
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::LoadResources(void){
		Debug("EngineGlobals::LoadResources");
		Ogre::ResourceBackgroundQueue::getSingleton().initialiseAllResourceGroups();
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::Go(void)
	{
		if (!Setup())
			return;
		mGRoot->startRendering();
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::CreateViewports(void)
	{
		// Create one viewport, entire window
		mGViewport = mGWindow->addViewport(mGCamera);
		mGViewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

		// Alter the camera aspect ratio to match the viewport
		mGCamera->setAspectRatio(
			float(mGViewport->getActualWidth()) / float(mGViewport->getActualHeight()));
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::PrecacheResources()
	{
		//Ѕазовые ресурсы
		std::string path = "..//Engine//Content//";
		// Adding Archives
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, "FileSystem", "Engine", true);
		Ogre::StringVectorPtr gameArchives = Ogre::ResourceGroupManager::getSingleton().findResourceNames("Engine", "*.npk", false);
		for (unsigned int i = 0; i < gameArchives->size(); i++)
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path + (*gameArchives)[i], "Zip", "Engine", true);

		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("..//Engine//Scripts//", "FileSystem", "Engine", true);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("..//Game//Scripts//", "FileSystem", "Game", true);
		LoadResources();

		if (mEngineConfig->mPrecacheResources)
		{
			Debug("[Engine-Resource]-Loading resources from PrecacheResources");


			// поиск и подключение архивов
			path = "..//Game//Content//";
			// Adding Archives
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, "FileSystem", "Game", true);
			gameArchives = Ogre::ResourceGroupManager::getSingleton().findResourceNames("Game", "*.npk", false);
			for (unsigned int i = 0; i < gameArchives->size(); i++)
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path + (*gameArchives)[i], "Zip", "Game", true, true);

			LoadResources();
		}
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::SetupResources()
	{
		Debug("[Engine-Resource]-Loading resources from setupResources");

		std::string path = "..//Engine//Shaders//";
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, "FileSystem", "Shaders", true);
		path = "..//Engine//Shaders//programs//Cg//";
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, "FileSystem", "Shaders", true);
		path = "..//Engine//Shaders//programs//GLSL//";
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, "FileSystem", "Shaders", true);
		path = "..//Engine//Shaders//programs//GLSL150//";
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, "FileSystem", "Shaders", true);
		path = "..//Engine//Shaders//programs//GLSL400//";
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, "FileSystem", "Shaders", true);
		path = "..//Engine//Shaders//programs//GLSLES//";
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, "FileSystem", "Shaders", true);
		path = "..//Engine//Shaders//programs//HLSL//";
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, "FileSystem", "Shaders", true);
		path = "..//Engine//Shaders//programs//";
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, "FileSystem", "Shaders", true);
		// Adding Archives
		Ogre::StringVectorPtr gameArchives = Ogre::ResourceGroupManager::getSingleton().findResourceNames("Shaders", "*.shaders", false);
		for (unsigned int i = 0; i < gameArchives->size(); i++)
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path + (*gameArchives)[i], "Zip", "Shaders", true);

	}
	//-------------------------------------------------------------------------------------
	bool EngineGlobals::Configure(void)
	{
		Ogre::String platform = "VEGAEngine";
#ifdef _WIN64
		platform += "(W64)";
#elif _WIN32
		platform += "(W32)";
#else
		platform += "(Unknown)";
#endif

		InitRenderer();
		mGWindow = mGRoot->initialise(true, platform);
		return true;
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::ChooseSceneManager(void)	{
		mGSceneMgr = mGRoot->createSceneManager("OctreeSceneManager");
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::CreateCamera(void)
	{
		mGCamera = mGSceneMgr->createCamera("PlayerCam");
		mGCamera->setPosition(Ogre::Vector3(0, 0, 0));
		mGCamera->lookAt(Ogre::Vector3(0, 0, 0));
		mGCamera->setNearClipDistance(5);
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::CreateFrameListener(void)
	{
		window->Listener();
		input->createFrameListener();
		mGRoot->addFrameListener(updater);
	}
	//-------------------------------------------------------------------------------------
	bool EngineGlobals::Setup(void)
	{
		StartupOgre();
		SetupResources();
		Configure();
		ChooseSceneManager();
		CreateCamera();
		CreateViewports();
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
		PreInitSystems();
		StartupSystems();
		CreateFrameListener();
		if (game)
			game->CreateScene();

		return true;
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::StartupSystems()	{
		Initialize();
		PrecacheResources();
	}
	//-------------------------------------------------------------------------------------
	bool EngineGlobals::Update(const Ogre::FrameEvent& evt)
	{
		if (mGWindow->isClosed())
			return false;

		if (isLaunched())
			return false;

		Update(evt.timeSinceLastFrame);

		return true;
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::RunScript(const char*_a)	{
		mScript->RunScript(_a);
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::SetPause(bool _a)	{
		(_a) ? mEngineState = ES_PAUSE : mEngineState = ES_PLAY;
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::RunConsoleCommand(const char*_str) {
		TODO("Make this");
	}
	//-------------------------------------------------------------------------------------
	void EngineGlobals::AttemptConfig() {
		mGSceneMgr->setShadowFarDistance(mEngineConfig->mShadowFarDistance);
		mGSceneMgr->setShadowTextureSize(mEngineConfig->mShadowTextureSize);
		mGCamera->setFarClipDistance(mEngineConfig->mFarClipDistance);
	}
	//-------------------------------------------------------------------------------------
	const char* EngineGlobals::GetCurrentState()
	{
		if (mEngineState == ES_LOADING)
			return "LOADING";
		else if (mEngineState == ES_PAUSE)
			return "PAUSE";
		else if (mEngineState == ES_PLAY)
			return "PLAYING";
	}
}