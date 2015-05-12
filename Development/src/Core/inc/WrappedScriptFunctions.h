/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once
#include "Log.h"

__inline void ScriptSetLoggingLevel(int n)
{
	SetLoggingLevel(n);
}

__inline static int ScriptLogPrintf(lua_State *L)
{
	int n = lua_gettop(L);
	int i;
	for (i = 1; i <= n; i++)
	{
		if (i > 1) LogPrintf("\t");
		if (lua_isstring(L, i))
			LogPrintf(lua_tostring(L, i));
		else if (lua_isnil(L, i))
			LogPrintf("nil");
		else if (lua_isboolean(L, i))
			LogPrintf("%s", lua_toboolean(L, i) ? "true" : "false");
		else
			LogPrintf("%s:%p", luaL_typename(L, i), lua_topointer(L, i));
	}
	return 0;
}

__inline static int ScriptWarning(lua_State *L)
{
	int n = lua_gettop(L);
	int i;
	for (i = 1; i <= n; i++)
	{
		if (i > 1) Warning("\t");
		if (lua_isstring(L, i))
			Warning(lua_tostring(L, i));
		else if (lua_isnil(L, i))
			Warning("nil");
		else if (lua_isboolean(L, i))
			Warning("%s", lua_toboolean(L, i) ? "true" : "false");
		else
			Warning("%s:%p", luaL_typename(L, i), lua_topointer(L, i));
	}
	return 0;
}
__inline static int ScriptSeriousWarning(lua_State *L)
{
	int n = lua_gettop(L);
	int i;
	for (i = 1; i <= n; i++)
	{
		if (i > 1) SeriousWarning(true, "\t");
		if (lua_isstring(L, i))
			SeriousWarning(true, lua_tostring(L, i));
		else if (lua_isnil(L, i))
			SeriousWarning(true, "nil");
		else if (lua_isboolean(L, i))
			SeriousWarning(true, "%s", lua_toboolean(L, i) ? "true" : "false");
		else
			SeriousWarning(true, "%s:%p", luaL_typename(L, i), lua_topointer(L, i));
	}
	return 0;
}

__inline static int ScriptDebug(lua_State *L)
{
	int n = lua_gettop(L);
	int i;
	for (i = 1; i <= n; i++)
	{
		if (i > 1) Debug("\t");
		if (lua_isstring(L, i))
			Debug(lua_tostring(L, i));
		else if (lua_isnil(L, i))
			Debug("nil");
		else if (lua_isboolean(L, i))
			Debug("%s", lua_toboolean(L, i) ? "true" : "false");
		else
			Debug("%s:%p", luaL_typename(L, i), lua_topointer(L, i));
	}
	return 0;
}