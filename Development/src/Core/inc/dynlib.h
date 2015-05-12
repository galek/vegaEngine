/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

#if ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibrary( a )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#elif ENGINE_PLATFORM == ENGINE_PLATFORM_LINUX
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY )
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )

#elif ENGINE_PLATFORM == ENGINE_PLATFORM_ANDROID
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY )
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )

#elif ENGINE_PLATFORM == ENGINE_PLATFORM_MAC
#    define DYNLIB_HANDLE CFBundleRef
#    define DYNLIB_LOAD( a ) mac_loadExeBundle( a )
#    define DYNLIB_GETSYM( a, b ) mac_getBundleSym( a, b )
#    define DYNLIB_UNLOAD( a ) mac_unloadExeBundle( a )
#endif

namespace vega
{
	//-----------------------------------------------------------------------------
	// Cross-platform dynamic library system
	//-----------------------------------------------------------------------------
	class CORE_API DynLib
	{
	public:
		///
		DynLib(const char* name);
		///
		~DynLib();
		/// Load the library
		void Load();
		/// Unload the library
		void Unload();
		/// Get the name of the library
		const char*	GetName(void) const { return mName; }
		/// Returns the address (handle) of the given symbol from the loaded library.
		void* GetSymbol(const char* strName) const throw();
	protected:
		/// Gets the last loading error
		const char* GetError(void);
	protected:
		const char* mName;
		/// Handle to the loaded library.
		DYNLIB_HANDLE m_hInst;
	};
}