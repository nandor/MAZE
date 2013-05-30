// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZRay.h"
#include "MZSphere.h"
#include "MZFrustum.h"
#include "MZException.h"
#include "MZBoundingBox.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
bool Ray::Inside(const Ray& ray) const
{
    throw std::runtime_error("Not implemented: Ray::Inside(Ray)");
}

// ------------------------------------------------------------------------------------------------
bool Ray::Inside(const Sphere& sphere) const
{
	return glm::length(glm::cross(mDirection, sphere.mCenter - mOrigin)) < sphere.mRadius;
}

// ------------------------------------------------------------------------------------------------
bool Ray::Inside(const BoundingBox& box) const
{
	if (box.mDirty) { box.Compute(); }

	for (size_t i = 0; i < 6; ++i)
	{
		float dot, dist;
		glm::vec3 point;
		glm::vec4& plane = box.mPlanes[i];

		dot = plane.x * mDirection.x + plane.y * mDirection.y + plane.z * mDirection.z;
		if (dot >= 0.0f)
		{
			continue;
		}

		dist = plane.x * mOrigin.x + plane.y * mOrigin.y + plane.z * mOrigin.z + plane.w;
		if (dist < 0.0f)
		{
			continue;
		}
		
		point = mOrigin - dist / dot * mDirection;
		if (box.mMin.x <= point.x && point.x <= box.mMax.x &&
			box.mMin.y <= point.y && point.y <= box.mMax.y &&
			box.mMin.z <= point.z && point.z <= box.mMax.z)
		{
			return true;
		}
	}

	return false;
}

// ------------------------------------------------------------------------------------------------
bool Ray::Inside(const Frustum& ray) const
{
    throw std::runtime_error("Not implemented: Ray::Inside(Frustum)");
}

// ------------------------------------------------------------------------------------------------
bool Ray::Outside(const Ray& ray) const
{
    throw std::runtime_error("Not implemented: Ray::Outside(Ray)");
}

// ------------------------------------------------------------------------------------------------
bool Ray::Outside(const Sphere& sphere) const
{
	return glm::length(glm::cross(mDirection, sphere.mCenter - mOrigin)) > sphere.mRadius;
}

// ------------------------------------------------------------------------------------------------
bool Ray::Outside(const BoundingBox& box) const
{
	if (box.mDirty) { box.Compute(); }
	
	for (size_t i = 0; i < 6; ++i)
	{
		float dot, dist;
		glm::vec3 point;
		glm::vec4& plane = box.mPlanes[i];

		dot = plane.x * mDirection.x + plane.y * mDirection.y + plane.z * mDirection.z;
		if (dot == 0.0f)
		{
			continue;
		}

		dist = plane.x * mOrigin.x + plane.y * mOrigin.y + plane.z * mOrigin.z + plane.w;
		if (dist == 0.0f)
		{
			continue;
		}
		
		point = mOrigin - dist / dot * mDirection;
		if (box.mMin.x <= point.x && point.x <= box.mMax.x &&
			box.mMin.y <= point.y && point.y <= box.mMax.y &&
			box.mMin.z <= point.z && point.z <= box.mMax.z)
		{
			return false;
		}
	}

	return true;
}

// ------------------------------------------------------------------------------------------------
bool Ray::Outside(const Frustum& ray) const
{
    throw std::runtime_error("Not implemented: Ray::Outside(Frustum)");
}

// ------------------------------------------------------------------------------------------------
float Ray::Distance(const Ray& ray) const
{
    throw std::runtime_error("Not implemented: Ray::Distance(Ray)");
}

// ------------------------------------------------------------------------------------------------
float Ray::Distance(const Sphere& sphere) const
{
	return glm::length(glm::cross(mDirection, sphere.mCenter - mOrigin));
}
		
// ------------------------------------------------------------------------------------------------
float Ray::Distance(const BoundingBox& box) const
{
	float r;
	if (box.mDirty) { box.Compute(); }

	r = std::numeric_limits<float>::max();
	for (size_t i = 0; i < 6; ++i)
	{
		float dot, dist;
		glm::vec3 point;
		glm::vec4& plane = box.mPlanes[i];

		dot = plane.x * mDirection.x + plane.y * mDirection.y + plane.z * mDirection.z;
		if (dot >= 0.0f)
		{
			continue;
		}

		dist = plane.x * mOrigin.x + plane.y * mOrigin.y + plane.z * mOrigin.z + plane.w;
		if (dist < 0.0f)
		{
			continue;
		}

		point = mOrigin - dist / dot * mDirection;
		if (box.mMin.x <= point.x && point.x <= box.mMax.x &&
			box.mMin.y <= point.y && point.y <= box.mMax.y &&
			box.mMin.z <= point.z && point.z <= box.mMax.z)
		{
			r = std::min(r, -dist / dot);
		}
	}

	return r;
}

// ------------------------------------------------------------------------------------------------
float Ray::Distance(const Frustum& frustum) const
{
    throw std::runtime_error("Not implemented: Ray::Distance(Frustum)");
}
