// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
#include "MZException.h"
#include "MZEngine.h"
#include "MZMath.h"
#include "MZRsmngr.h"
#include "MZWorld.h"
#include "MZModel.h"
#include "MZLight.h"
#include "MZCamera.h"
#include "MZObject.h"
#include "MZScript.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
World::World(Engine* engine)
	: mEngine(engine),
	  mScene(NULL),
	  mPlayer(NULL),
	  mScript(new Script(engine))
{
}

// ------------------------------------------------------------------------------------------------
World::~World()
{
	if (mScript != NULL) { delete mScript; mScript = NULL; }
	if (mScene != NULL)  { delete mScene;  mScene = NULL; }
}

// ------------------------------------------------------------------------------------------------
void World::Init()
{
	mSkyTexture = mEngine->GetResourceManager()->Get<Texture> ("sky");
	
	mSize = glm::vec3(100.0f, 100.0f, 100.0f);
	mScene = new Scene(mEngine, mSize);

	mScript->Include("core.lua");
	mScript->Include("maze.lua");
	mScript->Execute("on_world_init");

	mPlayer = mScene->Create<Player> ();
	mPlayer->SetPosition(glm::vec3(10.0f, 3.0f, 10.0f));

	mMoon = mScene->Create<Light>();
	mMoon->SetType(Light::DIRECTIONAL);
	mMoon->SetDiffuse(glm::vec3(0.1f, 0.1f, 0.6f));
	mMoon->SetSpecular(glm::vec3(0.2f, 0.2f, 1.0f));
	mMoon->SetAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
	mMoon->SetDirection(glm::vec3(-1.0f, -1.0f, -1.0f));
	mMoon->SetShadowCaster(true);

	mSun = mScene->Create<Light>();
	mSun->SetType(Light::DIRECTIONAL);
	mSun->SetDiffuse(glm::vec3(1.0f, 1.0f, 0.5f));
	mSun->SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
	mSun->SetAmbient(glm::vec3(0.1f, 0.1f, 0.1f));
	mSun->SetDirection(glm::vec3(1.0f, -1.0f, -1.0f));
	mSun->SetShadowCaster(true);
	mSun->SetActive(false);

	for (size_t i = 0; i < 50; ++i)
	{
		Light* obj = mScene->Create<Light>();
		obj->SetType(Light::POINT);
		obj->SetPosition(glm::vec3(rand() % 100, 1.0f, rand() % 100));
		obj->SetDiffuse(glm::vec3(rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f));
		obj->SetRadius(rand() % 100 / 10.0f);
		obj->SetShadowCaster(false);
	}

	for (size_t i = 0; i < 500; ++i)
	{
		Object* obj = mScene->Create<Object>();
		obj->SetModel(mEngine->GetResourceManager()->Get<Model> ("pillar"));
		obj->SetPosition(glm::vec3(rand() % 100, 0.0f, rand() % 100));
		obj->SetBoundingBox(BoundingBox(glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(2.0f, 4.0, 2.0f)));
	}

	Model::CreatePlane(mEngine->GetResourceManager(), "floor", "floor_diffuse", "floor_bump", glm::vec2(100.0f), glm::vec2(1.5f));

	Object* floor = mScene->Create<Object>();
	floor->SetModel(mEngine->GetResourceManager()->Get<Model> ("floor"));
	floor->SetPosition(glm::vec3(50.0f, 0.0f, 50.0f));
	floor->SetBoundingBox(BoundingBox(
		glm::vec3(-50.0f, 0.0f, -50.0f), 
		glm::vec3(100.0f, 0.0f, 100.0f)
	));
}

// ------------------------------------------------------------------------------------------------
void World::Generate()
{
}

// ------------------------------------------------------------------------------------------------
void World::Update(float time, float dt)
{
	if (mEngine->IsKeyDown(Engine::KEY_ESC))
	{
		mEngine->Quit();
	}
	
	mPlayer->Update(time, dt);
	
	float phi = 0.3f;
	float sunTheta = time / 20000.0f;
	float moonTheta = sunTheta + PI;
	/*
	glm::vec3 sunDir, moonDir;
	
	sunDir.x = -cos(sunTheta) * cos(phi);
	sunDir.y = -sin(sunTheta);
	sunDir.z = -cos(sunTheta) * sin(phi);
	
	moonDir.x = -cos(moonTheta) * cos(phi);
	moonDir.y = -sin(moonTheta);
	moonDir.z = -cos(moonTheta) * sin(phi);

	mSun->SetDirection(sunDir);
	mSun->SetActive(sunDir.y < 0.0f);

	mMoon->SetDirection(moonDir);
	mMoon->SetActive(moonDir.y < 0.0f);*/
}


// ------------------------------------------------------------------------------------------------
void World::Render(RenderBuffer* buffer)
{
	// Global data
	buffer->WorldSize = mSize;
	buffer->SkyTexture = mSkyTexture;
	buffer->ViewVolume = mPlayer->GetCamera()->GetVolume();
	
	// Prepare the player
	mPlayer->Render(buffer, Entity::RENDER_GBUFFER);
	mPlayer->GetCamera()->Render(buffer);

	// All visible objects
	mScene->QueryScene(buffer->ViewVolume, buffer);
}
