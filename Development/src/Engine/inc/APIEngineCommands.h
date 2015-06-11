/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

#include "EnginePrivate.h"

namespace vega
{
	namespace API{
		/*return 0 if successfully*/
		ENGINE_API int RunScript(const char*_a);
		ENGINE_API void RunScriptFunctionByName(const char*_file, const char*_a);
		ENGINE_API bool GetShowGUICursor();
		ENGINE_API bool IsBufferredInput();
		ENGINE_API void SetPause(bool _a);
		ENGINE_API void RunConsoleCommand(const char*_str);
		ENGINE_API void PlayVideo(const char*_str, bool _abort);
		ENGINE_API bool IsPlayingVideo();
		ENGINE_API void LoadLevel(const char*_name, bool _new);
		ENGINE_API void EngineShoutdown();
		ENGINE_API void AttemptConfig();
		ENGINE_API void SetMouseLock(bool _lock);
		ENGINE_API void SetBufferedUnBufferedMouseMode(bool _lock,bool _showGUICusor);
		ENGINE_API void SetShowGUICursor(bool _lock);
	}
}