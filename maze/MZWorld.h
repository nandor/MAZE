// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef WORLD_H
#define WORLD_H
#pragma once

#include "MZScene.h"
#include "MZTexture.h"

namespace MAZE
{
	class Engine;
	class Camera;
	class RenderBuffer;

	/**
		Game logic
	*/
	class World
	{
	public:

		/**
			Creates a new world object
			@param engine Reference to the game engine
		*/
		World(Engine* engine);

		/**
			Destroys an existing world object
		*/
		~World();

		/**
			Initialises the world
		*/
		void Init();

		/**
			Generate the world
		*/
		void Generate();

		/**
			Update method - run in the main thread
		*/
		void Update(float time, float dt);

		/**
			Fills the back renderbuffer - run in the main thread
		*/
		void Render(RenderBuffer* buffer);

	private:

		/**
			Camera movement
		*/
		void UpdateCamera();
		
	private:

		/// Reference to the engine
		Engine *mEngine;
		
		/// Camera
		std::auto_ptr<Camera> mCamera;
				
		/// Scene
		std::auto_ptr<Scene> mScene;

		/// Frame time
		float mTime;

		/// Time difference
		float mTimeDiff;

		/// Camera y rotation
		float mRotY;

		/// Camera x rotation
		float mRotX;
		
		/// Camera velocity
		glm::vec3 mVelocity;

		/// Ground texture
		Resource::Ptr<Texture> mGroundTexture;

		/// Skybox cube map
		Resource::Ptr<Texture> mSkyTexture;

		/// Size of the world
		glm::vec3 mSize;

		/// Seed
		size_t mSeed;

		/// Global ambient
		glm::vec3 mAmbient;
	};
};

#endif /*WORLD_H*/
