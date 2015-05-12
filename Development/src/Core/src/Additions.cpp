/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"


namespace vega
{
#ifdef USE_STEAMWORKS
	/**
	SteamWorks
	*/
	bool InitSteamWorks();
	void DestroySteamWorks();
#endif

	/**
	*/
	bool InitAdditions()
	{
#ifdef USE_STEAMWORKS
		if(!InitSteamWorks())
			return false;
#endif
		return true;
	}

	/**
	*/
	void DestroyAdditions(){
#ifdef USE_STEAMWORKS
		DestroySteamWorks();
#endif
	}
}