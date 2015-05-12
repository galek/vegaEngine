/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

namespace Ogre{
	class CgPlugin;
	class OctreePlugin;
	class ParticleFXPlugin;
	class OctreeZonePlugin;
	class GLPlugin;
	class GLESPlugin;
	class GLES2Plugin;
	class D3D9Plugin;
	class D3D11Plugin;
	class Root;
}

namespace vega
{
	/** Utility class for loading some plugins statically.
	@remarks
	When loading plugins statically, you are limited to loading plugins
	that are known about at compile time. You should define preprocessor
	symbols depending on which plugins you want to load - the symbol being
	OGRE_STATIC_<pluginname>, with pluginname being the usual name of the
	plugin DLL (no file extension, no debug suffix, and without the Plugin_
	or RenderSystem_ prefix.)
	*/
	class CORE_API StaticPluginLoader
	{
	public:
		StaticPluginLoader(Ogre::Root*_mRoot);
		/** Load all the enabled plugins against the passed in root object. */
		void load();
		/** UnLoad all the enabled plugins against the passed in root object. */
		void unload();
	private:
		Ogre::CgPlugin* mCgPlugin;
		Ogre::OctreePlugin* mOctreePlugin;
		Ogre::ParticleFXPlugin* mParticleFXPlugin;
		Ogre::OctreeZonePlugin* mOctreeZonePlugin;
		Ogre::GLPlugin* mGLPlugin;
		Ogre::GLES2Plugin* mGLES2Plugin;
		Ogre::GLESPlugin* mGLESPlugin;
		Ogre::D3D9Plugin* mD3D9Plugin;
		Ogre::D3D11Plugin* mD3D11Plugin;
		Ogre::Root*mRoot;
	};
}