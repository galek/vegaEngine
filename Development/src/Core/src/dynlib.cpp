/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"

#pragma message("NICK-REFACTOR IT 2015")

namespace vega
{
	//-----------------------------------------------------------------------
	DynLib::DynLib(const char* name)
	{
		mName = name;
		m_hInst = NULL;

#ifdef _DEBUG
		mName = (std::string(name)+"_D").c_str();
#endif
	}

	//-----------------------------------------------------------------------
	DynLib::~DynLib()
	{}

	//-----------------------------------------------------------------------
	void DynLib::Load()
	{
#if ENGINE_PLATFORM != ENGINE_PLATFORM_ANDROID
		// Log library load
		Debug("Loading library %s",mName);

		const char* name = mName;
#if ENGINE_PLATFORM == ENGINE_PLATFORM_LINUX
		// dlopen() does not add .so to the filename, like windows does for .dll
		if (name.substr(name.length() - 3, 3) != ".so")
			name += ".so";
#endif

		m_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD( name );
		if( !m_hInst )
			Warning("Could not load dynamic library %s.  System Error: %s DynLib::load",mName,GetError() );
#endif
	}

	//-----------------------------------------------------------------------
	void DynLib::Unload()
	{
#if ENGINE_PLATFORM != ENGINE_PLATFORM_ANDROID
		// Log library unload
		Debug("Unloading library %s",mName);

		if( DYNLIB_UNLOAD( m_hInst ) )
			Warning("Could not unload dynamic library %s.  System Error: %s.DynLib::unload",mName,GetError() );
#endif
	}

	//-----------------------------------------------------------------------
	void* DynLib::GetSymbol(const char* strName) const throw()	{
		return (void*)DYNLIB_GETSYM( m_hInst, strName );
	}

	//-----------------------------------------------------------------------
	const char* DynLib::GetError( void ) 
	{
#if ENGINE_PLATFORM != ENGINE_PLATFORM_ANDROID
#if ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
		LPVOID lpMsgBuf; 
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, 
			NULL, 
			GetLastError(), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR) &lpMsgBuf, 
			0, 
			NULL 
			); 
		const char* ret = (char*)lpMsgBuf;
		// Free the buffer.
		LocalFree( lpMsgBuf );
		return ret;
#elif ENGINE_PLATFORM == ENGINE_PLATFORM_LINUX
		const char* ret = (char*)dlerror();
		return ret;
#elif ENGINE_PLATFORM == ENGINE_PLATFORM_MAC
		return const char*(mac_errorBundle());
#else
		return "";
#endif
#else
		const char* ret = (char*)dlerror();
		return ret;
#endif
	}
}