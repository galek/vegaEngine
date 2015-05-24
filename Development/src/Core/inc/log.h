/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

#include "Core.h"


//Не надо переносить это в отдельный класс,т.к будет необходимость делать инклуд этого класса,а здесь напрямую функции 
//могут вызываться из любого фрагмента кода
/// Report warning without terminating program (stops program until user responds).
CORE_API void Warning(const char *fmt, ...);

/// Report error and terminate program. Returns S_OK to shut up functions. Will never really return.
CORE_API void ErrorFunction(bool _toLog,const char *fmt, const char *_file, int _line, ...);
#ifndef ErrorF
#if ENGINE_PLATFORM != ENGINE_PLATFORM_ANDROID
#define ErrorF(x) ErrorFunction(true,x,__FILE__,__LINE__)
#define ErrorTrace() ErrorFunction(true,__FUNCTION__,__FILE__,__LINE__)
#else
#define ErrorF(x) 
#define ErrorTrace()
#endif
#endif

/// Serious warnings, always show MB
CORE_API void SeriousWarning(bool _show, const char *fmt, ...);
/// Report a message to the user for debug-only builds
CORE_API void Debug(const char *fmt, ...);
/// Writing in Log
CORE_API void LogPrintf(const char *fmt, ...);
/// LL_LOW = 1,LL_NORMAL = 2,LL_BOREME = 3
CORE_API void SetLoggingLevel(int _level);
///
CORE_API int GetLoggingLevel();