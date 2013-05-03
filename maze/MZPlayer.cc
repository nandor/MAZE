// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZScene.h"
#include "MZEngine.h"
#include "MZPlayer.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
const float Player::ROTATE_SPEED = 1 / 200.0f;
const float Player::MOVE_SPEED = 0.007f;
const float Player::JUMP_SPEED = 0.015f;
const float Player::GRAVITY = 0.00005f;

// ------------------------------------------------------------------------------------------------
Player::Player()
	: Entity(PLAYER),
	  mPosition(0.0f, 2.0f, 0.0f),
	  mRotation(0.0f, 0.0f, 0.0f),
	  mVelocity(0.0f, 0.0f, 0.0f),
	  mAccel(0.0f, -GRAVITY, 0.0f),
	  mIsJumping(false),
	  mIsFalling(false),
	  mIsSprinting(false),
	  mIsMoving(true),
	  mSize(1.0f, 2.0f, 1.0f)
{
	mCamera = new Camera();
}

// ------------------------------------------------------------------------------------------------
Player::~Player()
{
	if (mCamera) { delete mCamera; mCamera = NULL; }
}

// ------------------------------------------------------------------------------------------------
void Player::Update(float time, float dt)
{
	int width = fEngine->GetConfig().WindowWidth;
	int height = fEngine->GetConfig().WindowHeight;
	glm::ivec2 pos = fEngine->GetMousePos();
	glm::vec3 moveDir(0.0f), moveDist(0.0f), lookDir(0.0f), actualDist;

	// Compute rotation
	mRotation.y += ((width >> 1) - pos.x) * ROTATE_SPEED;
	mRotation.x += ((height >> 1) - pos.y) * ROTATE_SPEED;	
	mRotation.x = std::min(std::max(mRotation.x, -PIOVER2 + 0.1f), PIOVER2 - 0.1f);

	// Camera orientation
	lookDir.x = sin(mRotation.y) * cos(mRotation.x);
	lookDir.y = sin(mRotation.x);
	lookDir.z = cos(mRotation.y) * cos(mRotation.x);
	
	// Movement direction
	if (fEngine->IsKeyDown(Engine::KEY_W))
	{
		moveDir.x += sin(mRotation.y) * cos(mRotation.x);
		moveDir.z += cos(mRotation.y) * cos(mRotation.x);
	}

	if (fEngine->IsKeyDown(Engine::KEY_S))
	{
		moveDir.x -= sin(mRotation.y) * cos(mRotation.x);
		moveDir.z -= cos(mRotation.y) * cos(mRotation.x);
	}

	if (fEngine->IsKeyDown(Engine::KEY_A))
	{
		moveDir.x += sin(mRotation.y + PIOVER2) * cos(mRotation.x);
		moveDir.z += cos(mRotation.y + PIOVER2) * cos(mRotation.x);
	}

	if (fEngine->IsKeyDown(Engine::KEY_D))
	{
		moveDir.x += sin(mRotation.y - PIOVER2) * cos(mRotation.x);
		moveDir.z += cos(mRotation.y - PIOVER2) * cos(mRotation.x);
	}

	// Compute movement direction & speed
	if (glm::length(moveDir) > 0.0f)
	{
		moveDir = glm::normalize(moveDir) * MOVE_SPEED;

		if (!mIsMoving)
		{
			mIsMoving = !mIsJumping && !mIsFalling;

			if (mIsMoving)
			{
				mMoveStart = time;
			}
		}

		if (mIsJumping)
		{
			moveDir *= 0.7f;
		}
		
		if (mIsSprinting)
		{
			moveDir *= 2.0f;
		}
	}
	else
	{
		mIsMoving = false;
	}

	mVelocity.x = moveDir.x;
	mVelocity.z = moveDir.z;
			
	// Start a jump
	if (fEngine->IsKeyDown(Engine::KEY_SPACE) && !mIsJumping)
	{
		mVelocity.y = JUMP_SPEED;
		mIsJumping = true;
	}

	// x = x0 + v0 * t + a * t * t / 2
	moveDist = mVelocity * dt + mAccel * dt * dt * 0.5f;
	mVelocity = mVelocity + mAccel * dt;

	fBox.SetSize(1.0f, 3.0f, 1.0f);
	fBox.SetPosition(mPosition - glm::vec3(0.5f, 1.5f, 0.5f));
		
	// Move the player
	actualDist = fScene->QueryDistance(fBox, moveDist);
	mPosition = mPosition + actualDist;

	if (abs(actualDist.y) < abs(moveDist.y))
	{
		// If we cannot fall down, it means that we are not falling anymore
		mIsJumping = false;
		mIsFalling = false;

		// Compute damage based on velocity
		mVelocity.y = 0.0f;
	}
	else
	{
		// The player is falling or continuing a jump
		mIsFalling = true;
	}

	mCamera->SetPosition(mPosition);
	mCamera->SetDirection(lookDir);
	mCamera->SetAspect((float)width / (float)height);
	
	fEngine->SetMousePos(glm::ivec2(width, height) >> 1);
}

// ------------------------------------------------------------------------------------------------
void Player::Render(RenderBuffer *buffer, RenderMode mode)
{
	LightRenderData* light = buffer->AddLight();

	light->Type	= Light::POINT;
	light->Specular	= glm::vec3(1.0f, 1.0f, 1.0f);
	light->Diffuse = glm::vec3(0.88f, 0.34f, 0.13f);
	light->Position	= glm::vec4(mPosition, 20.0f);	
	light->ModelMatrix = glm::translate(mPosition);
	light->ModelMatrix *= glm::scale(glm::vec3(20.0f * 2.15));
}

// ------------------------------------------------------------------------------------------------
void Player::InternalUpdate()
{
}
