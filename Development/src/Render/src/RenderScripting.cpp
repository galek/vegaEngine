/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "RenderPrivate.h"
//Externals
#include "lua\lua.hpp"
#include "luabind\luabind.hpp"
//Core
#include "Scripting.h"

namespace vega
{
	void RENDER_API _RenderAddScriptFunctionaluty(Scripting* mScript)
	{
		::lua_State*_luaState = mScript->getLua();
		using namespace luabind;
		using namespace vega;
		using namespace Ogre;
		module(_luaState)
			[
				//GlobalFunctions
				//Render
				class_ <Render>("Render")
				.def("PlayVideo", &Render::PlayVideo)
				.def("EnableDisableAA", &Render::EnableDisableAA)
				.def("GetPlayer", &Render::GetPlayer)
				.def("PostEffectSetStatus", &Render::PostEffectSetStatus)
				//VideoPlayer
				, class_ <VideoPlayer>("VideoPlayer")
				.def("playVideo", &VideoPlayer::playVideo)
				.def("Update", &VideoPlayer::Update)
				.def("close", &VideoPlayer::close)
				.def("stopVideo", &VideoPlayer::stopVideo)
				.def("isPlaying", &VideoPlayer::isPlaying)
				.def("setResolution", &VideoPlayer::setResolution)
			];
	}
}