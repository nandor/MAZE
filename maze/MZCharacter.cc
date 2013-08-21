// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
Character::Character(Engine *engine)
	: Entity(engine, CHARACTER)
{
}

// ------------------------------------------------------------------------------------------------
Character::~Character()
{
}

// ------------------------------------------------------------------------------------------------
void Character::Update(float time, float dt)
{
	Entity::Update(time, dt);
}

// ------------------------------------------------------------------------------------------------
void Character::UpdateInternals()
{
	mBoxWorld = mBoxModel.Move(mPosition);	
}

// ------------------------------------------------------------------------------------------------
void Character::Render(RenderBuffer* buffer, RenderMode mode)
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
	
	for (size_t i = 0; i < MAX_BONES; ++i)
	{
		data->Skin[i] = glm::mat4(1.0f);
	}
	
	data->TextureMatrix = glm::mat2(1.0f);
	data->ModelMatrix = mModelMat;
	data->Position = mPosition;
	data->model = mModel;
	data->Handle = fHandle;
	data->Skinned = true;
}