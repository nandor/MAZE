// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
#include "MZException.h"
#include "MZEngine.h"
#include "MZRsmngr.h"
#include "MZWorld.h"
#include "MZModel.h"
#include "MZLight.h"
#include "MZCamera.h"
#include "MZObject.h"
#include "MZScene.h"
#include "MZLuaMath.h"
#include "MZLuaScene.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
World::World(Engine* engine)
	: mEngine(engine),
	  mScene(NULL),
	  mScript(NULL),
	  mPlayer(NULL)
{
}

// ------------------------------------------------------------------------------------------------
World::~World()
{
	if (mScene != NULL)  { delete mScene;  mScene = NULL; }
	if (mScript != NULL) { lua_close(mScript); mScript = NULL; }
}

// ------------------------------------------------------------------------------------------------
void World::Init()
{
	InitScene();
	InitScript();
}

// ------------------------------------------------------------------------------------------------
void World::InitScene()
{
	mSkyTexture = mEngine->GetResourceManager()->Get<Texture> ("sky");
	
	mSize = glm::vec3(100.0f, 100.0f, 100.0f);
	mScene = new Scene(mEngine, mSize);
	
	mPlayer = mScene->Create<Player> ("player");
	mPlayer->SetPosition(glm::vec3(2.5f, 1.6f, 2.5f));

	mMoon = mScene->Create<Light>("moon");
	mMoon->SetType(Light::DIRECTIONAL);
	mMoon->SetDiffuse(glm::vec3(0.1f, 0.1f, 0.6f));
	mMoon->SetSpecular(glm::vec3(0.2f, 0.2f, 1.0f));
	mMoon->SetAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
	mMoon->SetDirection(glm::vec3(-1.0f, -1.0f, -1.0f));
	mMoon->SetShadowCaster(true);
	Model::CreatePlane(mEngine->GetResourceManager(), "floor", "floor_diffuse", "floor_bump", glm::vec2(100.0f), glm::vec2(1.5f));

	Object* floor = mScene->Create<Object>();
	floor->SetModel(mEngine->GetResourceManager()->Get<Model> ("floor"));
	floor->SetPosition(glm::vec3(50.0f, 0.0f, 50.0f));
	floor->SetCollider(true);
	floor->SetBoundingBox(BoundingBox(
		glm::vec3(-50.0f, 0.0f, -50.0f), 
		glm::vec3(100.0f, 0.0f, 100.0f)
	));
}

// ------------------------------------------------------------------------------------------------
void World::InitScript()
{
	if (!(mScript = luaL_newstate()))
	{
		throw Exception("[Script] Cannot create Lua VM");
	}

	luaL_openlibs(mScript);
	mzlRegisterMath(mScript);
	
	lua_pushcfunction(mScript, mzlDefaultPrint);
	lua_setglobal(mScript, "print");

	lua_pushlightuserdata(mScript, mScene);
	lua_setglobal(mScript, "__scene");
	lua_pushlightuserdata(mScript, mEngine->GetResourceManager());
	lua_setglobal(mScript, "__rsmngr");
	mzlRegisterScene(mScript);
	
	lua_getglobal(mScript, "package");
	lua_pushstring(mScript, (mEngine->GetSetup().ResourceDir + "/script/?.lua").c_str());
	lua_setfield(mScript, -2, "path");
	lua_pop(mScript, 1);

	if (luaL_dofile(mScript, "./data/script/maze.lua"))
	{
		throw Exception("[Script] ") << lua_tostring(mScript, -1);
	}
	
	// Call the lua init method
	lua_getglobal(mScript, "on_world_init");
	if (lua_isfunction(mScript, -1) && lua_pcall(mScript, 0, 0, 0))
	{
		throw Exception("[Script] ") << lua_tostring(mScript, -1);
	}
}

// ------------------------------------------------------------------------------------------------
void World::Update(float time, float dt)
{
	if (mEngine->IsKeyDown(Engine::KEY_ESC))
	{
		mEngine->Quit();
	}
	
	mScene->Update(time, dt);
		
	// Call the lua update method
	lua_getglobal(mScript, "on_world_update");
	lua_pushnumber(mScript, time);
	lua_pushnumber(mScript, dt);
	if (lua_isfunction(mScript, -1) && lua_pcall(mScript, 2, 0, 0))
	{
		throw Exception("[Script] ") << lua_tostring(mScript, -1);
	}
}

// ------------------------------------------------------------------------------------------------
void World::Render(RenderBuffer* buffer)
{
	// Global data
	buffer->WorldSize = mSize;
	buffer->Fog.Color = glm::vec3(0.4f, 0.4f, 0.7f);
	buffer->Fog.Density = 0.04f;
	buffer->Fog.Height = 7.0f;
	buffer->SkyTexture = mSkyTexture;
	buffer->ViewVolume = mPlayer->GetCamera()->GetVolume();
	
	// All visible objects
	mPlayer->GetCamera()->Render(buffer);
	mPlayer->Render(buffer, Entity::RENDER_GBUFFER);
	mScene->QueryRenderables(buffer->ViewVolume, buffer);
}
