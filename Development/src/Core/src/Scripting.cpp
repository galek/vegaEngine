/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "CorePrivate.h"
#include "lua\lua.hpp"
#include "Scripting.h"
#include "WrappedScriptFunctions.h"
#include "luabind\luabind.hpp"
#pragma message("NICK-REFACTOR IT 2015")
namespace vega
{

	//-------------------------------------------------------------------------------------
	Scripting::~Scripting() {
		lua_close(luaState);
	}

	//-------------------------------------------------------------------------------------
	void Scripting::Initialize()
	{
		luaState = luaL_newstate();
		luaL_openlibs(luaState);
		// Connect LuaBind to this lua state
		luabind::open(luaState);
		bindLogFunctions();
		bindMathFunctions();
		bindCoreFunctions();
	}

	//-------------------------------------------------------------------------------------
	lua_State * Scripting::getLua() const {
		return luaState;
	}

	//-------------------------------------------------------------------------------------
	int Scripting::doString(std::string code) const {
		Debug("Loaded script: %s \n", code.c_str());
		if (!luaL_loadbuffer(luaState, code.c_str(), code.length(), "line"))
		{
			if (lua_pcall(luaState, 0, 0, 0)){
				SeriousWarning(true, "[LUA]error running function: %s", lua_tostring(luaState, -1));
				lua_pop(luaState, 1);
				return 1;
			}
		}
		return 0;
	}

	//-------------------------------------------------------------------------------------
	int Scripting::RunScript(const char* _filename){
		VFile file;
		return doString(file.LoadFileHowString(_filename));
	}

	//-------------------------------------------------------------------------------------
	void Scripting::bindLogFunctions() {
		lua_register(luaState, "Debug", ScriptDebug);
		lua_register(luaState, "LogPrintf", ScriptLogPrintf);
		lua_register(luaState, "Warning", ScriptWarning);
		lua_register(luaState, "SeriousWarning", ScriptSeriousWarning);
		using namespace luabind;
		module(luaState)
			[
				def("SetLoggingLevel", &SetLoggingLevel),
				def("GetLoggingLevel", &GetLoggingLevel)
			];
	}

	//-------------------------------------------------------------------------------------
	void Scripting::bindMathFunctions() {
		TODO("Vector2,Matrix");
		using namespace Ogre;
		using namespace luabind;
		module(luaState)
			[
				class_<Vector3>("Vector3")
				.def_readwrite("x", &Vector3::x)
				.def_readwrite("y", &Vector3::y)
				.def_readwrite("z", &Vector3::z)
				.def(constructor<>())
				.def(constructor<Vector3&>())
				.def(constructor<Real, Real, Real>())
				.def("absDotProduct", &Vector3::absDotProduct)
				.def("crossProduct", &Vector3::crossProduct)
				.def("directionEquals", &Vector3::directionEquals)
				.def("distance", &Vector3::distance)
				.def("dotProduct", &Vector3::dotProduct)
				.def("getRotationTo", &Vector3::getRotationTo)
				.def("isZeroLength", &Vector3::isZeroLength)
				.def("length", &Vector3::length)
				.def("makeCeil", &Vector3::makeCeil)
				.def("makeFloor", &Vector3::makeFloor)
				.def("midPoint", &Vector3::midPoint)
				.def("normalise", &Vector3::normalise)
				.def("nornaliseCopy", &Vector3::normalisedCopy)
				.def("perpendicular", &Vector3::perpendicular)
				.def("positionCloses", &Vector3::positionCloses)
				.def("positionEquals", &Vector3::positionEquals)
				.def("randomDeviant", &Vector3::randomDeviant)
				.def("reflect", &Vector3::reflect)
				.def("squaredDistance", &Vector3::squaredDistance)
				.def("squaredLength", &Vector3::squaredLength)
				//ColourValue
				, class_<ColourValue>("ColourValue")
				.def(constructor<>())
				.def(constructor<Real, Real, Real, Real>())
				.def(constructor<Real, Real, Real>())
				.def_readwrite("r", &ColourValue::r)
				.def_readwrite("g", &ColourValue::g)
				.def_readwrite("b", &ColourValue::b)
				.def_readwrite("a", &ColourValue::a)
				.def("saturate", &ColourValue::saturate)
			];
	}

	//-------------------------------------------------------------------------------------
	void Scripting::bindCoreFunctions() {
		using namespace luabind;
		using namespace vega;
		module(luaState)
			[
				//VFile
				class_ <VFile>("VFile")
				.def(luabind::constructor<>())
				.def("LoadFileHowCChar", (&VFile::LoadFileHowCChar))
				.def("LoadFileAndGetSize", (&VFile::LoadFileAndGetSize))
				.def("LoadFileHowString", (&VFile::LoadFileHowString))
				.def("GetHowCChar", (&VFile::GetHowCChar))
				.def("GetSize", (&VFile::GetSize))
				,
				//EngineConfig
				class_<EngineConfig>("EngineConfig")
				.def(luabind::constructor<>())
				.def_readwrite("GlobalIllumination", &EngineConfig::mGlobalIllumination)
				.def_readwrite("SSAO", &EngineConfig::mSSAO)
				.def_readwrite("HDR", &EngineConfig::mHDR)
				.def_readwrite("DOF", &EngineConfig::mDOF)
				.def_readwrite("ShadowFarDistance", &EngineConfig::mShadowFarDistance)
				.def_readwrite("ShadowTextureSize", &EngineConfig::mShadowTextureSize)
				.def_readwrite("FarClipDistance", &EngineConfig::mFarClipDistance)
				.def_readwrite("PrecacheResources", &EngineConfig::mPrecacheResources)
				.def_readwrite("mAntiAlliasing", &EngineConfig::mAntiAlliasing)
				.def_readwrite("mRenderAPI", &EngineConfig::mRenderAPI)
				.enum_("AntiAlliasing")
				[
					value("AA_NONE", EngineConfig::AntiAlliasing::AA_NONE),
					value("AA_SMAA", EngineConfig::AntiAlliasing::AA_SMAA),
					value("AA_SSAA", EngineConfig::AntiAlliasing::AA_SSAA),
					value("AA_FXAA", EngineConfig::AntiAlliasing::AA_FXAA)
				]
				.enum_("RenderAPI")
					[
						value("RENDER_NONE", EngineConfig::RenderAPI::RENDER_NONE),
						value("RENDER_OGL", EngineConfig::RenderAPI::RENDER_OGL),
						value("RENDER_DX9", EngineConfig::RenderAPI::RENDER_DX9),
						value("RENDER_DX11", EngineConfig::RenderAPI::RENDER_DX11),
						value("RENDER_COUNT", EngineConfig::RenderAPI::RENDER_COUNT)
					]
			];
	}

	//-------------------------------------------------------------------------------------
	int Scripting::RunFunctionByName(const char*_name) {
		lua_getglobal(luaState, _name);
		return lua_pcall(luaState, 0, 0, 0);
	}
}