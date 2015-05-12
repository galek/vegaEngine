/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#pragma message("NICK-REFACTOR IT 2015")
namespace vega
{
	//-----------------------------------------------------------------------------
	// For plugin system...
	typedef void (*DLL_START_PLUGIN)(void);
	typedef void (*DLL_STOP_PLUGIN)(void);
	//-----------------------------------------------------------------------------
	void EnginePlugins::LoadEngineModule(const char *modulename)
	{
		// Load plugin library
		DynLib* module = new DynLib((modulename));
		module->Load();
		// Store for later unload
		modules.push_back(module);

		// Call startup function
		DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)module->GetSymbol("dllStartPlugin");

		if (!pFunc)
			Warning("Cannot find symbol dllStartPlugin in library %s",modulename);
		else
		{
			Debug("Found symbol dllStartPlugin in library %s",modulename);
			pFunc();
		};
	};


}