/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "APIEngineCommands.h"

namespace vega
{
	namespace API{
		//-------------------------------------------------------------------------------------
		void RunScript(const char*_a)	{
			GetEngine()->RunScript(_a);
		}
		//-------------------------------------------------------------------------------------
		void SetPause(bool _a)	{
			GetEngine()->SetPause(_a);
		}
		//-------------------------------------------------------------------------------------
		void RunConsoleCommand(const char*_str) {
			GetEngine()->RunConsoleCommand(_str);
		}
		//-------------------------------------------------------------------------------------
		void PlayVideo(const char*_str, bool _abort) {
			if (!GetEngine()){
				Warning("[API::PlayVideo]Engine is not initiliased");
				return;
			}
			VideoPlayer*vplayer = GetEngine()->GetRender()->GetPlayer();
			if (vplayer)
				vplayer->playVideo(_str, _abort);
			else
				Warning("[API::PlayVideo]VideoPlayer is not initiliased");
		}
		//-------------------------------------------------------------------------------------
		bool IsPlayingVideo() {
			if (!GetEngine()){
				Warning("[API::PlayVideo]Engine is not initiliased");
				return false;
			}
			VideoPlayer*vplayer = GetEngine()->GetRender()->GetPlayer();
			return vplayer->isPlaying();
		}
		//-------------------------------------------------------------------------------------
		void LoadLevel(const char*_name, bool _new) {
			GetEngine()->GetSceneMgr()->LoadLevel(_name, _new);
		}
		//-------------------------------------------------------------------------------------
		void EngineShoutdown() {
			GetEngine()->EngineShoutdown();
		}
		//-------------------------------------------------------------------------------------
		void AttemptConfig() {
			GetEngine()->AttemptConfig();
		}
		//-------------------------------------------------------------------------------------
		void SetMouseLock(bool _status) {
			GetEngine()->GetInput()->SetMouseLock(_status);
		}
		//-------------------------------------------------------------------------------------
		void SetBufferedUnBufferedMouseMode(bool _status, bool _showGUICusor) {
			GetEngine()->GetInput()->Recreate(_status);
			GetEngine()->GetGui()->ShowGUIMouseCursor(_showGUICusor);
		}
		//-------------------------------------------------------------------------------------
		void SetShowGUICursor(bool _status) {
			GetEngine()->GetGui()->ShowGUIMouseCursor(_status);
		}
		//-------------------------------------------------------------------------------------
		void RunScriptFunctionByName(const char*_file,const char* _status) {
			RunScript(_file);
			GetEngine()->mScript->RunFunctionByName(_status);
		}
	}
}