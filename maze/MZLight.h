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
	struct LightRenderData;

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
		Light(Type type) 
			: Entity(LIGHT), 
			  mType(type),
			  mDiffuse(1.0f, 1.0f, 1.0f),
			  mSpecular(1.0f, 1.0f, 1.0f),
			  mAmbient(0.2f, 0.2f, 0.2f),
			  mPosition(0.0f),
			  mDirection(0.0f, -1.0f, 0.0f),
			  mRadius(1.0f),
			  mAngle(PIOVER4)
		{
			mCollider = false;
		}

		/**
			Destroys the light
		*/
		virtual ~Light()
		{
		}

		/**
			Sets the diffuse color of the light
			@param diffuse Diffuse color
		*/
		void SetDiffuse(const glm::vec3& diffuse)
		{
			mDiffuse = diffuse;
		}

		/**
			Sets the specular color of the light
			@param specular Specular color
		*/
		void SetSpecular(const glm::vec3& specular)
		{
			mSpecular = specular;
		}

		/**
			Sets the position of the light
			@param diffuse Diffuse color
		*/
		void SetPosition(const glm::vec3& position)
		{
			mPosition = position;
			mDirty = true;
		}

		/**
			Sets the radius of the light
			@param radius Light radius
		*/
		void SetRadius(float radius)
		{
			mRadius = radius;
			mDirty = true;
		}

		/**
			Sets the direction of the spotlight
			@param direction Direction of the spotlight
		*/
		void SetDirection(const glm::vec3& direction)
		{
			mDirection = glm::normalize(direction);
			mDirty = true;
		}
		
		/**
			Sets the angle of the spotlight
			@param angle Angle of the spotlight
		*/
		void SetAngle(float angle)
		{
			mAngle = angle;
			mDirty = true;
		}

		/**
			Copies light data to the renderbuffer
			@param buffer Target renderbuffer
		*/
		void Prepare(LightRenderData* buffer);
		
		/**
			Computes the bounding volumes
		*/
		BoundingBox GetBoundingBox() const;

		/**
			Updates the objects (removes dirty flag)
		*/
		void Update();

	private:

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
	};
};

#endif
