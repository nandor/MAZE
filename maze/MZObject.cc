// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "MZPlatform.h"
#include "MZObject.h"
#include "MZScene.h"
#include "MZRsmngr.h"
#include "MZEngine.h"
#include "MZRenderBuffer.h"
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
void Object::PlaySound(const std::string& sound)
{
	SoundSource *src;

	mSounds.resize(mSounds.size() + 1);
	src = &(*mSounds.rbegin());

	src->SetSource(fEngine->GetResourceManager()->Get<Sound> (sound));
	src->Play();
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
	data->Model = mModel;
	data->Handle = fHandle;
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
	mModelMat = glm::translate(mPosition);
	mModelMat *= glm::rotate(mRotation.x, glm::vec3(1.0, 0.0, 0.0));
	mModelMat *= glm::rotate(mRotation.y, glm::vec3(0.0, 1.0, 0.0));
	mModelMat *= glm::rotate(mRotation.z, glm::vec3(0.0, 0.0, 1.0));
	mModelMat *= glm::scale(mScale);

	for (size_t i = 0; i < mSounds.size();)
	{
		mSounds[i].Update();
		mSounds[i].SetPosition(mPosition);

		if (mSounds[i].Finished())
		{
			std::swap(mSounds[i], mSounds[mSounds.size() - 1]);
		}
		else
		{
			++i;
		}
	}
}

// ------------------------------------------------------------------------------------------------
void Object::UpdateInternals()
{
	mBoxWorld = mBoxModel.Move(mPosition);	
}
	