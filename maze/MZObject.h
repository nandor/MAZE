// This f/ile is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef OBJECT_H
#define OBJECT_H
#pragma once

#include "MZMath.h"
#include "MZModel.h"
#include "MZEntity.h"

namespace MAZE
{
	/// Data passed to the renderer
	class ObjectRenderData;

	/**

	*/
	class Object : public Entity
	{
	public:

		/**
			Creates a new object
		*/
		Object();
		
		/**
			Destroys the object
		*/
		~Object();

		/**
			Sets the model of the object
		*/
		void SetModel(Resource::Ptr<Model> model)
		{
			mModel = model;
		}

		/**
			Sets the position of the object
		*/
		void SetPosition(const glm::vec3& position)
		{
			mPosition = position;
			InternalUpdate();
		}

		/**
			Sets the rotation of the object
		*/
		void SetRotation(float x, float y, float z)
		{
			mRotation = glm::vec3(x, y, z);
			InternalUpdate();
		}
		
		/**
			Attaches a bounding box to the object
		*/
		void SetBoundingBox(const BoundingBox& box)
		{
			mBoundingBox = box;
			InternalUpdate();
		}

		/**
			Allows the object to cast shadows
		*/
		void SetShadowCaster(bool flag)
		{
			mShadowCaster = true;
		}

		/**
			Places the entity in the renderbuffer
		*/
		void Render(RenderBuffer* buffer, RenderMode mode);

		/**
			Updates the entity
		*/
		void Update(float time, float dt)
		{
		}

	private:

		/**
			Updates the object
		*/
		void InternalUpdate();

	private:

		/// Model used by this entity
		Resource::Ptr<Model> mModel;

		/// Position of the entity
		glm::vec3 mPosition;

		/// Rotation of the entity
		glm::vec3 mRotation;

		/// Texture matrix
		glm::mat2 mTexture;

		/// Bounding box of the entity
		BoundingBox mBoundingBox;
	};
};

#endif
