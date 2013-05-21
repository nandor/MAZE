// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "MZPlatform.h"
#include "MZBound.h"
#include "MZCamera.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
Camera::Camera()
	: mPosition(glm::vec3(0.0, 5.0, 0.0)),
	  mDirection(glm::vec3(0.0, 0.0, -1.0)),
	  mUp(glm::vec3(0.0, 1.0, 0.0)),
	  mNearPlane(0.3f),
	  mFarPlane(30.0f),
	  mFocus(15.0f),
	  mFOV(45.0f),
	  mAspect(800.0f / 600.0f),
	  mDirty(true)
{
}

// ------------------------------------------------------------------------------------------------
Camera::~Camera()
{
}

// ------------------------------------------------------------------------------------------------
void Camera::Render(RenderBuffer* buf)
{
	buf->Camera.ProjMatrix = glm::perspective(mFOV, mAspect, mNearPlane, mFarPlane);
	buf->Camera.ViewMatrix = glm::lookAt(mPosition, mPosition + mDirection, mUp);
	buf->Camera.SkyMatrix = glm::lookAt(glm::vec3(0.0f), mDirection, mUp);
	buf->Camera.Position = mPosition;
	buf->Camera.Direction = mDirection;
	buf->Camera.NearPlane = mNearPlane;
	buf->Camera.FarPlane = mFarPlane;
	buf->Camera.Focus = mFocus;
}

// ------------------------------------------------------------------------------------------------
ViewFrustum& Camera::GetVolume()
{
	if (mDirty)
	{
		mFrustum.SetPosition(mPosition, mDirection, mUp);
		mFrustum.SetPerspective(mFOV, mAspect, mNearPlane, mFarPlane);
		mDirty = false;
	}

	return mFrustum;
}
