// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include "MZPlatform.h"
#include "MZRenderer.h"
#include "MZBound.h"

namespace MAZE
{
	class Engine;
	class RenderBuffer;

	class Camera
	{
	public:

		/**
			Creates a new camera
		*/
		Camera();

		/**
			Destroys the camera
		*/
		~Camera();
		
		/**
			Sets the position of the camera
			@param pos Camera position
		*/
		void SetPosition(const glm::vec3& pos)
		{
			mPosition = pos;
			mDirty = true;
		}

		/**
			Sets the orientation of the camera
			@param target Camera target
		*/
		void SetTarget(const glm::vec3& target)
		{
			mDirection = mPosition - target;
			mDirty = true;
		}

		/**
			Sets the orientation of the camera
			@param dir Camera direction
		*/
		void SetDirection(const glm::vec3& dir)
		{
			mDirection = glm::normalize(dir);
			mDirty = true;
		}

		/**
			Changes the aspect ratio
		*/
		void SetAspect(float aspect)
		{
			mAspect = aspect;
			mDirty = true;
		}

		/**
			Returns the direction of the camera
		*/
		glm::vec3 GetDirection()
		{
			return mDirection;
		}

		/**
			Returns the position of the camera
		*/
		glm::vec3 GetPosition()
		{
			return mPosition;
		}

		/**
			Returns the up vector
		*/
		glm::vec3 GetUp()
		{
			return mUp;
		}

		/**
			Retrieves the view volume
		*/
		ViewFrustum& GetVolume();

		/**
			Prepares render data
		*/
		void Render(RenderBuffer*);

	private:

		/// Change flag
		bool mDirty;
		
		/// Position of the camera
		glm::vec3 mPosition;

		/// Direction
		glm::vec3 mDirection;

		/// Upvard direction
		glm::vec3 mUp;

		/// Aspect ratio
		float mAspect;

		/// Near plane
		float mNearPlane;

		/// Far plane
		float mFarPlane;

		/// Focal point
		float mFocus;
		
		/// Field of view
		float mFOV;

		/// View frustum
		ViewFrustum mFrustum;

		/// World needs to access prepare
		friend class World;
	};
};

#endif
