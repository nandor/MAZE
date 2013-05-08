// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZObject.h"
#include "MZPlayer.h"
#include "MZScene.h"
#include "MZRsmngr.h"
#include "MZLuaScene.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
// object
// ------------------------------------------------------------------------------------------------
static int object__setter(lua_State *L)
{
	Object **obj = (Object**)mzlGetObject(L,  1, "object");
	const char *name = luaL_checkstring(L, 2);

	if (!strcmp(name, "model"))
	{
		ResourceManager *rsmngr;

		lua_getglobal(L, "__rsmngr");
		if (!lua_isuserdata(L, -1) || ((rsmngr = (ResourceManager*)lua_touserdata(L, -1)) == NULL))
		{
			lua_pushstring(L, "'__rsmngr' not found");
			lua_error(L);
		}

		(*obj)->SetModel(rsmngr->Get<Model>(luaL_checkstring(L, 3)));
		return 0;
	}
	else if (!strcmp(name, "position"))
	{
		glm::vec3 *v;

		v = (glm::vec3*)mzlGetObject(L, 3, "vec3");

		(*obj)->SetPosition(*v);
		return 0;
	}
	else if (!strcmp(name, "rotation"))
	{
		glm::vec3 *v;

		v = (glm::vec3*)mzlGetObject(L, 3, "vec3");

		(*obj)->SetRotation(v->x, v->y, v->z);
		return 0;
	}
	else if (!strcmp(name, "box"))
	{
		BoundingBox *box;

		box = (BoundingBox*)mzlGetObject(L, 3, "box");

		(*obj)->SetBoundingBox(*box);
		return 0;
	}

	lua_pushnil(L);
	return 1;
}

// ------------------------------------------------------------------------------------------------
static int object__getter(lua_State *L)
{
	Object **obj = (Object**)mzlGetObject(L,  1, "object");
	const char *name = luaL_checkstring(L, 2);

	if (!strcmp(name, "box"))
	{
		void *box;

		box = mzlCreateObject(L, sizeof(BoundingBox), "box");
		new (box) BoundingBox((*obj)->GetBoundingBox());

		return 1;
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------
static int object__tostring(lua_State *L)
{
	return 0;
}

// ------------------------------------------------------------------------------------------------
// player
// ------------------------------------------------------------------------------------------------
static int player__getter(lua_State *L)
{
	Player **p = (Player**)mzlGetObject(L,  1, "player");
	const char *name = luaL_checkstring(L, 2);

	if (!strcmp(name, "box"))
	{
		void *box;

		box = mzlCreateObject(L, sizeof(BoundingBox), "box");
		new (box) BoundingBox((*p)->GetBoundingBox());

		return 1;
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------
static const struct luaL_Reg player_m[] =
{
	{"__getter", player__getter},
	{NULL, NULL}
};

// ------------------------------------------------------------------------------------------------
static const struct luaL_Reg object_m[] =
{
	{"__setter", object__setter},
	{"__getter", object__getter},
	{"__tostring", object__tostring},
	{NULL, NULL}
};

// ------------------------------------------------------------------------------------------------
// scene
// ------------------------------------------------------------------------------------------------
static int scenecreate(lua_State *L)
{
	Scene *scene;
	const char *type;

	lua_getglobal(L, "__scene");
	if (!lua_isuserdata(L, -1) || ((scene = (Scene*)lua_touserdata(L, -1)) == NULL))
	{
		lua_pushstring(L, "'__scene' not found");
		lua_error(L);
	}

	type = luaL_checkstring(L, 1);
	if (!strcmp(type, "object"))
	{
		Object **obj;

		obj = (Object**)mzlCreateObject(L, sizeof(Object**), "object");
		*obj = scene->Create<Object> ();

		return 1;
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------
static int sceneget(lua_State *L)
{
	lua_pushstring(L, "not implemented: scene.get");
	lua_error(L);
	return 0;
}

// ------------------------------------------------------------------------------------------------
static int scenedelete(lua_State *L)
{
	Scene *scene;

	lua_getglobal(L, "__scene");
	if (!lua_isuserdata(L, -1) || ((scene = (Scene*)lua_touserdata(L, -1)) == NULL))
	{
		lua_pushstring(L, "'__scene' not found");
		lua_error(L);
	}

	Object **obj = (Object**)mzlGetObject(L,  1, "object");
	scene->DestroyEntity(*obj);

	return 0;
}

// ------------------------------------------------------------------------------------------------
static const struct luaL_Reg scene_s[] =
{
	{"create",	scenecreate},
	{"get",		sceneget},
	{"delete",	scenedelete},
	{NULL, NULL}
};

// ------------------------------------------------------------------------------------------------
void MAZE::mzlRegisterScene(lua_State* L)
{
	mzlClass(L, "object", "", NULL, NULL, object_m);
	mzlClass(L, "player", "", NULL, NULL, player_m);
	mzlClass(L, "scene", "", NULL, scene_s, NULL);
}
