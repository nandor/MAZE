// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
const float Light::CASCADE_SPLIT[5] = {0.0f, 0.05f, 0.15f, 0.6f, 1.0f};  

// ------------------------------------------------------------------------------------------------
void Light::Render(RenderBuffer* buffer, RenderMode mode)
{
	LightRenderData *data;

	data = buffer->AddLight();

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
		
			data->ShadowLevels = 1;

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

				data->ShadowLevels = 4;
				for (size_t j = 0; j < 4; ++j)
				{
					Frustum lightvol;

					float begin = n + (f - n) * CASCADE_SPLIT[j];
					float end = n + (f - n) * CASCADE_SPLIT[j + 1];
			
					lightvol = buffer->ViewVolume.Slice(begin, end).GetLightVolume(mDirection);
					data->Shadow[j].MVP = lightvol.GetProjMat() * lightvol.GetViewMat();
					data->Shadow[j].NearZ = begin;	

					data->Shadow[j].Index = buffer->ShadowCasters.size();			
					fScene->QueryShadowCasters(lightvol, buffer);
					data->Shadow[j].Count = buffer->ShadowCasters.size() - data->Shadow[j].Index;
				}
			}

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------------
void Light::UpdateInternals()
{
	switch (mType)
	{
		case Light::POINT:
		{		
			glm::vec3 halfSize = glm::vec3(mRadius * 2.0f);
			mBoxWorld = BoundingBox(mPosition - halfSize, halfSize * 2.0f);
			break;
		}
		case Light::SPOT:
		{
			glm::vec3 halfSize = glm::vec3(mRadius * 2.0f);
			mBoxWorld = BoundingBox(mPosition - halfSize, halfSize * 2.0f);
			break;
		}
		case Light::DIRECTIONAL:
		{
			mBoxWorld = BoundingBox(glm::vec3(-1e10f), glm::vec3(2e10f));
			break;
		}
	}
}

