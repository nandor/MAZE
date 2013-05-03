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
			  mPosition(0.0f),
			  mDirection(0.0f, -1.0f, 0.0f),
			  mRadius(1.0f),
			  mAngle(PIOVER4),
			  mShadowCaster(false)
		{
			mCollider = false;
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
			InternalUpdate();
		}

		/**
			Sets the position of the light
		*/
		void SetPosition(const glm::vec3& position)
		{
			mPosition = position;
			InternalUpdate();
		}

		/**
			Sets the radius of the light
		*/
		void SetRadius(float radius)
		{
			mRadius = radius;
			InternalUpdate();
		}

		/**
			Sets the direction of the spotlight
		*/
		void SetDirection(const glm::vec3& direction)
		{
			mDirection = glm::normalize(direction);
		}
		
		/**
			Sets the angle of the spotlight
		*/
		void SetAngle(float angle)
		{
			mAngle = angle;
		}

		/**
			Allow the light to cast shadows
		*/
		void SetShadowCaster(bool flag)
		{
			mShadowCaster = flag;
		}

		/**
			Computes the bounding volumes
		*/
		BoundingBox GetBoundingBox() const;

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

		void InternalUpdate();

	private:

		/// View frustum split intervals
		static const float CASCADE_SPLIT[5];

		/// Type of the light
		Type mType;

		/// Diffuse color
		glm::vec3 mDiffuse;

		/// Specular color
		glm::vec3 mSpecular;
		
		/// Position of the light
		glm::vec3 mPosition;

		/// Ambient color
		glm::vec3 mAmbient;

		/// Direction of the light
		glm::vec3 mDirection;
		
		/// Radius of the light
		float mRadius;

		/// Angle of the light (in radians)
		float mAngle;

		/// Shadow flag
		bool mShadowCaster;
	};
};

#endif
