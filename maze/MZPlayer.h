// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZPLAYER_H
#define MZPLAYER_H
#pragma once

#include "MZMath.h"
#include "MZSound.h"
#include "MZEntity.h"
#include "MZCamera.h"
#include "MZSoundSource.h"

namespace MAZE
{
	class Player : public Entity
	{
	public:
		
		/**
			Initializes the player
		*/
		Player(Engine* engine);

		/**
			Destroys the player
		*/
		~Player();
		
		/**
			Checks if the object is jumping
		*/
		bool IsJumping() const
		{
			return mIsJumping;
		}

		/**
			Checks if the object is falling
		*/
		bool IsFalling() const
		{
			return mIsFalling;
		}

		/**
			Checks if the object is sprinting
		*/
		bool IsSprinting() const
		{
			return mIsSprinting;
		}

		/**
			Checks if the object is sprinting
		*/
		bool IsMoving() const
		{
			return mIsMoving;
		}
		
		/**
			Returns the movement speed
		*/
		float GetMoveSpeed() const
		{
			return mMoveSpeed;
		}

		/**
			Returns the camera attached to the player
		*/
		Camera* GetCamera()
		{
			return mCamera.get();
		}

		/**
			Changes the movement speed
		*/
		void SetMoveSpeed(float speed)
		{
			mMoveSpeed = speed;
		}

		/**
			Places the entity in the renderbuffer
		*/
		void Render(RenderBuffer* buffer, RenderMode mode);

		/**
			Updates the player
			@param time Last frame time
			@param dt	Time since last frame
		*/
		void Update(float time, float dt);

	private:
		
		void UpdateInternals()
		{
		}
		
	private:
		
		/// Camera move speed
		static const float ROTATE_SPEED;
				
		/// Player movement speed
		static const float MOVE_SPEED;

		/// Jump speed - determines jump height
		static const float JUMP_SPEED;

		/// Gravitational acceleration
		static const float GRAVITY;
		
		/// Crosshair texture
		Resource::Ptr<Texture> mCrosshair;

		/// UI font
		Resource::Ptr<Font> mFont;

		/// Torch model
		Resource::Ptr<Model> mTorch;

		/// Walk sound
		SoundSource mWalk;

		/// FPS camera
		std::auto_ptr<Camera> mCamera;
			
		/// Velocity
		glm::vec3 mVelocity;

		/// Acceleration
		glm::vec3 mAccel;

		/// Jump flag
		bool mIsJumping;

		/// Fall flag
		bool mIsFalling;

		/// Sprint flag
		bool mIsSprinting;

		/// Movement flag
		bool mIsMoving;

		/// Movement speed
		float mMoveSpeed;
		
		/// Size of the entity
		glm::vec3 mSize;

		/// Closest object which is useable
		Entity *mUseable;

		/// True if we've just used an object
		bool mUsed;

		/// True if we've just jumped
		bool mJumped;

		/// Movement oscillation timer
		float mMoveTime;
	};
};

#endif