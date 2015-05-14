/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.6.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

struct lua_State;

namespace vega
{
	struct CORE_API Scripting
	{
	public:
		Scripting(){}

		~Scripting();

		::lua_State* getLua() const;
		//0 if sucessfully
		int RunScript(const char*_filename);
		//0 if sucessfully
		int RunFunctionByName(const char*_name);
		void Initialize();
	private:
		//0 if sucessfully
		int doString(std::string code) const;
		void bindLogFunctions();
		void bindMathFunctions();
		void bindCoreFunctions();
	private:
		::lua_State * luaState;
	};
}