// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef LIGHT_H
#define LIGHT_H
#pragma once

#include "MZMath.h"
#include "MZEntity.h"

namespace MAZE
{
	class LightRenderData;

	/**
		Base class for lights
	*/
	class Light : public Entity
	{
	public:

		/**
			Type of the light
		*/
		enum Type
		{
			DIRECTIONAL = 1,
			POINT		= 2,
			SPOT		= 3
		};
		
	public:

		/**
			Creates a new light
			@param type Type of the light
		*/
		Light() 
			: Entity(LIGHT), 
			  mType(Light::POINT),
			  mDiffuse(1.0f, 1.0f, 1.0f),
			  mSpecular(1.0f, 1.0f, 1.0f),
			  mAmbient(0.2f, 0.2f, 0.2f),
			  mDirection(0.0f, -1.0f, 0.0f),
			  mRadius(1.0f),
			  mAngle(PIOVER4)
		{
			mCollider = false;
		}

		/**
			Returns the diffuse color of the light
		*/
		glm::vec3 GetDiffuse() const
		{
			return mDiffuse;
		}

		/**
			Returns the specular color of the light
		*/
		glm::vec3 GetSpecular() const
		{
			return mSpecular;
		}

		/**
			Returns the ambient color of the light
		*/
		glm::vec3 GetAmbient() const
		{
			return mAmbient;
		}

		/**
			Returns the type of light
		*/
		Type GetType() const
		{
			return mType;
		}

		/**
			Returns the radius of the light
		*/
		float GetRadius() const
		{
			return mRadius;
		}

		/**
			Returns the direction of the spotlight
		*/
		glm::vec3 GetDirection() const
		{
			return mDirection;
		}
		
		/**
			Returns the angle of the spotlight
		*/
		float GetAngle() const
		{
			return mAngle;
		}
		
		
		/**
			Sets the diffuse color of the light
		*/
		void SetDiffuse(const glm::vec3& diffuse)
		{
			mDiffuse = diffuse;
		}

		/**
			Sets the specular color of the light
		*/
		void SetSpecular(const glm::vec3& specular)
		{
			mSpecular = specular;
		}

		/**
			Sets the ambient color of the light
		*/
		void SetAmbient(const glm::vec3& ambient)
		{
			mAmbient = ambient;
		}

		/**
			Sets the type of light
		*/
		void SetType(Type type)
		{
			mType = type;
			fDirty = true;
		}

		/**
			Sets the radius of the light
		*/
		void SetRadius(float radius)
		{
			mRadius = radius;
			fDirty = true;
		}

		/**
			Sets the direction of the spotlight
		*/
		void SetDirection(const glm::vec3& direction)
		{
			mDirection = glm::normalize(direction);
			fDirty = true;
		}
		
		/**
			Sets the angle of the spotlight
		*/
		void SetAngle(float angle)
		{
			mAngle = angle;
			fDirty = true;
		}
		
		/**
			Places the entity in the renderbuffer
		*/
		void Render(RenderBuffer* buffer, RenderMode mode);
		
		/**
			Nothing to update here...
			@param time Total game time
			@param dt	Time difference
		*/
		void Update(float time, float dt)
		{
		}
		
	private:

		void UpdateInternals();

	private:

		/// View frustum split intervals
		static const float CASCADE_SPLIT[5];

		/// Type of the light
		Type mType;

		/// Diffuse color
		glm::vec3 mDiffuse;

		/// Specular color
		glm::vec3 mSpecular;
		
		/// Ambient color
		glm::vec3 mAmbient;

		/// Direction of the light
		glm::vec3 mDirection;
		
		/// Radius of the light
		float mRadius;

		/// Angle of the light (in radians)
		float mAngle;
	};
};

#endif
