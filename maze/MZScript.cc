// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2013 The MAZE project. All rights reserved.

#include <cstdio>
#include "MZException.h"
#include "MZEngine.h"
#include "MZLog.h"
#include "MZScript.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
static int LuaInclude(lua_State* L)
{
	Script* script;
	std::string fn;

	lua_getglobal(L, "__state");
	script = (Script*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	if (!script)
	{
		lua_pushstring(L, "'__state' not found");
		lua_error(L);
	}
	
	if (lua_gettop(L) != 1)
	{
		lua_pushstring(L, "'include' takes 1 argument");
		lua_error(L);
	}

	fn = script->GetScriptDir().append(lua_tostring(L, 1));
	if (luaL_loadfile(L, fn.c_str()) || lua_pcall(L, 0, 0, 0))
	{
		lua_error(L);
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------
static int LuaPrint(lua_State* L)
{
	std::string message;
	
	for (size_t i = 1, top = lua_gettop(L); i <= top; ++i)
	{
		if (lua_isstring(L, i) || lua_isnumber(L, i))
		{
			message.append(lua_tostring(L, i));
		}
	}

	Log::Inst() << "[Script] " << message;
	return 0;
}

// ------------------------------------------------------------------------------------------------
Script::Script(Engine *engine)
	: mEngine(engine)
{
	mScriptDir = mEngine->GetConfig().ResourceDir + "/script/";

	if (!(mState = luaL_newstate()))
	{
		throw Exception("Cannot create lua state");
	}

	luaL_openlibs(mState);

	lua_pushlightuserdata(mState, this);
	lua_setglobal(mState, "__state");
	
	lua_pushcfunction(mState, LuaInclude);
	lua_setglobal(mState, "include");

	lua_pushcfunction(mState, LuaPrint);
	lua_setglobal(mState, "print");

	Log::Inst() << "[Script] " << LUA_VERSION;
}

// ------------------------------------------------------------------------------------------------
Script::~Script()
{
	lua_close(mState);
}

// ------------------------------------------------------------------------------------------------
void Script::Execute(const std::string& func)
{
	lua_getglobal(mState, func.c_str());
	if (!lua_isfunction(mState, -1))
	{
		lua_pop(mState, 1);
		Log::Inst() << "[Script] '" << func << "' is not callable";
		throw Exception("Callback not found: '" + func + "'");
	}
	
	if (lua_pcall(mState, 0, 0, 0))
	{
		Log::Inst() << "[Script] '" << lua_tostring(mState, -1);
		throw Exception("Callback failed: '" + func + "'");
	}
}

// ------------------------------------------------------------------------------------------------
void Script::Include(const std::string& script)
{
	if (luaL_loadfile(mState, (mScriptDir + script).c_str()))
	{
		Log::Inst() << "[Script] " << lua_tostring(mState, -1);
		throw Exception("Script compilation failed: '" + script + "'");
	}

	if (lua_pcall(mState, 0, 0, 0))
	{
		Log::Inst() << "[Script] " << lua_tostring(mState, -1);
		throw Exception("Script compilation failed: '" + script + "'");
	}
}