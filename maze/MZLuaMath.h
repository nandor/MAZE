// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZLUAMATH_H
#define MZLUAMATH_H
#pragma once

#include "MZLua.h"

namespace MAZE
{
	/**
		Registers math libraries
	*/
	void mzlRegisterMath(lua_State* L);
};

#endif
