// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef ENTITY_H
#define ENTITY_H
#pragma once

namespace MAZE
{
	class Scene;
	class Engine;
	class SceneNode;
	class RenderBuffer;

    class Entity : public Aligned
	{
	public:

		/**
			Types of items stored in the Octree
		*/
		enum Type
		{
			/// Void type
			NONE,

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
			/// The entity is rendered into the geometry buffer
			RENDER_GBUFFER,

			/// The entity is rendered into a shadow map
			RENDER_SHADOW,

			/// Debug overlay is rendered after PostFX
			RENDER_DEBUG
		};

	public:

		/**	
			Creates a new entity
		*/
		Entity(Engine *engine, Type type)
			: mCollider(false),
			  mPickable(false),
			  mUseable(false),
			  mUseText("Activate"),
			  mRenderable(true),
			  mShadowCaster(false),
			  mActive(true),
			  mPosition(0.0f),
			  mRotation(0.0f),
			  mScale(1.0f),
			  mType(type),
			  mBoxModel(glm::vec3(-0.5f), glm::vec3(1.0f)),
			  mBoxWorld(glm::vec3(-0.5f), glm::vec3(1.0f)),
			  fName(""),
			  fHandle(0),
			  fScene(NULL),
			  fEngine(engine),
			  fParentNode(NULL),
			  fDelete(false),
			  fDirty(true)
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
		bool IsUseable() const
		{
			return mUseable;
		}

		/**
			Returns the text displayed when
			the entity can be used
		*/
		std::string GetUseText() const
		{
			return mUseText;
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
		Type GetType() const
		{
			return mType;
		}

		/**
			Returns the handle
		*/
		unsigned GetHandle() const
		{
			return fHandle;
		}

		/**
			Returns the identifier
		*/
		std::string GetName() const
		{
			return fName;
		}
		
		/**
			Returns the position of the object
		*/
		glm::vec3 GetPosition() const
		{
			return mPosition;
		}	

		/**
			Returns the rotation of the object
		*/
		glm::vec3 GetRotation() const
		{
			return mRotation;
		}	

		/**
			Returns the scaleing of the object
		*/
		glm::vec3 GetScale() const
		{
			return mScale;
		}	
				
		/**
			Returns the bounding box of the entity
		*/
		BoundingBox GetBoundingBox() const
		{
			return mBoxWorld;
		}

		/**
			Returns the model matrix
		*/
		glm::mat4 GetModelMat() const
		{
			return mModelMat;
		}

		/**
			Checks if the entity was queued for deletion
		*/
		bool IsDeleted() const
		{
			return fDelete;
		}

		/**
			Check if the object can be deleted
		*/
		virtual bool IsDeletable() const
		{
			return mSounds.size() == 0;
		}

		/**
			Checks if the entity has a collision mesh
		*/
		virtual bool HasCollisionMesh() const
		{
			return false;
		}

		/**
			Prevents the entity from colliding
		*/
		void SetCollider(bool flag)
		{
			mCollider = flag;
			mPickable = !flag;
		}

		/**
			Allows the object to be picked up
		*/
		void SetPickable(bool flag)
		{
			mPickable = flag;
			mCollider = !flag;
			mUseable  = !flag;
		}

		/**
			Activates the object to be activated by pressing 'F'
		*/
		void SetUseable(bool flag)
		{
			mUseable  = flag;
			mPickable = !flag;
		}

		/**
			Changes the use text of the object
		*/
		void SetUseText(const std::string& text)
		{
			mUseText = text;
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
			mShadowCaster = flag;
		}	

		/**
			Activates the object
		*/
		void SetActive(bool flag)
		{
			mActive = flag;
		}	

		/**
			Sets the position of the object
		*/
		void SetPosition(float x, float y, float z)
		{
			mPosition = glm::vec3(x, y, z);
			fDirty = true;
		}

		/**
			Sets the position of the player
		*/
		void SetPosition(const glm::vec3& position)
		{
			mPosition = position;
			fDirty = true;
		}
		
		/**
			Sets the rotation of the object
		*/
		void SetRotation(float x, float y, float z)
		{
			mRotation = glm::vec3(x, y, z);
		}
		
		/**
			Sets the rotation of the object
		*/
		void SetRotation(const glm::vec3& v)
		{
			mRotation = v;
		}
		
		/**
			Sets the scale of the object
		*/
		void SetScale(float x, float y, float z)
		{
			mScale = glm::vec3(x, y, z);
		}

		/**
			Sets the rotation of the object
		*/
		void SetScale(const glm::vec3& v)
		{
			mScale = v;
		}

		/**
			Sets the model space bounding box
		*/
		void SetBoundingBox(const BoundingBox& box)
		{
			mBoxModel = box;
			fDirty = true;
		}

		/**
			Marks the entity for deletion
		*/
		void Delete()
		{
			fDelete = true;
			mActive = false;
		}

		/**
			Called when the object is picked
		*/
		virtual void OnPick(Entity *who)
		{
		}
		
		/**
			Called when the object is used
		*/
		virtual void OnUse(Entity *who)
		{
		}

		/**
			Plays a sound
		*/
		void PlaySound(const std::string& sound);

		/**
			Updates the entity on each frame
			@param time Total elapsed time
			@param dt	Time since last updated
		*/
		virtual void Update(float time, float dt);

		/**
			Places the entity in the renderbuffer
		*/
		virtual void Render(RenderBuffer* buffer, RenderMode mode) = 0;

	protected:

		/**
			Update the internal, cached objects of the entity
		*/
		virtual void UpdateInternals() = 0;

	protected:

		/// True if the entity collides with the player
		bool mCollider;

		/// True if the entity can be picked up by the player
		bool mPickable;
		
		/// True if the player can use the entity
		bool mUseable;

		/// Text displayed when the entity can be used
		std::string mUseText;

		/// True if the object is renderable
		bool mRenderable;

		/// True if the object casts shadows
		bool mShadowCaster;
				
		/// True if the entity is active
		bool mActive;
		
		/// Position of the entity in the world
		glm::vec3 mPosition;

		/// Rotation of the entity
		glm::vec3 mRotation;

		/// Scale of the entity
		glm::vec3 mScale;

		/// Model matrix
		glm::mat4 mModelMat;

		/// Type of the entity
		Type mType;
		
		/// Bounding box in model space
		BoundingBox mBoxModel;

		/// Bounding box in world space
		BoundingBox mBoxWorld;
		
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

		/// True if the entity is queued for deletion
		bool fDelete;

		/// True if the entity was modifies
		bool fDirty;
		
		/// List of sound sources
		std::vector<SoundSource> mSounds;
		
		/// Scene needs access to Entity
		friend class Scene;
	};
};

#endif
