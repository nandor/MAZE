// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZCALLBACK_H
#define MZCALLBACK_H
#pragma once

#include "MZLua.h"

namespace MAZE
{
    class Entity;
    
    class Callback
    {
    public:
    
        Callback();
        Callback(lua_State *L, int ref);
		Callback(const Callback& call);

        virtual ~Callback();

        void Call(Entity *who);
        void Call(float time, float dt);
        
		void operator = (const Callback& call);
    
    private:
    
        /// Lua VM
        lua_State *mScript;
        
        /// Callback function reference
        int mFunc;
    
    };
};

#endif
