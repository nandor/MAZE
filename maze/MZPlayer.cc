// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZScene.h"
#include "MZEngine.h"
#include "MZRsmngr.h"
#include "MZPlayer.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
const float Player::ROTATE_SPEED = 1 / 200.0f;
const float Player::JUMP_SPEED = 0.017f;
const float Player::GRAVITY = 0.00005f;

// ------------------------------------------------------------------------------------------------
Player::Player(Engine *engine)
	: Entity(engine, PLAYER),
	  mCamera(new Camera()),
	  mVelocity(0.0f, 0.0f, 0.0f),
	  mAccel(0.0f, -GRAVITY, 0.0f),
	  mIsJumping(false),
	  mIsFalling(false),
	  mIsSprinting(false),
	  mIsMoving(true),
	  mMoveSpeed(0.007f),
	  mSize(1.0f, 2.0f, 1.0f),
	  mUseable(NULL),
	  mUsed(true)
{
	mPosition = glm::vec3(0.0f, 2.0f, 0.0f);
	mRenderable = false;

	mCrosshair = fEngine->GetResourceManager()->Get<Texture> ("cross");
	mFont = fEngine->GetResourceManager()->Get<Font> ("hud");
}

// ------------------------------------------------------------------------------------------------
Player::~Player()
{
}

// ------------------------------------------------------------------------------------------------
void Player::Update(float time, float dt)
{
	int width = fEngine->GetSetup().WindowWidth;
	int height = fEngine->GetSetup().WindowHeight;
	glm::ivec2 pos;
	glm::vec3 moveDir(0.0f), moveDist(0.0f), lookDir(0.0f), actualDist;

	// Handle the mouse
	pos = fEngine->GetMousePos();
	fEngine->SetMousePos(glm::ivec2(width, height) >> 1);

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
		moveDir = glm::normalize(moveDir) * mMoveSpeed;

		if (!mIsMoving)
		{
			mIsMoving = !mIsJumping && !mIsFalling;
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

	mBoxWorld.SetSize(1.0f, 3.0f, 1.0f);
	mBoxWorld.SetPosition(mPosition - glm::vec3(0.5f, 1.5f, 0.5f));
		
	// Move the player
	actualDist = fScene->QueryDistance(this, moveDist);
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
	
	mBoxWorld.SetSize(0.9f, 2.0f, 0.9f);
	mBoxWorld.SetPosition(mPosition - glm::vec3(0.45f, 1.5f, 0.45f));

	// Interavt with the environment
	fScene->QueryPickables(this);
	mUseable = fScene->QueryUseable(this, Ray(mPosition, lookDir));

	if (mUseable && !mUsed && fEngine->IsKeyDown(Engine::KEY_F))
	{
		mUseable->OnUse(this);
		if (mUseable->IsDeleted())
		{
			mUseable = NULL;
		}

		mUsed = true;
	}

	if (!fEngine->IsKeyDown(Engine::KEY_F))
	{
		mUsed = false;
	}
}

// ------------------------------------------------------------------------------------------------
void Player::Render(RenderBuffer *buffer, RenderMode mode)
{
	int width = fEngine->GetSetup().WindowWidth;
	int height = fEngine->GetSetup().WindowHeight;

	LightRenderData* light = buffer->AddLight();

	light->Type	= Light::POINT;
	light->Specular	= glm::vec3(1.0f, 1.0f, 1.0f);
	light->Diffuse = glm::vec3(0.88f, 0.34f, 0.13f);
	light->Position	= glm::vec4(mPosition, 20.0f);	
	light->ModelMatrix = glm::translate(mPosition);
	light->ModelMatrix *= glm::scale(glm::vec3(20.0f * 2.15));

	WidgetRenderData* cross = buffer->AddWidget();
	cross->Texture = mCrosshair;
	cross->Size = glm::vec2(mCrosshair->Width(), mCrosshair->Height());
	cross->Position = glm::floor((glm::vec2(width, height) - cross->Size) / 2.0f);
	cross->Z = 0;

	if (mUseable)
	{
		TextRenderData* text;
		
		text = buffer->AddText();
		text->Position = glm::floor(glm::vec2(width / 2.0f + 100.0f, height / 2.0f + 100.0f));
		text->Z = 0;
		text->Font = mFont;
		text->Text = mUseable->GetUseText();
	}
}

// ------------------------------------------------------------------------------------------------
void Player::UpdateInternals()
{
}
