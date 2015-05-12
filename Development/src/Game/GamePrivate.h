#pragma once

// Engine Interfaces
#include "../Common/inc/IEngine.h"

#if ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
#define ENGINE_API __declspec(dllimport)
#elif ENGINE_PLATFORM == ENGINE_PLATFORM_ANDROID
#define ENGINE_API 
#endif

// Engine.dll
#include "../Engine/inc/EngineBase.h"

// Core.dll Log writting
#include "../Core/inc/log.h"
#include "../Core/inc/buildconfig.h"

#if ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
#ifdef GAME_EXPORTS
#define GAME_API __declspec(dllexport)
#else
#define GAME_API __declspec(dllimport)
#endif
#elif ENGINE_PLATFORM == ENGINE_PLATFORM_ANDROID
#define GAME_API
#endif
//GameDll
#include "Game.h"
#include "GameApp.h"