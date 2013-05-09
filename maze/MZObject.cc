// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "MZPlatform.h"
#include "MZObject.h"
#include "MZScene.h"
#include "MZRenderBuffer.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
Object::Object()
	: Entity(Entity::OBJECT)
{
}

// ------------------------------------------------------------------------------------------------
Object::~Object()
{
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
	
	data->ModelMatrix  = glm::translate(mPosition);
	data->ModelMatrix *= glm::rotate(mRotation.x, glm::vec3(1.0, 0.0, 0.0));
	data->ModelMatrix *= glm::rotate(mRotation.y, glm::vec3(0.0, 1.0, 0.0));
	data->ModelMatrix *= glm::rotate(mRotation.z, glm::vec3(0.0, 0.0, 1.0));

	data->TextureMatrix = glm::mat2(1.0f);
	data->Position = mPosition;
	data->Model = mModel;
	data->Handle = fHandle;
}

// ------------------------------------------------------------------------------------------------
void Object::UpdateInternals()
{
	mBoxWorld.SetPosition(mPosition + mBoxModel.GetPosition());
	mBoxWorld.SetSize(mBoxModel.GetSize());
}
	