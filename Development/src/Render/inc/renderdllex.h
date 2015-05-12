/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#if ENGINE_PLATFORM == ENGINE_PLATFORM_WINDOWS
#ifdef RENDER_EXPORTS
#define RENDER_API __declspec(dllexport)
#else
#define RENDER_API __declspec(dllimport)
#endif
#elif ENGINE_PLATFORM == ENGINE_PLATFORM_ANDROID
#define RENDER_API
#endif