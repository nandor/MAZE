// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
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
	glm::vec3* obj = (glm::vec3*)mzlGetObject(L,  1, "vec3");
	const char* name = luaL_checkstring(L, 2);

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
	glm::vec3* obj = (glm::vec3*)mzlGetObject(L,  1, "vec3");
	const char* name = luaL_checkstring(L, 2);

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
	return 1;
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
	{"__add",		vec3__add},
	{"__sub",		vec3__sub},
	{"__tostring",	vec3__tostring},
	{"__setter",	vec3__setter},
	{"__getter",	vec3__getter},
	{"length",		vec3length},
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
static int box__ctor(lua_State* L)
{
	glm::vec3 *a, *b;
	BoundingBox *box;
	
	a = (glm::vec3*)mzlGetObject(L, 2, "vec3");
	b = (glm::vec3*)mzlGetObject(L, 3, "vec3");

	box = (BoundingBox*)mzlCreateObject(L, sizeof(BoundingBox), "box", 16);
	new (box) BoundingBox(*a, *b);

	return 1;
}

// ------------------------------------------------------------------------------------------------
static int boxintersect(lua_State *L)
{
	BoundingBox *a = (BoundingBox*)mzlGetObject(L,  1, "box", 16);
	BoundingBox *b = (BoundingBox*)mzlGetObject(L,  2, "box", 16);

	lua_pushnumber(L, !a->Outside(*b));
	return 1;
}

// ------------------------------------------------------------------------------------------------
static int box__tostring(lua_State *L)
{
	BoundingBox *a;
	std::stringstream ss;
	
	a = (BoundingBox*)mzlGetObject (L, 1, "box", 16);
	__m128 min = a->GetMin(), max = a->GetMax();

	ss << "box" 
			<< "(" 
				<< "(" 
					<< (((float*)&min)[0]) << ", " 
					<< (((float*)&min)[1]) << ", " 
					<< (((float*)&min)[2]) 
				<< ")," 
			<< "("
				<< (((float*)&max)[0]) << ", " 
				<< (((float*)&max)[1]) << ", " 
				<< (((float*)&max)[2]) 
			<< ")" 
		<< ")";
				

	lua_pushstring(L, ss.str().c_str());
	return 1;
}

// ------------------------------------------------------------------------------------------------
static const struct luaL_Reg box_m[] = 
{
	{"__tostring",	box__tostring},
	{"intersect",	boxintersect},
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
	mzlClass(L, "box", "", box__ctor, box_s, box_m);
}
