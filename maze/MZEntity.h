// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef ENTITY_H
#define ENTITY_H
#pragma once

#include "MZMath.h"
#include "MZBound.h"
#include "MZRenderBuffer.h"

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
			PARTICLE,

			/// It's complicated
			PLAYER,

			/// Draw debug data
			DEBUG
		};

		/**
			Render modes
		*/
		enum RenderMode
		{
			/// The entity is rendered into the GBuffer
			RENDER_GBUFFER,

			/// The entity is rendered into a shadow map
			RENDER_SHADOW

		};

	public:

		/**	
			Creates a new entity
		*/
		Entity(Type type)
			: mType(type),
			  mActive(true),
			  fName(""),
			  fScene(NULL),
			  fParentNode(NULL)
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
			Check if the object can be picked up
		*/
		bool IsPickable() const
		{
			return mPickable;
		}

		/**
			Check if the object can be used
		*/
		bool IsUsable() const
		{
			return mUsable;
		}
					
		/**
			Returns true if the entity is renderable
		*/
		bool IsRenderable() const
		{
			return mRenderable;
		}
		
		/**
			Returns true if the entity casts shadows
		*/
		bool IsShadowCaster() const
		{
			return mShadowCaster;
		}

		/**
			Checks if the entity is active or not
		*/
		bool IsActive() const
		{
			return mActive;
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
			return fBox;
		}

		/**
			Prevents the entity from colliding
		*/
		void SetCollider(bool flag)
		{
			mCollider = flag;
		}

		/**
			Allows the object to be picked up
		*/
		void SetPickable(bool flag)
		{
			mPickable = flag;
		}

		/**
			Activates the object to be activated by pressing 'F'
		*/
		void SetUsable(bool flag)
		{
			mUsable = flag;
		}

		/**
			Allows the object to be rendered
		*/
		void SetRenderable(bool flag)
		{
			mRenderable = flag;
		}	

		/**
			Allows the object to cast shadows
		*/
		void SetShadowCaster(bool flag)
		{
			mRenderable = flag;
		}	

		/**
			Activates the object
		*/
		void SetActive(bool flag)
		{
			mActive = flag;
		}				

		/**
			Updates the entity on each frame
			@param time Total elapsed time
			@param dt	Time since last updateo
		*/
		virtual void Update(float time, float dt) = 0;

		/**
			Places the entity in the renderbuffer
		*/
		virtual void Render(RenderBuffer* buffer, RenderMode mode) = 0;

	protected:

		/// True if the entity collides with the player
		bool mCollider;

		/// True if the entity can be picked up by the player
		bool mPickable;

		/// True if the player can use the entity
		bool mUsable;

		/// True if the object is renderable
		bool mRenderable;

		/// True if the object casts shadows
		bool mShadowCaster;
				
		/// True if the entity is active
		bool mActive;

		/// Type of the entity
		Type mType;

		/// ID of the entity
		std::string fName;

		/// Unique numeric handle
		unsigned fHandle;

		/// Parent scene manager
		Scene *fScene;

		/// Parent engine instance
		Engine* fEngine;

		/// Parent octree node
		SceneNode *fParentNode;

		/// Bounding box
		BoundingBox fBox;

		/// Scene is our friend
		friend class Scene;
	};
};

#endif