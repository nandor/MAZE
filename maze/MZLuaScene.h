// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZLUASCENE_H
#define MZLUASCENE_H
#pragma once

#include "MZLua.h"

namespace MAZE
{
	/**
		Registers math libraries
	*/
	void mzlRegisterScene(lua_State* L);
};

#endif
