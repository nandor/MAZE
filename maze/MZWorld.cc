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
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
World::World(Engine* engine)
	: mEngine(engine),
	  mCamera(new Camera(engine)),
	  mRotX(0.0f),
	  mRotY(0.0f),
	  mVelocity(0.0f, 0.0f, 0.0f),
	  mScene(new Scene(100.0f, 100.0f, 100.0f))
{
}

// ------------------------------------------------------------------------------------------------
World::~World()
{
}

// ------------------------------------------------------------------------------------------------
void World::Init()
{
	mSize = glm::vec3(100.0f, 100.0f, 100.0f);
	mAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	mCamera->SetPosition(glm::vec3(7.0f, 2.0f, 7.0f));
	mCamera->SetTarget(glm::vec3(0.0, 0.0, 2.0));

	mSkyTexture = mEngine->GetResourceManager()->Get<Texture> ("sky");
	mGroundTexture = mEngine->GetResourceManager()->Get<Texture> ("grass");
	
	for (size_t i = 0; i < 10; ++i)
	{		
		Light* light = mScene->CreateLight(Light::POINT);
		light->SetPosition(glm::vec3(rand() % 100, 1.0f, rand() % 100));
		light->SetDiffuse(glm::vec3(rand() % 10 / 10.0f, rand() % 10 / 10.0f, rand() % 10 / 10.0f));
		light->SetRadius(10.0f + rand() % 100 / 10.0f);
		light->SetSpecular(glm::vec3(2.0f, 2.0f, 2.0f));
	}
	
	for (size_t i = 0; i < 100; ++i)
	{
		Object* obj = mScene->CreateObject();
		obj->SetModel(mEngine->GetResourceManager()->Get<Model> ("hedge"));
		obj->SetPosition(glm::vec3(rand() % 100, 0.0f, rand() % 100));
		obj->SetRotation((float)(rand() % 100), 1.0f, (float)(rand() % 100));
		obj->SetShadowCaster(true);
		obj->SetBoundingBox(BoundingBox(glm::vec3(-1.0f, 0.0f, -0.5f), glm::vec3(2.0f, 1.7f, 1.0f)));
	}

	Light* l2 = mScene->CreateLight(Light::DIRECTIONAL);
	l2->SetDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
	l2->SetSpecular(glm::vec3(2.0f, 2.0f, 2.0f));
	l2->SetDirection(glm::vec3(-1.0f, -1.0f, -1.0f));

	Model::CreatePlane(mEngine->GetResourceManager(), "base_plane", 100.0f, 100.0f, 1.0f, 1.0f);
	Object *obj = mScene->CreateObject();
	obj->SetModel(mEngine->GetResourceManager()->Get<Model> ("base_plane"));
	obj->SetPosition(glm::vec3(50.0f, 0.0f, 50.0f));
	obj->SetBoundingBox(BoundingBox(
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
	mTime = time;
	mTimeDiff = dt;

	if (mEngine->IsKeyDown(Engine::KEY_ESC))
	{
		mEngine->Quit();
	}
	
	mScene->Update();
	UpdateCamera();
}

// ------------------------------------------------------------------------------------------------
void World::Render(RenderBuffer* buffer)
{
	ViewFrustum& view = mCamera->GetVolume();

	buffer->GroundTexture = mGroundTexture;
	buffer->SkyTexture = mSkyTexture;
	buffer->WorldSize = mSize;
	
	mCamera->Prepare(buffer);	
	mScene->QueryScene(view, buffer);
	
	for (size_t i = 0; i < buffer->Lights.size(); ++i)
	{
		LightRenderData& light = buffer->Lights[i];
		if (light.Type != Light::DIRECTIONAL)
		{
			continue;
		}

		float n = view.GetNearPlane(), f = view.GetFarPlane();
		float p[] = {0.0f, 0.06f, 0.15f, 0.4f, 1.0f};  
		for (size_t j = 0; j < 4; ++j)
		{
			ViewFrustum lightvol;

			float begin = n + (f - n) * p[j];
			float end = n + (f - n) * p[j + 1];
			
			lightvol = view.Slice(begin, end).GetLightVolume(glm::vec3(light.Direction));
			light.Shadow[j].MVP = lightvol.GetProjection() * lightvol.GetView();
			light.Shadow[j].NearZ = begin;	

			light.Shadow[j].Index = buffer->ShadowCasters.size();			
			mScene->QueryShadows(lightvol, buffer);
			light.Shadow[j].Count = buffer->ShadowCasters.size() - light.Shadow[j].Index;
		}
	}
}

// ------------------------------------------------------------------------------------------------
void World::UpdateCamera()
{
	int width = mEngine->GetConfig().WindowWidth;
	int height = mEngine->GetConfig().WindowHeight;
	glm::ivec2& pos = mEngine->GetMousePos();
	glm::vec3 dir(0.0f, 0.0f, 0.0f);
	
	mRotY += (width / 2 - pos.x) / 100.0f;
	mRotX += (height / 2 - pos.y) / 100.0f;

	mRotX = std::min(std::max(mRotX, -PIOVER2 + 0.1f), PIOVER2 - 0.1f);

	mCamera->SetDirection(glm::vec3(
		sin(mRotY) * cos(mRotX),
		sin(mRotX),
		cos(mRotY) * cos(mRotX)
	));

	if (mEngine->IsKeyDown(Engine::KEY_W))
	{
		dir.x += sin(mRotY) * cos(mRotX);
		dir.z += cos(mRotY) * cos(mRotX);
	}

	if (mEngine->IsKeyDown(Engine::KEY_S))
	{
		dir.x -= sin(mRotY) * cos(mRotX);
		dir.z -= cos(mRotY) * cos(mRotX);
	}

	if (mEngine->IsKeyDown(Engine::KEY_A))
	{
		dir.x += sin(mRotY + PIOVER2) * cos(mRotX);
		dir.z += cos(mRotY + PIOVER2) * cos(mRotX);
	}

	if (mEngine->IsKeyDown(Engine::KEY_D))
	{
		dir.x += sin(mRotY - PIOVER2) * cos(mRotX);
		dir.z += cos(mRotY - PIOVER2) * cos(mRotX);
	}

	if (glm::length(dir) > 0.0f)
	{
		dir = glm::normalize(dir) * 0.0075f * mTimeDiff;
	}

	mVelocity.x = dir.x;
	mVelocity.z = dir.z;
	
	if (mEngine->IsKeyDown(Engine::KEY_SPACE) && mVelocity.y == 0.0f)
	{
		mVelocity.y += 0.03f;
	}

	mVelocity.y -= 0.0001f * mTimeDiff;

	BoundingBox player;
	
	player.SetSize(1.0f, 3.0f, 1.0f);
	player.SetPosition(mCamera->GetPosition() - glm::vec3(0.5f, 1.5f, 0.5f));
		
	glm::vec3 vel = mScene->QueryDistance(player, mVelocity);
	mCamera->SetPosition(mCamera->GetPosition() + vel);

	mVelocity.x = (abs(vel.x) < abs(mVelocity.x)) ? 0.0f : mVelocity.x;
	mVelocity.y = (abs(vel.y) < abs(mVelocity.y)) ? 0.0f : mVelocity.y;
	mVelocity.z = (abs(vel.z) < abs(mVelocity.z)) ? 0.0f : mVelocity.z;

	::SetCursorPos(width >> 1, height >> 1);
}
