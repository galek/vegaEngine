/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "SceneManager.h"
#include "Render.h"
#include "Console.h"
#include "EngineScriptingEx.h"

namespace vega
{
	/**
	*/
	namespace LowLevelConsoleCommands
	{
		// !@Low Level Pause command
		void Pause(bool _st)
		{
			if (_st)
				GetEngine()->GetAudio()->pauseAllSounds();
			else
				GetEngine()->GetAudio()->resumeAllSounds();
			
			GetEngine()->SetPause(_st);
		}
	}
	/**
	*/
	namespace ConsoleCommands
	{
		// !@Loading Level command
		void LoadLevel(const MyGUI::UString & _key, const MyGUI::UString & _value) {

			std::string path = _value + ".level";
			if (_key == "loadlevel")
				if (!_value.empty())
					GetEngine()->GetSceneMgr()->LoadLevel(path.c_str(), true);

		}
		// !@Loading Level command
		void PlayV(const MyGUI::UString & _key, const MyGUI::UString & _value) {
			GetEngine()->GetRender()->PlayVideo(_value.asUTF8_c_str());
		}
		// !@Runing script
		void RunScript(const MyGUI::UString & _key, const MyGUI::UString & _value) {
			std::string path = _value + ".lua";
			if (_key == "runscript")
				if (!_value.empty())
					GetEngine()->mScript->RunScript(path.c_str());
		}
		// !@Help command
		void Help(const MyGUI::UString & _key, const MyGUI::UString & _value) {
			GetEngine()->GetConsole()->GetListCommands();
		}
		// !@EngineShoutdown
		void EngineShoutdown(const MyGUI::UString & _key, const MyGUI::UString & _value) {
			GetEngine()->EngineShoutdown();
		}
		// !@EngineShoutdown
		void CleanConsole(const MyGUI::UString & _key, const MyGUI::UString & _value) {
			GetEngine()->GetConsole()->CleanConsole();
		}
		// !@R_AA command
		void R_AA(const MyGUI::UString & _key, const MyGUI::UString & _value) {
			int status = 0;
			int currentSt = GetEngine()->mEngineConfig->mAntiAlliasing;
			if (!_value.empty()){
				MyGUI::utility::parseComplex(_value, status);

				switch (status)
				{
				case 0:
					if (currentSt == 1){
						GetEngine()->GetRender()->PostEffectSetStatus("SMAA", 0);
						Debug("[AA]Deactivated SMAA-Engine");
					}
					if (currentSt == 2){
						GetEngine()->GetRender()->PostEffectSetStatus("SSAA", 0);
						Debug("[AA]Deactivated SSAA-Engine");
					}
					if (currentSt == 3){
						GetEngine()->GetRender()->PostEffectSetStatus("FXAA", 0);
						Debug("[AA]Deactivated SSAA-Engine");
					}
					GetEngine()->mEngineConfig->mAntiAlliasing = (EngineConfig::AntiAlliasing)0;
					break;
				case 1:
					Debug("[AA]Activated SMAA-Engine");
					GetEngine()->GetRender()->PostEffectSetStatus("SMAA", 1);
					GetEngine()->mEngineConfig->mAntiAlliasing = (EngineConfig::AntiAlliasing)1;
					break;
				case 2:
					Debug("[AA]Activated SSAA-Engine");
					GetEngine()->GetRender()->PostEffectSetStatus("SSAA", 1);
					GetEngine()->mEngineConfig->mAntiAlliasing = (EngineConfig::AntiAlliasing)2;
					break;
				case 3:
					Debug("[AA]Activated FXAA-Engine");
					GetEngine()->GetRender()->PostEffectSetStatus("FXAA", 1);
					GetEngine()->mEngineConfig->mAntiAlliasing = (EngineConfig::AntiAlliasing)3;
					break;
				}
			}
		}
		// !@R_HDR command
		void R_HDR(const MyGUI::UString & _key, const MyGUI::UString & _value) {
			bool status = false;
			if (!_value.empty()){
				MyGUI::utility::parseComplex(_value, status);
				GetEngine()->GetRender()->PostEffectSetStatus("HDR", status);
			}
		}
		// !@R_SSAO command
		void R_SSAO(const MyGUI::UString & _key, const MyGUI::UString & _value) {
			bool status = false;
			if (!_value.empty()){
				MyGUI::utility::parseComplex(_value, status);
				GetEngine()->GetRender()->PostEffectSetStatus("SSAO", status);
			}
		}
		// !@E_Pause command
		void E_Pause(const MyGUI::UString & _key, const MyGUI::UString & _value) {
			bool status = false;
			if (!_value.empty()){
				MyGUI::utility::parseComplex(_value, status);
				using namespace LowLevelConsoleCommands;
				Pause(status);
			}
		}
		// !@ReloadCfg command
		void ReloadCfg(const MyGUI::UString & _key, const MyGUI::UString & _value){
			GetEngine()->AttemptConfig();
		}
	}
}