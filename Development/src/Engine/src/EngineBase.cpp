/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "EngineBase.h"
#include "EnginePlugins.h"
#include "OgreStaticPluginLoader.h"

namespace vega
{
	ENGINE_API EngineGlobals engine;
	EngineGlobals*GetEngine(){
		return &engine;
	}
	//-------------------------------------------------------------------------------------
	void SetEngine(EngineGlobals&_engine) {
		engine = _engine;
	}
	//-------------------------------------------------------------------------------------
	EngineBase::EngineBase(void)
	{}
	//-------------------------------------------------------------------------------------
	EngineBase::~EngineBase(void)
	{
		GetEngine()->EngineShoutdown();
	}
	//-------------------------------------------------------------------------------------
	void EngineBase::Go(void)	{
		GetEngine()->Go();
	}
	//-------------------------------------------------------------------------------------
	void EngineBase::LoadEngineModule(const char *_moduleName)	{
		GetEngine()->GetPlugins()->LoadEngineModule((_moduleName));
	}
	//-------------------------------------------------------------------------------------
	void EngineBase::SetGame(iGame* _game)	{
		GetEngine()->SetGame(_game);
	}
	//-------------------------------------------------------------------------------------
}