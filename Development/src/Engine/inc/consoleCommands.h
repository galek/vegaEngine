/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

namespace vega
{
	namespace ConsoleCommands
	{
		// !@Loading Level command
		void LoadLevel(const MyGUI::UString & _key, const MyGUI::UString & _value);
		//Команда для тестирования
		// !@Loading Level command
		void PlayV(const MyGUI::UString & _key, const MyGUI::UString & _value);
		// !@Help command
		void Help(const MyGUI::UString & _key, const MyGUI::UString & _value);
		// !@EngineShoutdown
		void EngineShoutdown(const MyGUI::UString & _key, const MyGUI::UString & _value) ;
		// !@EngineShoutdown
		void CleanConsole(const MyGUI::UString & _key, const MyGUI::UString & _value);
		void RunScript(const MyGUI::UString & _key, const MyGUI::UString & _value);
		void R_AA(const MyGUI::UString & _key, const MyGUI::UString & _value);
		void R_HDR(const MyGUI::UString & _key, const MyGUI::UString & _value);
		void R_SSAO(const MyGUI::UString & _key, const MyGUI::UString & _value);
		void E_Pause(const MyGUI::UString & _key, const MyGUI::UString & _value);
		void ReloadCfg(const MyGUI::UString & _key, const MyGUI::UString & _value);
	}
}