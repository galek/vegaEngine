#include "stdafx.h"
#include "EnginePrivate.h"

#include "OgreSceneManagerEnumerator.h"

#include "EngineWrapper.h"

//Common.dll
#include "inc/Raycast.h"
//Core.dll
#include "OgreStaticPluginLoader.h"
//Engine.dll
#include "Updater.h"
//cgSys.dll
#include "xgSceneFactory_t.h"

namespace vega
{
	static xgSceneFactory_t xgSceneFactory;
	/**
	*/
	EngineWrapper editor;
	/**
	*/
	EngineWrapper*GetEditor(){ return &editor; }
	/**
	*/
	EngineWrapper::EngineWrapper()
		:EngineGlobals(*this)
	{}
	/**
	*/
	EngineWrapper::~EngineWrapper()
	{}
	void EngineWrapper::StartupOgre(){
		const char* mLogPath = "EditorLog.log";

		// Init Ogre
		mGRoot = new Ogre::Root(Ogre::BLANKSTRING,
			Ogre::BLANKSTRING,
			mLogPath);
		SetLoggingLevel(3);//Disables ogreLog trash messages
		mOgrePluginLoader = new StaticPluginLoader(mGRoot);
		mOgrePluginLoader->load();
		InitRenderer();

		mGWindow = mGRoot->initialise(false);

		SetupResources();
	}

	void EngineWrapper::PreInitSystems()
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
		editorL = true;
	}

	void EngineWrapper::StartupSystems()
	{
		Initialize();
		SetPause(true);
		PrecacheResources();

		//Базовые ресурсы
		std::string path = "..//Editor//Content//";
		// Adding Archives
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, "FileSystem", "Editor", true);
	}

	void EngineWrapper::Go(int width, int height, Ogre::NameValuePairList*_ptr)
	{
		StartupOgre();
		SetupResources();
		//Configure();
		ChooseSceneManager();
		CreateCamera();
		mGWindow = mGRoot->createRenderWindow("OgreRenderWindow", width, height, false, _ptr);
		CreateViewports();
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
		PreInitSystems();
		StartupSystems();
		CreateFrameListener();
		SetEngine(*this);
	}

	void EngineWrapper::ChooseSceneManager()
	{
		Ogre::SceneManagerEnumerator::getSingleton().addFactory(&xgSceneFactory);
		mGSceneMgr = mGRoot->createSceneManager(Ogre::ST_INTERIOR + 4, "xgSceneInstance");
	}

	xgScene_t* EngineWrapper::GetEditorScene(){
		if (!edscene)
			edscene = (xgScene_t*)(mGSceneMgr);
		return edscene;
	}

	void EngineWrapper::CreateFrameListener()
	{
		window->Listener();
		//crash	input->createFrameListener();
		//crash	input->createFrameListener();
		mGRoot->addFrameListener(updater);
	}
}