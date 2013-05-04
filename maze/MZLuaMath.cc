// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <sstream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "MZBound.h"
#include "MZLuaMath.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
// vec3
// ------------------------------------------------------------------------------------------------
static int vec3__ctor(lua_State* L)
{
	glm::vec3 *v;
	float x, y, z;

	x = (float)luaL_checknumber(L, 2);
	y = (float)luaL_checknumber(L, 3);
	z = (float)luaL_checknumber(L, 4);

	v = (glm::vec3*)mzlCreateObject(L, sizeof(glm::vec3), "vec3");
	v->x = x;
	v->y = y;
	v->z = z;

	return 1;
}
 
// ------------------------------------------------------------------------------------------------
static int vec3length(lua_State *L)
{
	glm::vec3 *v;

	v = (glm::vec3*)mzlGetObject(L, 1, "vec3");

	luaL_argcheck(L, v != NULL, 1, "");
	lua_pushnumber(L, glm::length(*v));

	return 1;
}

// ------------------------------------------------------------------------------------------------
static int vec3__add(lua_State *L)
{
	glm::vec3 *a, *b, *v;

	a = (glm::vec3*)mzlGetObject (L, 1, "vec3");
	b = (glm::vec3*)mzlGetObject (L, 2, "vec3");
	v = (glm::vec3*)mzlCreateObject(L, sizeof(glm::vec3), "vec3");
	
	v->x = a->x + b->x;
	v->y = a->y + b->y;
	v->z = a->z + b->z;

	return 1;
}

// ------------------------------------------------------------------------------------------------
static int vec3__sub(lua_State *L)
{
	glm::vec3 *a, *b, *v;
	
	a = (glm::vec3*)mzlGetObject (L, 1, "vec3");
	b = (glm::vec3*)mzlGetObject (L, 2, "vec3");
	v = (glm::vec3*)mzlCreateObject(L, sizeof(glm::vec3), "vec3");
	
	v->x = a->x - b->x;
	v->y = a->y - b->y;
	v->z = a->z - b->z;

	return 1;
}

// ------------------------------------------------------------------------------------------------
static int vec3__tostring(lua_State *L)
{
	glm::vec3 *a;
	std::stringstream ss;
	
	a = (glm::vec3*)mzlGetObject (L, 1, "vec3");

	ss << "vec3(" << a->x << ", " << a->y << ", " << a->z << ")";

	lua_pushstring(L, ss.str().c_str());
	return 1;
}

// ------------------------------------------------------------------------------------------------
static int vec3__getter(lua_State *L)
{
	const char* name = luaL_checkstring(L, 2);
	glm::vec3* obj = (glm::vec3*)mzlGetObject(L,  1, "vec3");

	if (!strcmp(name, "x"))
	{
		lua_pushnumber(L, obj->x);
		return 1;
	}
	else if (!strcmp(name, "y"))
	{
		lua_pushnumber(L, obj->y);
		return 1;
	}
	else if (!strcmp(name, "z"))
	{
		lua_pushnumber(L, obj->z);
		return 1;
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------
static int vec3__setter(lua_State *L)
{
	const char* name = luaL_checkstring(L, 2);
	glm::vec3* obj = (glm::vec3*)mzlGetObject(L,  1, "vec3");

	if (!strcmp(name, "x"))
	{
		obj->x = (float)luaL_checknumber(L, 3);
		return 0;
	}
	else if (!strcmp(name, "y"))
	{
		obj->y = (float)luaL_checknumber(L, 3);
		return 0;
	}
	else if (!strcmp(name, "z"))
	{
		obj->z = (float)luaL_checknumber(L, 3);
		return 0;
	}

	lua_pushnil(L);
	return 0;
}

// ------------------------------------------------------------------------------------------------
static int vec3cross(lua_State *L)
{
	glm::vec3 *a, *b, *v;
	
	a = (glm::vec3*)mzlGetObject (L, 1, "vec3");
	b = (glm::vec3*)mzlGetObject (L, 2, "vec3");
	v = (glm::vec3*)mzlCreateObject(L, sizeof(glm::vec3), "vec3");
	
	*v = glm::cross(*a, *b);

	return 1;
}

// ------------------------------------------------------------------------------------------------
static int vec3dot(lua_State *L)
{
	glm::vec3 *a, *b;
	
	a = (glm::vec3*)mzlGetObject (L, 1, "vec3");
	b = (glm::vec3*)mzlGetObject (L, 2, "vec3");
	
	lua_pushnumber(L, glm::dot(*a, *b));
	return 1;
}

// ------------------------------------------------------------------------------------------------
static const struct luaL_Reg vec3_m[] = 
{
	{"length",		vec3length},
	{"__add",		vec3__add},
	{"__sub",		vec3__sub},
	{"__tostring",	vec3__tostring},
	{"__setter",	vec3__setter},
	{"__getter",	vec3__getter},
	{NULL, NULL}
};

static const struct luaL_Reg vec3_s[] =
{
	{"cross",		vec3cross},
	{"dot",			vec3dot},
	{NULL, NULL}
};

// ------------------------------------------------------------------------------------------------
// box
// ------------------------------------------------------------------------------------------------
static int box_ctor(lua_State* L)
{
	glm::vec3 *a, *b;
	BoundingBox *box;
	
	a = (glm::vec3*)mzlGetObject (L, 2, "vec3");
	b = (glm::vec3*)mzlGetObject (L, 3, "vec3");

	box = (BoundingBox*)mzlCreateObject(L, sizeof(BoundingBox), "vec3");
	new (box) BoundingBox(*a, *b);

	return 1;
}

// ------------------------------------------------------------------------------------------------
static const struct luaL_Reg box_m[] = 
{
	{NULL, NULL}
};

static const struct luaL_Reg box_s[] =
{
	{NULL, NULL}
};

// ------------------------------------------------------------------------------------------------
void MAZE::mzlRegisterMath(lua_State* L)
{
	mzlClass(L, "vec3", "", vec3__ctor, vec3_s, vec3_m);
	mzlClass(L, "box", "", box_ctor, box_s, box_m);
}
