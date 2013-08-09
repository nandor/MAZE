// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
void MAZE::mzlClass(lua_State* L, const std::string& name, const std::string& base, 
					lua_CFunction ctor, const luaL_Reg *s, const luaL_Reg *m)
{
	// Initialize the static table
	s ? luaL_newlib(L, s) : lua_createtable(L, 0, 1);
	lua_pushvalue(L, -1);
	lua_setglobal(L, name.c_str());
	
	// Attach a metatable with the __call method
	if (ctor != NULL)
	{
		lua_createtable(L, 0, 1);
		lua_pushcfunction(L, ctor);
		lua_setfield(L, -2, "__call");
		lua_setmetatable(L, -2);
	}
	
	lua_pop(L, 1);

	// Initialize the metatable
	m ? luaL_newlib(L, m) : lua_createtable(L, 0, 5);

	lua_pushstring(L, name.c_str());
	lua_setfield(L, -2, "__type");
	
	lua_getglobal(L, (base + "__meta").c_str());
	lua_setfield(L, -2, "__base");

	lua_pushcfunction(L, mzlDefaultGetter);
	lua_setfield(L, -2, "__index");

	lua_pushcfunction(L, mzlDefaultSetter);
	lua_setfield(L, -2, "__newindex");

	lua_setglobal(L, (name + "__meta").c_str());
}

// ------------------------------------------------------------------------------------------------
int MAZE::mzlDefaultPrint(lua_State *L)
{
	std::stringstream ss;

	for (size_t i = 1, sz = lua_gettop(L); i <= sz; ++i)
	{
		switch (lua_type(L, i))
		{
			case LUA_TNONE: 
			{
				ss << "none"; 
				break;
			}
			case LUA_TNIL:
			{
				ss << "nil"; 
				break;
			}
			case LUA_TBOOLEAN: 
			{
				ss << (lua_toboolean(L, i) ? "true" : "false"); 
				break;
			}
			case LUA_TLIGHTUSERDATA: 
			{
				ss << "<0x" << std::hex << lua_touserdata(L, i) << std::dec << ">";
				break;
			}
			case LUA_TNUMBER: 
			{
				ss << lua_tonumber(L, i); 
				break;
			}
			case LUA_TSTRING: 
			{
				ss << lua_tostring(L, i); 
				break;
			}
			case LUA_TTABLE:
			case LUA_TUSERDATA:
			{
				lua_getmetatable(L, i);
				if (lua_istable(L, -1))
				{
					lua_getfield(L, -1, "__tostring");
					if (lua_isfunction(L, -1))
					{
						lua_pushvalue(L, i);
						lua_call(L, 1, 1);

						if (lua_isstring(L, -1))
						{
							ss << lua_tostring(L, -1);
						}

						lua_pop(L, 1);
					}
				}
				
				lua_pop(L, 1);
				break;
			}
			default:
			{
				continue;
			}
		}
	}

	Log::Inst() << "[Script] " << ss.str();

	return 0;
}
// ------------------------------------------------------------------------------------------------
int MAZE::mzlDefaultGetter(lua_State *L)
{
	lua_getmetatable(L, 1);

	while (lua_istable(L, -1))
	{
		lua_pushstring(L, lua_tostring(L, 2));
		lua_rawget(L, -2);

		if (!lua_isnil(L, -1))
		{
			lua_remove(L, -2);
			return 1;
		}

		lua_pop(L, 1);
		lua_pushstring(L, "__getter");
		lua_rawget(L, -2);

		if (!lua_isnil(L, -1))
		{
			int top = lua_gettop(L) - 1;

			lua_pushvalue(L, 1);
			lua_pushvalue(L, 2);
			lua_call(L, 2, LUA_MULTRET);
			
			if (lua_gettop(L) - top > 0)
			{
				return 1;
			}
		}
		else
		{
			lua_pop(L, 1);
		}

		lua_pushstring(L, "__base");
		lua_rawget(L, -2);
	}

	return 0;
}
	
// ------------------------------------------------------------------------------------------------
int MAZE::mzlDefaultSetter(lua_State* L)
{
	lua_getmetatable(L, 1);
	
	while (lua_istable(L, -1))
	{
		lua_pushstring(L, "__setter");
		lua_rawget(L, -2);

		if (!lua_isnil(L, -1))
		{
			int top = lua_gettop(L) - 1, count;

			lua_pushvalue(L, 1);
			lua_pushvalue(L, 2);
			lua_pushvalue(L, 3);

			lua_call(L, 3, LUA_MULTRET);
			count = lua_gettop(L) - top;

			if (count <= 0)
			{
				return 0;
			}

			lua_pop(L, count);
		}
		else
		{
			lua_pop(L, 1);
		}
		
		lua_pushstring(L, "__base");
		lua_rawget(L, -2);
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------
void* MAZE::mzlGetObject(lua_State *L, int arg, const std::string& type, size_t align)
{
	void *p;

	lua_getmetatable(L, arg);
		
	while (lua_istable(L, -1))
	{
		lua_pushstring(L, "__type");
		lua_rawget(L, -2);

		if (!lua_isstring(L, -1))
		{
			lua_pushstring(L, ("invalid metatable: '" + type + "'").c_str());
			lua_error(L);
		}

		if (!strcmp(type.c_str(), lua_tostring(L, -1)))
		{
			lua_pop(L, 2);

			p = lua_touserdata(L, arg);
			p = (void*)(((size_t)p + align - 1) & ~(align - 1));

			return p;
		}

		lua_pop(L, 1);
		lua_pushstring(L, "__base");
		lua_rawget(L, -2);
		lua_remove(L, -2);
	}

	lua_pushstring(L, ("invalid object: '" + type + "'").c_str());
	lua_error(L);

	return NULL;
}

// ------------------------------------------------------------------------------------------------
void* MAZE::mzlCreateObject(lua_State *L, size_t sz, const std::string& type, size_t align)
{
	void* p;

	p = lua_newuserdata(L, sz + align - 1);
	p = (void*)(((size_t)p + align - 1) & ~(align - 1));

	lua_getglobal(L, (type + "__meta").c_str());		

	if (!lua_istable(L, -1))
	{
		lua_pop(L, 2);
		lua_pushstring(L, ("invalid type: '" + type + "'").c_str());
		lua_error(L);
	}

	lua_setmetatable(L, -2);
	return p;
}
