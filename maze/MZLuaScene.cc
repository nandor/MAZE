// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
// entity
// ------------------------------------------------------------------------------------------------
static int entity__setter(lua_State *L)
{
	const char *field;
	Entity** object;

	object = (Entity**)mzlGetObject(L, 1, "entity");
	field = luaL_checkstring(L, 2);
	
	if (!strcmp(field, "collider"))
	{
		(*object)->SetCollider(lua_toboolean(L, 3) != 0);
		return 0;
	}
	else if (!strcmp(field, "pickable"))
	{
		(*object)->SetPickable(lua_toboolean(L, 3) != 0);
		return 0;
	}
	else if (!strcmp(field, "useable"))
	{
		(*object)->SetUseable(lua_toboolean(L, 3) != 0);
		return 0;
	}
	else if (!strcmp(field, "use_text"))
	{
		(*object)->SetUseText(luaL_checkstring(L, 3));
		return 0;
	}
	else if (!strcmp(field, "rendereable"))
	{
		(*object)->SetRenderable(lua_toboolean(L, 3) != 0);
		return 0;
	}
	else if (!strcmp(field, "shadow_caster"))
	{
		(*object)->SetShadowCaster(lua_toboolean(L, 3) != 0);
		return 0;
	}
	else if (!strcmp(field, "active"))
	{
		(*object)->SetActive(lua_toboolean(L, 3) != 0);
		return 0;
	}
	else if (!strcmp(field, "position"))
	{
		(*object)->SetPosition(*(glm::vec3*)mzlGetObject(L, 3, "vec3"));
		return 0;
	}
	else if (!strcmp(field, "rotation"))
	{
		(*object)->SetRotation(*(glm::vec3*)mzlGetObject(L, 3, "vec3"));
		return 0;
	}
	else if (!strcmp(field, "scale"))
	{
		(*object)->SetScale(*(glm::vec3*)mzlGetObject(L, 3, "vec3"));
		return 0;
	}
	else if (!strcmp(field, "box"))
	{
		(*object)->SetBoundingBox(*(BoundingBox*)mzlGetObject(L, 3, "box", 16));
		return 0;
	}
	
	lua_pushnil(L);
	return 1;
}

