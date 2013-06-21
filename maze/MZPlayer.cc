// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
const float Player::ROTATE_SPEED = 1 / 150.0f;
const float Player::JUMP_SPEED = 0.012f;
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
	  mUsed(false),
	  mJumped(false),
	  mMoveTime(0.0f)
{
	mBoxModel = BoundingBox(glm::vec3(-0.4f, -1.5f, -0.4f), glm::vec3(0.8f, 1.5f, 0.8f));
	mPosition = glm::vec3(0.0f, 2.0f, 0.0f);
	mRenderable = false;

	// Retrieve resources
	ResourceManager *rsmngr = fEngine->GetResourceManager();
	mCrosshair = rsmngr->Get<Texture> ("cross");
	mTorch	   = rsmngr->Get<Model> ("torch");
	mFont	   = rsmngr->Get<Font> ("hud");

	// Initialize sounds
	mWalk.SetSource(rsmngr->Get<Sound> ("walk"));
	mWalk.SetRepeat(true);
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
	mRotation.x = std::min(std::max(mRotation.x, -PIOVER4), PIOVER4);

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

		mMoveTime += dt;
		if (mIsSprinting)
		{
			moveDir *= 2.0f;
		}
	}
	else
	{
		mIsMoving = false;
		mMoveTime = 0.0f;
	}

	mVelocity.x = moveDir.x;
	mVelocity.z = moveDir.z;
			
	// Start a jump
	if (fEngine->IsKeyDown(Engine::KEY_SPACE))
	{
		if (!mIsJumping && !mIsFalling && !mJumped)
		{
			mVelocity.y = JUMP_SPEED;
			mIsJumping = true;
			mJumped = true;
		}
	}
	else
	{
		mJumped = false;
	}

	// x = x0 + v0 * t + a * t * t / 2
	moveDist = mVelocity * dt + mAccel * dt * dt * 0.5f;
	mVelocity = mVelocity + mAccel * dt;
	
	// Move the player	
	mBoxWorld = mBoxModel.Move(mPosition);
	actualDist = fScene->QueryDistance(this, moveDist);
	mPosition = mPosition + actualDist;
	mBoxWorld = mBoxModel.Move(mPosition);

	if (abs(actualDist.y) < abs(moveDist.y))
	{
		// If we cannot fall down, it means that we hit rock bottom
		if (moveDist.y > 0)
		{
			mIsJumping = true;
			mIsFalling = true;
		}
		else
		{
			mIsJumping = false;
			mIsFalling = false;
		}

		// Compute damage based on velocity
		mVelocity.y = 0.0f;
	}
	else
	{
		// The player is falling or continuing a jump
		mIsFalling = true;
	}
	
	// Update the camera position
	mCamera->SetPosition(mPosition + glm::vec3(0.0f, sin(mMoveTime / 200.0f) * 0.03f, 0.0f));
	mCamera->SetDirection(lookDir);
	mCamera->SetAspect((float)width / (float)height);

	// Update the walk sound position
	float o[] = {lookDir.x, lookDir.y, lookDir.z, 0.0f, 1.0f, 0.0f};
	alListenerfv(AL_POSITION, &mPosition[0]);
	alListenerfv(AL_ORIENTATION, o);

	glm::vec3 z = glm::normalize(lookDir);
	glm::vec3 x = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), z));

	float side = int(mMoveTime) % 1000 > 500 ? 0.4f : -0.4f;
	mWalk.SetPosition(mPosition + x * side + glm::vec3(0.0f, -1.4f, 0.0f));
	
	if (mIsMoving && !mIsJumping)
	{
		mWalk.Play();
	}
	else
	{
		mWalk.Stop();
	}
		
	// Interact with the environment
	fScene->QueryPickables(this);
	mUseable = fScene->QueryUseable(this, Ray(mPosition, lookDir));

	if (fEngine->IsKeyDown(Engine::KEY_F))
	{
		if (mUseable && !mUsed)
		{
			mUseable->OnUse(this);
			if (mUseable->IsDeleted())
			{
				mUseable = NULL;
			}

			mUsed = true;
		}
	}
	else
	{
		mUsed = false;
	}
}

// ------------------------------------------------------------------------------------------------
void Player::Render(RenderBuffer *buffer, RenderMode mode)
{
	int width = fEngine->GetSetup().WindowWidth;
	int height = fEngine->GetSetup().WindowHeight;
	glm::mat4 torchMtx;
	glm::vec3 torchPos;
	
	// Camera parameters
	glm::vec3 z = glm::normalize(mCamera->GetDirection());
	glm::vec3 x = glm::normalize(glm::cross(mCamera->GetUp(), z));
	glm::vec3 y = glm::cross(z, x);
	float n = mCamera->GetNearPlane();
	float f = mCamera->GetFarPlane();
	float h = n * tan(mCamera->GetFOV() * PI / 360.0f);
	float w = h * mCamera->GetAspect();

	// Compute the position of the torch
	torchPos = mPosition + z * n + x * w - y * h / 2.0f; 
	torchMtx = glm::translate(torchPos);
	torchMtx *= glm::rotate(mRotation.y * 180.0f / PI, glm::vec3(0.0f, 1.0f, 0.0f));

	// The light provided by the torch
	LightRenderData* light = buffer->AddLight();
	light->Type	= Light::POINT;
	light->Specular	= glm::vec3(0.88f, 0.34f, 0.13f);
	light->Diffuse = glm::vec3(0.88f, 0.34f, 0.13f);
	light->Position	= glm::vec4(torchPos, 10.0f);	
	light->ModelMatrix = glm::translate(torchPos);
	light->ModelMatrix *= glm::scale(glm::vec3(10.0f * 2.15));

	// Torch model
	ObjectRenderData* object = buffer->AddObject();
	object->model = mTorch;
	object->ModelMatrix = torchMtx;
	object->Position = torchPos;
	
	// Crosshair
	WidgetRenderData* cross = buffer->AddWidget();
	cross->texture = mCrosshair;
	cross->Size = glm::vec2(mCrosshair->Width(), mCrosshair->Height());
	cross->Position = glm::floor((glm::vec2(width, height) - cross->Size) / 2.0f);
	cross->Z = 0;

	// Use text
	TextRenderData* text;
	text = buffer->AddText();
	text->Position = glm::floor(glm::vec2(width / 2.0f + 100.0f, height / 2.0f + 100.0f));
	text->Z = 0;
	text->font = mFont;
	text->Text = mUseable ? mUseable->GetUseText() : "";
}
