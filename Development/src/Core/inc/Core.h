/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

#if ENGINE_PLATFORM == ENGINE_PLATFORM_ANDROID
#define CORE_API 
#elif ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  { if (p) { delete (p); (p)=0; } } 
#endif

#ifndef _STR
#define _STR(x) #x
#endif

#ifndef STR
#define STR(x) _STR(x)
#endif

#if ENGINE_PLATFORM == ENGINE_PLATFORM_ANDROID
#define TODO(x) 
#elif ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
#define TODO(x) __pragma(message("TODO: "_STR(x) " :: " __FILE__ "@"STR(__LINE__)))
#endif
#if ENGINE_PLATFORM == ENGINE_PLATFORM_ANDROID
#define BUG(x) 
#elif ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
#define BUG(x) __pragma(message("BUG: "_STR(x) " :: " __FILE__ "@"STR(__LINE__)))
#endif