// ------------------------------------------------------------------------------------------------
static int entity__getter(lua_State *L)
{
	const Object** object = (const Object**)mzlGetObject(L, 1, "object");
	const char *field = luaL_checkstring(L, 2);
	
	if (!strcmp(field, "collider"))
	{
		lua_pushboolean(L, (*object)->IsCollider() != 0);
		return 1;
	}
	else if (!strcmp(field, "pickable"))
	{
		lua_pushboolean(L, (*object)->IsPickable() != 0);
		return 1;
	}
	else if (!strcmp(field, "useable"))
	{
		lua_pushboolean(L, (*object)->IsUseable() != 0);
		return 1;
	}
	else if (!strcmp(field, "use_text"))
	{
		lua_pushstring(L, (*object)->GetUseText().c_str());
		return 1;
	}
	else if (!strcmp(field, "renderable"))
	{
		lua_pushboolean(L, (*object)->IsRenderable() != 0);
		return 1;
	}
	else if (!strcmp(field, "active"))
	{
		lua_pushboolean(L, (*object)->IsActive() != 0);
		return 1;
	}
	else if (!strcmp(field, "type"))
	{
		lua_pushinteger(L, (*object)->GetType() != 0);
		return 1;
	}
	else if (!strcmp(field, "renderable"))
	{
		lua_pushboolean(L, (*object)->IsRenderable() != 0);
		return 1;
	}
	else if (!strcmp(field, "shadow_caster"))
	{
		lua_pushboolean(L, (*object)->IsShadowCaster() != 0);
		return 1;
	}
	else if (!strcmp(field, "name"))
	{
		lua_pushstring(L, (*object)->GetName().c_str());
		return 1;
	}
	else if (!strcmp(field, "position"))
	{
		void *pos;

		pos = mzlCreateObject(L, sizeof(glm::vec3), "vec3");
		new (pos) glm::vec3((*object)->GetPosition());

		return 1;
	}
	else if (!strcmp(field, "rotation"))
	{
		void *rot;

		rot = mzlCreateObject(L, sizeof(glm::vec3), "vec3");
		new (rot) glm::vec3((*object)->GetRotation());

		return 1;
	}
	else if (!strcmp(field, "scale"))
	{
		void *scale;

		scale = mzlCreateObject(L, sizeof(glm::vec3), "vec3");
		new (scale) glm::vec3((*object)->GetScale());

		return 1;
	}
	else if (!strcmp(field, "box"))
	{
		void *box;

		box = mzlCreateObject(L, sizeof(BoundingBox), "box", 16);
		new (box) BoundingBox((*object)->GetBoundingBox());

		return 1;
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------
static int entity__tostring(lua_State *L)
{	
	std::stringstream ss;
	Object** obj;

	obj = (Object**)mzlGetObject(L,  1, "player");

	ss << "entity('"  << (*obj)->GetName() << "')";
		
	lua_pushstring(L, ss.str().c_str());
	return 1;
}

// ------------------------------------------------------------------------------------------------
static int entity__delete(lua_State *L)
{
	(*(Object**)mzlGetObject(L,  1, "object"))->Delete();
	return 0;
}

// ------------------------------------------------------------------------------------------------
static const struct luaL_Reg entity_m[] =
{
	{"__setter", entity__setter},
	{"__getter", entity__getter},
	{"__tostring", entity__tostring},
	{"delete", entity__delete},
	{NULL, NULL}
};

// ------------------------------------------------------------------------------------------------
// object
// ------------------------------------------------------------------------------------------------
static int object__setter(lua_State *L)
{
	Object **object = (Object**)mzlGetObject(L,  1, "object");
	const char *field = luaL_checkstring(L, 2);

	if (!strcmp(field, "model"))
	{
		ResourceManager *rsmngr;

		lua_getglobal(L, "__rsmngr");
		if (!lua_isuserdata(L, -1) || ((rsmngr = (ResourceManager*)lua_touserdata(L, -1)) == NULL))
		{
			lua_pushstring(L, "'__rsmngr' not found");
			lua_error(L);
		}

		(*object)->SetModel(rsmngr->Get<Model>(luaL_checkstring(L, 3)));
		return 0;
	}
	else if (!strcmp(field, "on_pick"))
	{
		luaL_argcheck(L, lua_isfunction(L, 3), 3, "callback function expected");
		lua_pushvalue(L, 3);

		(*object)->SetPickCall(Callback(L, luaL_ref(L, LUA_REGISTRYINDEX)));
		return 0;
	}
	else if (!strcmp(field, "on_use"))
	{
		luaL_argcheck(L, lua_isfunction(L, 3), 3, "callback function expected");
		lua_pushvalue(L, 3);

		(*object)->SetUseCall(Callback(L, luaL_ref(L, LUA_REGISTRYINDEX)));
		return 0;
	}
	else if (!strcmp(field, "on_update"))
	{
		luaL_argcheck(L, lua_isfunction(L, 3), 3, "callback function expected");
		lua_pushvalue(L, 3);

		(*object)->SetUpdateCall(Callback(L, luaL_ref(L, LUA_REGISTRYINDEX)));
		return 0;
	}

	lua_pushnil(L);
	return 1;
}

// ------------------------------------------------------------------------------------------------
static int object__getter(lua_State *L)
{
	const Object **object = (const Object**)mzlGetObject(L,  1, "object");
	const char *field = luaL_checkstring(L, 2);
	
	if (!strcmp(field, "model"))
	{
		Model *model;

		if (!(model = (*object)->GetModel()))
		{
			lua_pushnil(L);
		}
		else
		{
			lua_pushstring(L, model->GetID().c_str());
		}
		
		return 1;
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------
static int object__tostring(lua_State *L)
{
	std::stringstream ss;

	Object **p = (Object**)mzlGetObject(L,  1, "object");

	ss << "object('" 
	   << (*p)->GetName() << "', vec3(" 
	   << (*p)->GetPosition().x << ", "
	   << (*p)->GetPosition().y << ", "
	   << (*p)->GetPosition().z << "))";
	
	lua_pushstring(L, ss.str().c_str());
	return 1;
}


// ------------------------------------------------------------------------------------------------
static int objectplay_sound(lua_State *L)
{
	Object **p = (Object**)mzlGetObject(L,  1, "object");
	(*p)->PlaySound(luaL_checkstring(L, 2));
	return 0;
}

// ------------------------------------------------------------------------------------------------
static const struct luaL_Reg object_m[] =
{
	{"__setter", object__setter},
	{"__getter", object__getter},
	{"__tostring", object__tostring},
	{"play_sound", objectplay_sound},
	{NULL, NULL}
};

// ------------------------------------------------------------------------------------------------
// light
// ------------------------------------------------------------------------------------------------
static int light__setter(lua_State *L)
{
	Light **light = (Light**)mzlGetObject(L,  1, "light");
	const char *field = luaL_checkstring(L, 2);

	if (!strcmp(field, "diffuse"))
	{
		(*light)->SetDiffuse(*(glm::vec3*)mzlGetObject(L, 3, "vec3"));
		return 0;
	}
	else if (!strcmp(field, "specular"))
	{
		(*light)->SetSpecular(*(glm::vec3*)mzlGetObject(L, 3, "vec3"));
		return 0;
	}
	else if (!strcmp(field, "ambient"))
	{
		(*light)->SetAmbient(*(glm::vec3*)mzlGetObject(L, 3, "vec3"));
		return 0;
	}
	else if (!strcmp(field, "type"))
	{
		const char *type;
		
		type = luaL_checkstring(L, 3);
		if (!strcmp(type, "directional"))
		{
			(*light)->SetType(Light::DIRECTIONAL);
			return 0;
		}
		else if (!strcmp(type, "point"))
		{
			(*light)->SetType(Light::POINT);
			return 0;
		}
		else if (!strcmp(type, "spot"))
		{
			(*light)->SetType(Light::SPOT);
			return 0;
		}

		lua_pushstring(L, "invalid light type");
		lua_error(L);
	}
	else if (!strcmp(field, "radius"))
	{
		(*light)->SetRadius((float)luaL_checknumber(L, 3));
		return 0;
	}
	else if (!strcmp(field, "direction"))
	{
		(*light)->SetDirection(*(glm::vec3*)mzlGetObject(L, 3, "vec3"));
		return 0;
	}
	else if (!strcmp(field, "angle"))
	{
		(*light)->SetAngle((float)luaL_checknumber(L, 3));
		return 0;
	}

	lua_pushnil(L);
	return 1;
}

// ------------------------------------------------------------------------------------------------
static int light__getter(lua_State *L)
{
	const Light **light = (const Light**)mzlGetObject(L,  1, "light");
	const char *field = luaL_checkstring(L, 2);

	if (!strcmp(field, "diffuse"))
	{
		new (mzlCreateObject(L, sizeof(glm::vec3), "vec3")) glm::vec3((*light)->GetDiffuse());
		return 1;
	}
	else if (!strcmp(field, "specular"))
	{
		new (mzlCreateObject(L, sizeof(glm::vec3), "vec3")) glm::vec3((*light)->GetSpecular());
		return 1;
	}
	else if (!strcmp(field, "ambient"))
	{
		new (mzlCreateObject(L, sizeof(glm::vec3), "vec3")) glm::vec3((*light)->GetAmbient());
		return 1;
	}
	else if (!strcmp(field, "type"))
	{
		switch ((*light)->GetType())
		{
			case Light::DIRECTIONAL: lua_pushstring(L, "directional"); break;
			case Light::POINT: lua_pushstring(L, "point"); break;
			case Light::SPOT: lua_pushstring(L, "spot"); break;
		} 
		
		return 1;
	}
	else if (!strcmp(field, "radius"))
	{
		lua_pushnumber(L, (*light)->GetRadius());
		return 1;
	}
	else if (!strcmp(field, "direction"))
	{
		new (mzlCreateObject(L, sizeof(glm::vec3), "vec3")) glm::vec3((*light)->GetDirection());
		return 1;
	}
	else if (!strcmp(field, "angle"))
	{
		lua_pushnumber(L, (*light)->GetAngle());
		return 1;
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------
static int light__tostring(lua_State *L)
{
	std::stringstream ss;
	std::string name;

	Object **p = (Object**)mzlGetObject(L,  1, "object");

	ss << "light('" 
	   << (*p)->GetName() << "', "
	   << (*p)->GetType() << ", vec3(" 
	   << (*p)->GetPosition().x << ", "
	   << (*p)->GetPosition().y << ", "
	   << (*p)->GetPosition().z << "))";
	
	lua_pushstring(L, ss.str().c_str());
	return 1;
}

// ------------------------------------------------------------------------------------------------
static const struct luaL_Reg light_m[] =
{
	{"__setter", light__setter},
	{"__getter", light__getter},
	{"__tostring", light__tostring},
	{NULL, NULL}
};
// ------------------------------------------------------------------------------------------------
// player
// ------------------------------------------------------------------------------------------------
static int player__setter(lua_State *L)
{
	Player **p = (Player**)mzlGetObject(L,  1, "player");
	const char *name = luaL_checkstring(L, 2);

	if (!strcmp(name, "move_speed"))
	{
		(*p)->SetMoveSpeed((float)luaL_checknumber(L, 3));
		return 0;
	}

	lua_pushnil(L);
	return 1;
}

// ------------------------------------------------------------------------------------------------
static int player__getter(lua_State *L)
{
	const Player **p = (const Player**)mzlGetObject(L,  1, "player");
	const char *name = luaL_checkstring(L, 2);
	
	if (!strcmp(name, "move_speed"))
	{
		lua_pushnumber(L, (*p)->GetMoveSpeed());
		return 1;
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------
static int player__tostring(lua_State *L)
{
	std::stringstream ss;
	std::string name;

	Player **p = (Player**)mzlGetObject(L,  1, "object");

	ss << "player(vec3(" 
	   << (*p)->GetPosition().x << ", "
	   << (*p)->GetPosition().y << ", "
	   << (*p)->GetPosition().z << "))";
	
	lua_pushstring(L, ss.str().c_str());
	return 1;
}

// ------------------------------------------------------------------------------------------------
static const struct luaL_Reg player_m[] =
{
	{"__setter", player__setter},
	{"__getter", player__getter},
	{"__tostring", player__tostring},
	{NULL, NULL}
};

// ------------------------------------------------------------------------------------------------
// scene
// ------------------------------------------------------------------------------------------------
static int scene__create(lua_State *L)
{
	const char *type = luaL_checkstring(L, 1);
	const char *name = luaL_optstring(L, 2, "");
	Scene *scene;

	lua_getglobal(L, "__scene");
	if (!lua_isuserdata(L, -1) || ((scene = (Scene*)lua_touserdata(L, -1)) == NULL))
	{
		lua_pushstring(L, "'__scene' not found");
		lua_error(L);
	}

	if (!strcmp(type, "object"))
	{
		*(Object**)mzlCreateObject(L, sizeof(Object**), "object") = scene->Create<Object> (name);
		return 1;
	}
	else if (!strcmp(type, "light"))
	{
		*(Light**)mzlCreateObject(L, sizeof(Light**), "light") = scene->Create<Light> (name);
		return 1;
	}
	
	lua_pushstring(L, "scene.create: invalid type");
	lua_error(L);
	return 0;
}

// ------------------------------------------------------------------------------------------------
static int scene__get(lua_State *L)
{
	const char *type = luaL_checkstring(L, 1);
	const char *name = luaL_checkstring(L, 2);
	Scene *scene;

	lua_getglobal(L, "__scene");
	if (!lua_isuserdata(L, -1) || ((scene = (Scene*)lua_touserdata(L, -1)) == NULL))
	{
		lua_pushstring(L, "'__scene' not found");
		lua_error(L);
	}

	if (!strcmp(type, "object"))
	{
		*(Object**)mzlCreateObject(L, sizeof(Object**), "object") = scene->Get<Object> (name);
		return 1;
	}
	else if (!strcmp(type, "player"))
	{
		*(Player**)mzlCreateObject(L, sizeof(Player**), "player") = scene->Get<Player> (name);
		return 1;
	}
	else if (!strcmp(type, "light"))
	{
		*(Light**)mzlCreateObject(L, sizeof(Light**), "light") = scene->Get<Light> (name);
		return 1;
	}

	lua_pushstring(L, "scene.get: invalid type");
	lua_error(L);
	return 0;
}

// ------------------------------------------------------------------------------------------------
static const struct luaL_Reg scene_s[] =
{
	{"create", scene__create},
	{"get", scene__get},
	{NULL, NULL}
};

// ------------------------------------------------------------------------------------------------
void MAZE::mzlRegisterScene(lua_State* L)
{
	mzlClass(L, "entity", "",		NULL, NULL,    entity_m);
	mzlClass(L, "light",  "entity", NULL, NULL,    light_m );
	mzlClass(L, "object", "entity", NULL, NULL,    object_m);
	mzlClass(L, "player", "entity", NULL, NULL,	   player_m);
	mzlClass(L, "scene",  "",		NULL, scene_s, NULL    );
}
