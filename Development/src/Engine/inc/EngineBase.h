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
	//-----------------------------------------------------------------------------
	// Base engine class.
	//-----------------------------------------------------------------------------
	class ENGINE_API EngineBase 
	{
	public:
		EngineBase(void);
		virtual ~EngineBase(void);
		void Go(void);
	protected:
		void SetGame(iGame* _game);
		// !@Proxy function
		void LoadEngineModule(const char *_moduleName);
	};
}