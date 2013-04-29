// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "MZPlatform.h"
#include "MZObject.h"
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
void Object::Prepare(ObjectRenderData* data)
{
	data->ModelMatrix = glm::translate(mPosition);
	data->NormalMatrix = glm::inverseTranspose(data->ModelMatrix);
	data->TextureMatrix = glm::mat2(1.0f);
	data->Model = mModel;
}

// ------------------------------------------------------------------------------------------------
void Object::Update()
{
	mBox.SetPosition(mPosition + mBoundingBox.GetPosition());
	mBox.SetSize(mBoundingBox.GetSize());
	mDirty = false;
}