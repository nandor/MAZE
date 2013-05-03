// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2013 The MAZE project. All rights reserved.

#ifndef SCRIPT_H
#define SCRIPT_H
#pragma once

#include <string>
#include <lua.hpp>

namespace MAZE
{
    class Scene;
    class Engine;
    class Log;
    
    class Script
    {
    public:

		/**
			Creates a new lua virtual machine
		*/
		Script(Engine *engine);

		/**
			Closes the state and destroys the vm
		*/
		virtual ~Script();
        
		/**
			Loads & executes a script file
		*/
        void Include(const std::string& script);

		/**
			Executes a handler function
		*/
        void Execute(const std::string& func);

		/**
			Returns the script folder
		*/
		std::string GetScriptDir() const
		{
			return mScriptDir;
		}
        
	private:

        void RegisterEngine(Engine *engine);
        void RegisterLogger(Log *log);
        void RegisterScene(Scene *scene);
        
    private:
    
		/// Reference to the game engine
		Engine *mEngine;

		/// Lua vm
        lua_State *mState;

		/// Folder where scripts are stored
		std::string mScriptDir;
    };
};

#endif
