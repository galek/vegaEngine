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
#include "Window.h"
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
	}

	bool EngineWrapper::Configure(void)
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
		mGWindow = mGRoot->initialise(false);
		return true;
	}

	void EngineWrapper::PreInitSystems()
	{
		EngineGlobals::PreInitSystems();
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

	void EngineWrapper::SetupResources()
	{

	}

	bool EngineWrapper::Setup(void)
	{
		StartupOgre();
		Configure();
		ChooseSceneManager();
		CreateCamera();
		mGWindow = mGRoot->createRenderWindow("OgreRenderWindow", params.width, params.height, false, params.ptr);
		CreateViewports();
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
		PreInitSystems();
		StartupSystems();
		CreateFrameListener();
		PrecacheResources();
		SetEngine(*this);

		return true;
	}

	void EngineWrapper::PrecacheResources()
	{
		//Базовые ресурсы
		std::string path = "..//Editor//Content//";
		// Adding Archives
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, "FileSystem", "Editor", true);
		EngineGlobals::PrecacheResources();
	}

	void EngineWrapper::Go(int width, int height, Ogre::NameValuePairList*_ptr)
	{
		params = { width, height, _ptr };
		if (!Setup())
			return;
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

	void EngineWrapper::InitD3D9()
	{
		Ogre::RenderSystem *renderSystem = mGRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem");
		renderSystem->setConfigOption("Allow DirectX9Ex", "Yes");
		renderSystem->setConfigOption("Full Screen", mEngineConfig->mFullscreen);
		renderSystem->setConfigOption("Video Mode", mEngineConfig->mResolution += " @ 32-bit colour");
		renderSystem->setConfigOption("Use Multihead", "Yes");
		renderSystem->setConfigOption("Backbuffer Count", "Auto");
		renderSystem->setConfigOption("FSAA", "0");
		renderSystem->setConfigOption("Resource Creation Policy", "Create on active device");
		renderSystem->setConfigOption("Multi device memory hint", "Auto hardware buffers management");
		renderSystem->setConfigOption("Fixed Pipeline Enabled", "Yes");
		renderSystem->setConfigOption("VSync", "No");
		renderSystem->setConfigOption("sRGB Gamma Conversion", "No");

		mGRoot->setRenderSystem(renderSystem);
	}
}