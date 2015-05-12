/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

namespace vega
{
	class EnginePlugins
	{	
	public:
		// Loads Engine Module from dll
		void LoadEngineModule(const char *modulename);
		// Plug-In dynamic libraries, platform-independent
		std::vector<class DynLib*> modules;
	};
}