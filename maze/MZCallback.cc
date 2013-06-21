// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
Callback::Callback()
	: mScript(NULL),
	  mFunc(LUA_NOREF)
{
}

// ------------------------------------------------------------------------------------------------
Callback::Callback(lua_State *L, int ref)
	: mScript(L)
{
	lua_rawgeti(mScript, LUA_REGISTRYINDEX, ref);
	mFunc = luaL_ref(mScript, LUA_REGISTRYINDEX);
}

// ------------------------------------------------------------------------------------------------
Callback::Callback(const Callback& call)
	: mScript(call.mScript)
{
	lua_rawgeti(mScript, LUA_REGISTRYINDEX, call.mFunc);
	mFunc = luaL_ref(mScript, LUA_REGISTRYINDEX);
}

// ------------------------------------------------------------------------------------------------
Callback::~Callback()
{
	if (mScript && mFunc != LUA_NOREF)
	{
		luaL_unref(mScript, LUA_REGISTRYINDEX, mFunc);
	}
}

// ------------------------------------------------------------------------------------------------
void Callback::Call(Entity *who)
{
	if (mScript && mFunc != LUA_NOREF)
	{
		lua_rawgeti(mScript, LUA_REGISTRYINDEX, mFunc);
		*(Entity**)mzlCreateObject(mScript, sizeof(Entity**), "entity") = who;

		if (lua_pcall(mScript, 1, 0, 0))
		{
			throw Exception("[Script] ") << lua_tostring(mScript, -1);
		}
	}
}

// ------------------------------------------------------------------------------------------------
void Callback::Call(float time, float dt)
{
	if (mScript && mFunc != LUA_NOREF)
	{
		lua_rawgeti(mScript, LUA_REGISTRYINDEX, mFunc);
		lua_pushnumber(mScript, time);
		lua_pushnumber(mScript, dt);

		if (lua_pcall(mScript, 2, 0, 0))
		{
			throw Exception("[Script] ") << lua_tostring(mScript, -1);
		}
	}
}

// ------------------------------------------------------------------------------------------------        
void Callback::operator = (const Callback& call)
{
	if (mFunc)
	{
		luaL_unref(mScript, LUA_REGISTRYINDEX, mFunc);
	}

	mScript = call.mScript;

	lua_rawgeti(mScript, LUA_REGISTRYINDEX, call.mFunc);
	mFunc = luaL_ref(mScript, LUA_REGISTRYINDEX);
}