/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"

#ifdef USE_STEAMWORKS
#include "steamworks/public/steam/steam_api.h"


#ifdef STEAM_CEG
// Steam DRM header file
#include "cegclient.h"
#else
#define Steamworks_InitCEGLibrary() (true)
#define Steamworks_TermCEGLibrary()
#define Steamworks_TestSecret()
#define Steamworks_SelfCheck()
#endif
#ifdef WIN32
#include <direct.h>
#else
#define MAX_PATH PATH_MAX
#define _getcwd getcwd
#endif

#pragma comment(lib,"steam_api")
#pragma message("NICK-REFACTOR IT 2015")
namespace vega
{
	/**
	*/
	int Alert(const char *lpCaption, const char *lpText)
	{
#ifndef _DEVELOP
#ifndef _WIN32
		fprintf(stderr, "Message: '%s', Detail: '%s'\n", lpCaption, lpText);
		return 0;
#else
		return ::MessageBox(NULL, lpText, lpCaption, MB_OK);
#endif
#else
		return 0;
#endif
	}

	/**
	*/
	extern "C" void __cdecl SteamAPIDebugTextHook(int nSeverity, const char *pchDebugText)
	{
		// if you're running in the debugger, only warnings (nSeverity >= 1) will be sent
		// if you add -debug_steamapi to the command-line, a lot of extra informational messages will also be sent
		::OutputDebugString(pchDebugText);

		if (nSeverity >= 1)
		{
			// place to set a breakpoint for catching API errors
			int x = 3;
			x = x;
		}
	}

	/**
	*/
	bool InitSteamWorks()
	{
		if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid))
			return false;

		if (!Steamworks_InitCEGLibrary())
		{
			::OutputDebugString("Steamworks_InitCEGLibrary() failed\n");
			Alert("Fatal Error", "Steam must be running to play this game.\n");
			return false;
		}

		if (!SteamAPI_Init())
		{
			::OutputDebugString("SteamAPI_Init() failed\n");
			Alert("Fatal Error", "Steam must be running to play this game.\n");
			return false;
		}

		SteamClient()->SetWarningMessageHook(&SteamAPIDebugTextHook);
		SteamUtils()->SetOverlayNotificationPosition(k_EPositionTopRight);

		// do a DRM self check
		Steamworks_SelfCheck();
		return true;
	}

	/**
	*/
	void DestroySteamWorks(){
		if (SteamAPI_IsSteamRunning())
			SteamAPI_Shutdown();
		Steamworks_TermCEGLibrary();
	}
}
#endif