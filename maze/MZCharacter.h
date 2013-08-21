// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZCHARACTER_H
#define MZCHARACTER_H
#pragma once

namespace MAZE
{
	class Character : public Entity
	{
	public:

		/**
			Creates a new object
		*/
		Character(Engine *engine);
		
		/**
			Destroys the object
		*/
		~Character();
		
		/**
			Updates the entity
		*/
		void Update(float time, float dt);

		/**
			Places the character in the renderbuffer
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

	private:

		/**
			Update the internal, cached objects of the entity
		*/
		void UpdateInternals();

	protected:
		
		/// Model used by this entity
		Resource::Ptr<Model> mModel;
	};
};

#endif