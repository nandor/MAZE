// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef ENTITY_H
#define ENTITY_H
#pragma once

#include "MZMath.h"
#include "MZBound.h"

namespace MAZE
{
	class SceneNode;
	class Scene;

    class Entity
	{
	public:

		/**
			Types of items stored in the Octree
		*/
		enum Type
		{
			/// Light
			LIGHT,
				
			/// Mesh, character etc
			OBJECT,

			/// Billboard - faces the camera
			BILLBOARD,

			/// Particle engine
			PARTICLE
		};

	public:

		/**	
			Creates a new entity
		*/
		Entity(Type type)
			: mDirty(true),
			  mType(type),
			  mParentNode(NULL)
		{
		}

		/**	
			Destroys the entity
		*/
		virtual ~Entity()
		{
		}

		/**
			Returns true if the entity collides with others
		*/
		bool IsCollider() const
		{
			return mCollider;
		}
					
		/**
			Returns true if the entity is renderable
		*/
		bool IsRenderable() const
		{
			return mRenderable;
		}

		/**	
			Checks if the node is dirty
		*/
		bool IsDirty() const
		{
			return mDirty;
		}

		/**
			Returns true if the entity casts shadows
		*/
		bool IsShadowCaster() const
		{
			return mShadowCaster;
		}
				
		/**
			Returns the type of the entity
		*/
		Type GetEntityType() const
		{
			return mType;
		}
				
		/**
			Returns the bounding box of the entity
		*/
		BoundingBox GetBoundingBox() const
		{
			return mBox;
		}

		/**
			Updates the entity
		*/
		virtual void Update() = 0;

	protected:

		/// True if the entity collides with the player
		bool mCollider;

		/// True if the object is renderable
		bool mRenderable;

		/// True if the object casts shadows
		bool mShadowCaster;

		/// True if the entity needs to be updated
		bool mDirty;
				
		/// Type of the entity
		Type mType;

		/// Parent octree node
		SceneNode* mParentNode;

		/// Bounding box
		BoundingBox mBox;

		/// Scene is our friend
		friend class Scene;
	};
};

#endif