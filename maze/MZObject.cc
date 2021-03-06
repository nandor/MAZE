// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
Object::Object(Engine *engine)
	: Entity(engine, Entity::OBJECT)
{
}

// ------------------------------------------------------------------------------------------------
Object::~Object()
{
}

// ------------------------------------------------------------------------------------------------
bool Object::HasCollisionMesh() const
{
	return mModel->IsReady() && mModel->HasCollisionMesh();
}

// ------------------------------------------------------------------------------------------------
void Object::Render(RenderBuffer* buffer, RenderMode mode)
{
	ObjectRenderData* data;

	switch (mode)
	{
		case Entity::RENDER_GBUFFER:
		{
			buffer->Objects.resize(buffer->Objects.size() + 1);
			data = &(*buffer->Objects.rbegin());
			break;
		}
		case Entity::RENDER_SHADOW:
		{
			buffer->ShadowCasters.resize(buffer->ShadowCasters.size() + 1);
			data = &(*buffer->ShadowCasters.rbegin());
			break;
		}
	}
	
	data->ModelMatrix = mModelMat;
	data->TextureMatrix = glm::mat2(1.0f);
	data->Position = mPosition;
	data->model = mModel;
	data->Handle = fHandle;
	data->Skinned = false;
}

// ------------------------------------------------------------------------------------------------
void Object::OnPick(Entity *who)
{
	mPickCall.Call(who);
}

// ------------------------------------------------------------------------------------------------
void Object::OnUse(Entity *who)
{
	mUseCall.Call(who);
}

// ------------------------------------------------------------------------------------------------
void Object::Update(float time, float dt)
{
	mUpdateCall.Call(time, dt);
	Entity::Update(time, dt);
}

// ------------------------------------------------------------------------------------------------
void Object::UpdateInternals()
{
	mBoxWorld = mBoxModel.Move(mPosition);	
}
	