// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "MZLight.h"
#include "MZScene.h"
#include "MZRenderBuffer.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
const float Light::CASCADE_SPLIT[5] = {0.0f, 0.06f, 0.15f, 0.4f, 1.0f};  

// ------------------------------------------------------------------------------------------------
void Light::Render(RenderBuffer* buffer, RenderMode mode)
{
	LightRenderData *data;

	buffer->Lights.resize(buffer->Lights.size() + 1);
	data = &(*buffer->Lights.rbegin());

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

			data->CastsShadows = mShadowCaster;
						
			if (mShadowCaster)
			{
				float n = buffer->ViewVolume.GetNearPlane();
				float f = buffer->ViewVolume.GetFarPlane();
				for (size_t j = 0; j < 4; ++j)
				{
					ViewFrustum lightvol;

					float begin = n + (f - n) * CASCADE_SPLIT[j];
					float end = n + (f - n) * CASCADE_SPLIT[j + 1];
			
					lightvol = buffer->ViewVolume.Slice(begin, end).GetLightVolume(mDirection);
					data->Shadow[j].MVP = lightvol.GetProjection() * lightvol.GetView();
					data->Shadow[j].NearZ = begin;	

					data->Shadow[j].Index = buffer->ShadowCasters.size();			
					fScene->QueryShadows(lightvol, buffer);
					data->Shadow[j].Count = buffer->ShadowCasters.size() - data->Shadow[j].Index;
				}
			}
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------------
void Light::InternalUpdate()
{
	switch (mType)
	{
		case Light::POINT:
		{		
			glm::vec3 halfSize = glm::vec3(mRadius * 2.0f);
			fBox.SetPosition(mPosition - halfSize);
			fBox.SetSize(halfSize * 2.0f);
			break;
		}
		case Light::SPOT:
		{
			glm::vec3 halfSize = glm::vec3(mRadius * 2.0f);
			fBox.SetPosition(mPosition - halfSize);
			fBox.SetSize(halfSize * 2.0f);
			break;
		}
		case Light::DIRECTIONAL:
		{
			fBox.SetPosition(glm::vec3(-1e10f));
			fBox.SetSize(glm::vec3(2e10f));
			break;
		}
	}

	fScene->UpdateEntity(this);
}

