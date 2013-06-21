// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZLUA_H
#define MZLUA_H
#pragma once

namespace MAZE
{
	/**
		Registers a new class
		@param L	Lua VM
		@param name Name of the class
		@param ctor Constructor
		@param s	Static table
		@param m	Metatable
	*/
	void mzlClass(lua_State *L, const std::string& name, const std::string& base, 
				  lua_CFunction ctor, const luaL_Reg *s, const luaL_Reg *m);
	
	/**
		Default print method
	*/
	int mzlDefaultPrint(lua_State *L);

	/**
		Default getter method for classes
	*/
	int mzlDefaultGetter(lua_State *L);

	/**
		Default setter method
	*/
	int mzlDefaultSetter(lua_State* L);
	
	/**
		Retrieves an object from the stack or throws
		and error if the given object is invalid
	*/
	void* mzlGetObject(lua_State *L, int arg, const std::string& type, size_t align = 1);

	/**
		Creates a new object placed in Lua userdata
		Uses placement new to initialize our object
		in memory managed by Lua
	*/
	void* mzlCreateObject(lua_State *L, size_t sz, const std::string& type, size_t align = 1);
	
	/**
		Registers math libraries
	*/
	void mzlRegisterScene(lua_State* L);
	
	/**
		Registers math libraries
	*/
	void mzlRegisterMath(lua_State* L);
}

#endif
