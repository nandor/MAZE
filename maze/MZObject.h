// This f/ile is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef OBJECT_H
#define OBJECT_H
#pragma once

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
		Object(Engine *engine);
		
		/**
			Destroys the object
		*/
		~Object();
		
		/**
			Checks if the obj has a collision mesh
		*/
		bool HasCollisionMesh() const;

		/**
			Plays a sound
		*/
		void PlaySound(const std::string& sound);
		
		/**
			Called when the object is picked
		*/
		void OnPick(Entity *who);
		
		/**
			Called when the object is used
		*/
		void OnUse(Entity *who);

		/**
			Updates the entity
		*/
		void Update(float time, float dt);

		/**
			Places the entity in the renderbuffer
		*/
		void Render(RenderBuffer* buffer, RenderMode mode);

	public:

		/**
			Returns a reference to the model
		*/
		Resource::Ptr<Model> GetModel() const
		{
			return mModel;
		}

		/**
			Sets the model of the object
		*/
		void SetModel(Resource::Ptr<Model> model)
		{
			mModel = model;
		}

		/**
			Sets the OnPick callback
		*/
		void SetPickCall(const Callback& call)
		{
			mPickCall = call;
		}

		/**
			Sets the OnUpdate
		*/
		void SetUpdateCall(const Callback& call)
		{
			mUpdateCall = call;
		}

		/**
			Sets the OnUse callback
		*/
		void SetUseCall(const Callback& call)
		{			
			mUseCall = call;
		}

	private:

		/**
			Updates the object
		*/
		void UpdateInternals();
		
	private:

		/// Model used by this entity
		Resource::Ptr<Model> mModel;

		/// Callback used when the object is picked up
		Callback mPickCall;

		/// Callback in the update loop
		Callback mUpdateCall;

		/// Callback on use of the object
		Callback mUseCall;
		
		/// List of sound sources
		std::vector<SoundSource> mSounds;
	};
};

#endif
