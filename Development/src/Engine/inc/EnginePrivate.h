/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

//-----------------------------------------------------------------------------
// Visual Studio Warnings
//-----------------------------------------------------------------------------
#pragma warning(disable: 4267) /// convert type1->type2
#pragma warning(disable: 4390)
#pragma warning(disable: 4305)
//-----------------------------------------------------------------------------
// Engine public includes.
//-----------------------------------------------------------------------------
//Render.dll в нем уже включен модуль интерфейсов,в который уже включен Core
#include "../../Render/inc/RenderPrivate.h"
//AI.dll
#include "../../AI/AIPrivate.h"

//external
#include "MyGUI_OgrePlatform.h"
#include "MyGUI.h"

#if ENGINE_PLATFORM == ENGINE_PLATFORM_ANDROID
#define ENGINE_API 
#elif ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif 
#endif

//Main Engine
#include "EngineGlobals.h"
//Entity
#include "world/Actor.h"