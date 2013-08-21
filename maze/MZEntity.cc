// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
void Entity::PlaySound(const std::string& sound)
{
	SoundSource *src;

	mSounds.resize(mSounds.size() + 1);
	src = &(*mSounds.rbegin());

	src->SetSource(fEngine->GetResourceManager()->Get<Sound> (sound));
	src->Play();
}

// ------------------------------------------------------------------------------------------------
void Entity::Update(float time, float dt)
{
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
			mSounds.erase(mSounds.end() - 1);
		}
		else
		{
			++i;
		}
	}
}
