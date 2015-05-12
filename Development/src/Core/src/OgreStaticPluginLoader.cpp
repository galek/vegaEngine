/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"
#include "OgreStaticPluginLoader.h"

#pragma message("NICK-REFACTOR IT 2015")
#pragma comment(lib,"Plugin_OctreeSceneManager.lib")
#pragma comment(lib,"Plugin_CgProgramManager.lib")
#pragma comment(lib,"Plugin_ParticleFX.lib")

// Static plugin headers
#  include "OgreCgPlugin.h"
#  include "OgreOctreePlugin.h"
#  include "OgreParticleFXPlugin.h"

#if ENGINE_PLATFORM == ENGINE_PLATFORM_ANDROID

#ifdef OGRE_BUILD_RENDERSYSTEM_GLES2
#include "OgreGLES2Plugin.h"
#pragma comment(lib,"libRenderSystem_GLES2")
#endif

#ifdef OGRE_BUILD_RENDERSYSTEM_GLES
#include "OgreGLESPlugin.h"
#pragma comment(lib,"libRenderSystem_GLES.a")
#endif

#elif ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
#ifdef USE_D3D11_RENDER
#ifdef OGRE_BUILD_RENDERSYSTEM_D3D11
#  include "OgreD3D11Plugin.h"
#pragma comment(lib,"RenderSystem_Direct3D11")
#endif
#endif

#ifdef USE_D3D9_RENDER
#ifdef OGRE_BUILD_RENDERSYSTEM_D3D9
#  include "OgreD3D9Plugin.h"
#pragma comment(lib,"RenderSystem_Direct3D9")

#ifdef OGRE_STATIC_LIB
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"DxErr.lib")
#pragma comment(lib,"dxguid.lib")
#endif
#endif
#endif

#ifdef USE_OGL_RENDER
#ifdef OGRE_BUILD_RENDERSYSTEM_GL
#  include "OgreGLPlugin.h"
#pragma comment(lib,"RenderSystem_OpenGL")
#endif
#endif
#elif ENGINE_PLATFORM == ENGINE_PLATFORM_LINUX
#ifdef OGRE_BUILD_RENDERSYSTEM_GL
#  include "OgreGLPlugin.h"
#pragma comment(lib,"RenderSystem_OpenGL")
#endif
#else
#error(Unknow platform-plugins not will installed)
#endif

namespace vega
{
	StaticPluginLoader::StaticPluginLoader(Ogre::Root*_mRoot)
		:mCgPlugin(nullptr),
		mOctreePlugin(nullptr),
		mParticleFXPlugin(nullptr),
		mOctreeZonePlugin(nullptr),
		mGLPlugin(nullptr),
		mGLESPlugin(nullptr),
		mGLES2Plugin(nullptr),
		mD3D9Plugin(nullptr),
		mD3D11Plugin(nullptr),
		mRoot(_mRoot) {}
	/** Load all the enabled plugins against the passed in root object. */
	void StaticPluginLoader::load()
	{
		using namespace Ogre;
#ifdef USE_OGL_RENDER
		mGLPlugin = new GLPlugin();
		mRoot->installPlugin(mGLPlugin);
#endif
#if ENGINE_PLATFORM == ENGINE_PLATFORM_ANDROID
//#ifdef OGRE_BUILD_RENDERSYSTEM_GLES2
//		mGLES2Plugin = new GLES2Plugin();
//		mRoot->installPlugin(mGLES2Plugin);
//#endif

#ifdef OGRE_BUILD_RENDERSYSTEM_GLES
		mGLESPlugin = new GLESPlugin();
		mRoot->installPlugin(mGLESPlugin);
#endif
#endif


#if ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
#ifdef USE_D3D11_RENDER
		mD3D11Plugin = new D3D11Plugin();
		mRoot->installPlugin(mD3D11Plugin);
#endif
#ifdef USE_D3D9_RENDER
		mD3D9Plugin = new D3D9Plugin();
		mRoot->installPlugin(mD3D9Plugin);
#endif
#endif

#if ENGINE_PLATFORM != ENGINE_PLATFORM_ANDROID
		mCgPlugin = new CgPlugin();
		mRoot->installPlugin(mCgPlugin);
#endif

		mOctreePlugin = new OctreePlugin();
		mRoot->installPlugin(mOctreePlugin);

		mParticleFXPlugin = new ParticleFXPlugin();
		mRoot->installPlugin(mParticleFXPlugin);
	}

	void StaticPluginLoader::unload()
	{
		// don't unload plugins, since Root will have done that. Destroy here.
		SAFE_DELETE(mParticleFXPlugin);
		SAFE_DELETE(mOctreePlugin);
#if ENGINE_PLATFORM != ENGINE_PLATFORM_ANDROID
		SAFE_DELETE(mCgPlugin);
#endif

#if ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
#ifdef USE_D3D11_RENDER
		SAFE_DELETE(mD3D11Plugin);
#endif
#ifdef USE_D3D9_RENDER
		SAFE_DELETE(mD3D9Plugin);
#endif
#endif
#ifdef USE_OGL_RENDER
		SAFE_DELETE(mGLPlugin);
#endif

#if ENGINE_PLATFORM == ENGINE_PLATFORM_ANDROID
#ifdef OGRE_BUILD_RENDERSYSTEM_GLES2
		SAFE_DELETE(mGLES2Plugin);
#endif
#ifdef OGRE_BUILD_RENDERSYSTEM_GLES
		SAFE_DELETE(mGLESPlugin);
#endif
#endif
	}

}