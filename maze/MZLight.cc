// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "MZLight.h"
#include "MZException.h"
#include "MZRenderBuffer.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
void Light::Prepare(LightRenderData* data)
{
	data->Type = mType;
	data->Specular = mSpecular;	
	data->Diffuse = mDiffuse;
	data->Ambient = mAmbient;

	switch (mType)
	{
		case Light::POINT:
		{
			data->Position.x = mPosition.x;
			data->Position.y = mPosition.y;
			data->Position.z = mPosition.z;
			data->Position.w = mRadius;

			data->Direction.x = 0.0f;
			data->Direction.y = 0.0f;
			data->Direction.z = 0.0f;
			data->Direction.w = -1.0f;
	
			data->ModelMatrix = glm::translate(mPosition);
			data->ModelMatrix *= glm::scale(glm::vec3(mRadius * 2.15));
			break;
		}
		case Light::SPOT:
		{
			float d = mRadius * tan(mAngle / 2.0f) * 2.15f;
			
			data->Position.x = mPosition.x;
			data->Position.y = mPosition.y;
			data->Position.z = mPosition.z;
			data->Position.w = mRadius;

			data->Direction.x = mDirection.x;
			data->Direction.y = mDirection.y;
			data->Direction.z = mDirection.z;
			data->Direction.w = cos(mAngle / 2.0f);
			
			data->ModelMatrix = glm::translate(mPosition);
			data->ModelMatrix *= glm::orientation(mDirection, glm::vec3(1.0f, 0.0f, 0.0f));
			data->ModelMatrix *= glm::scale(mRadius, d, d);
		
			data->NormalMatrix = glm::inverseTranspose(data->ModelMatrix);
			break;
		}
		case Light::DIRECTIONAL:
		{
			data->Position.x = 0.0f;
			data->Position.y = 0.0f;
			data->Position.z = 0.0f;
			data->Position.w = -1.0f;
	
			data->Direction.x = mDirection.x;
			data->Direction.y = mDirection.y;
			data->Direction.z = mDirection.z;
			data->Direction.w = -1.0f;
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------------
void Light::Update()
{
	switch (mType)
	{
		case Light::POINT:
		{		
			mBox.SetPosition(mPosition - glm::vec3(mRadius));
			mBox.SetSize(glm::vec3(mRadius) * 2.0f);
			break;
		}
		case Light::SPOT:
		{
			mBox.SetPosition(mPosition - glm::vec3(mRadius));
			mBox.SetSize(glm::vec3(mRadius) * 2.0f);
			break;
		}
		case Light::DIRECTIONAL:
		{
			mBox.SetPosition(glm::vec3(-1e10f));
			mBox.SetSize(glm::vec3(2e10f));
			break;
		}
	}

	mDirty = false;
}

