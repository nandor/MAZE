// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZRay.h"
#include "MZFrustum.h"
#include "MZException.h"
#include "MZBoundingBox.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
BoundingBox::BoundingBox()
{
	mMin = glm::vec3(0.0f);
	mMax = glm::vec3(1.0f);
	mDirty = true;
}

// ------------------------------------------------------------------------------------------------
BoundingBox::BoundingBox(const glm::vec3& pos, const glm::vec3& size)
{
	mMin = pos;
	mMax = pos + size;
	mDirty = true;
}

// ------------------------------------------------------------------------------------------------
BoundingBox::BoundingBox(float px, float py, float pz, float w, float h, float d)
{
	mMin.x = px;
	mMin.y = py;
	mMin.z = pz;
	mMax.x = px + w;
	mMax.y = py + h;
	mMax.z = pz + d;
	mDirty = true;
}

// ------------------------------------------------------------------------------------------------   
BoundingBox BoundingBox::Move(const glm::vec3& pos)
{
	BoundingBox box;

	box.mMin = pos + mMin;
	box.mMax = pos + mMax;
	box.mDirty = true;

	return box;
}

// ------------------------------------------------------------------------------------------------
BoundingBox BoundingBox::Extend(const glm::vec3& dir)
{
	BoundingBox box;

	box.mMin.x = mMin.x + (dir.x < 0.0f ? dir.x : 0.0f);
	box.mMin.y = mMin.y + (dir.y < 0.0f ? dir.y : 0.0f);
	box.mMin.z = mMin.z + (dir.z < 0.0f ? dir.z : 0.0f);
	
	box.mMax.x = mMax.x + (dir.x > 0.0f ? dir.x : 0.0f);
	box.mMax.y = mMax.y + (dir.y > 0.0f ? dir.y : 0.0f);
	box.mMax.z = mMax.z + (dir.z > 0.0f ? dir.z : 0.0f);

	box.mDirty = true;

	return box;
}

// ------------------------------------------------------------------------------------------------
void BoundingBox::Compute() const
{	
	mCorners[0]	= glm::vec3(mMin.x, mMax.y, mMin.z);
	mCorners[1]	= glm::vec3(mMax.x, mMax.y, mMin.z);
	mCorners[2]	= glm::vec3(mMin.x, mMin.y, mMin.z);
	mCorners[3] = glm::vec3(mMax.x, mMin.y, mMin.z);
	mCorners[4]	= glm::vec3(mMin.x, mMax.y, mMax.z);
	mCorners[5]	= glm::vec3(mMax.x, mMax.y, mMax.z);
	mCorners[6]	= glm::vec3(mMin.x, mMin.y, mMax.z);
	mCorners[7]	= glm::vec3(mMax.x, mMin.y, mMax.z);
	
	mPlanes[0] = glm::vec4( 1.0f,  0.0f,  0.0f, -mMax.x);
	mPlanes[1] = glm::vec4(-1.0f,  0.0f,  0.0f,  mMin.x);
	mPlanes[2] = glm::vec4( 0.0f,  1.0f,  0.0f, -mMax.y);
	mPlanes[3] = glm::vec4( 0.0f, -1.0f,  0.0f,  mMin.y);
	mPlanes[4] = glm::vec4( 0.0f,  0.0f,  1.0f, -mMax.z);
	mPlanes[5] = glm::vec4( 0.0f,  0.0f, -1.0f,  mMin.z);

	mDirty = false;
}

// ------------------------------------------------------------------------------------------------
bool BoundingBox::Inside(const Ray& ray) const
{
	return ray.Inside(*this);
}

// ------------------------------------------------------------------------------------------------
bool BoundingBox::Inside(const Sphere& ray) const
{
    throw std::runtime_error("Not implemented: BoundingBox::Inside(Sphere)");
}

// ------------------------------------------------------------------------------------------------
bool BoundingBox::Inside(const glm::vec3& point) const
{
	return (mMin.x < point.x && point.x < mMax.x &&
			mMin.y < point.y && point.y < mMax.y && 
			mMin.z < point.z && point.z < mMax.z);
}

// ------------------------------------------------------------------------------------------------
bool BoundingBox::Inside(const BoundingBox& box) const
{
	return (mMin.x < box.mMin.x && box.mMax.x < mMax.x &&
			mMin.y < box.mMin.y && box.mMax.y < mMax.y && 
			mMin.z < box.mMin.z && box.mMax.z < mMax.z);
}

// ------------------------------------------------------------------------------------------------
bool BoundingBox::Inside(const Frustum& ray) const
{
    throw std::runtime_error("Not implemented: BoundingBox::Inside(Frustum)");
}

// ------------------------------------------------------------------------------------------------
bool BoundingBox::Outside(const Ray& ray) const
{
	return ray.Outside(*this);
}

// ------------------------------------------------------------------------------------------------
bool BoundingBox::Outside(const Sphere& ray) const
{
    throw std::runtime_error("Not implemented: BoundingBox::Outside(Sphere)");
}

// ------------------------------------------------------------------------------------------------
bool BoundingBox::Outside(const glm::vec3& point) const
{
	return (point.x < mMin.x || mMax.x < point.x ||
			point.y < mMin.y || mMax.y < point.y ||
			point.z < mMin.z || mMax.z < point.z);
}

// ------------------------------------------------------------------------------------------------
bool BoundingBox::Outside(const BoundingBox& box) const
{
	return (box.mMax.x < mMin.x || mMax.x < box.mMin.x ||
			box.mMax.y < mMin.y || mMax.y < box.mMin.y ||
			box.mMax.z < mMin.z || mMax.z < box.mMin.z);
}

// ------------------------------------------------------------------------------------------------
bool BoundingBox::Outside(const Frustum& f) const
{
	if (mDirty) { Compute(); }
	if (f.mDirty) { f.Compute(); }

	for (size_t i = 0; i < 6; ++i)
	{
		size_t in = 0, out = 0;

		for (size_t j = 0; j < 8; ++j)
		{
			float dot = f.mPlanes[i].x * mCorners[j].x + 
						f.mPlanes[i].y * mCorners[j].y +
						f.mPlanes[i].z * mCorners[j].z +
						f.mPlanes[i].w;

			if (dot < 0.0f)
			{
				++in;
			}
			else
			{
				++out;
			}
		}

		if (in == 0)
		{
			return true;
		}
	}

	return false;
}

// ------------------------------------------------------------------------------------------------
float BoundingBox::Distance(const Ray& ray) const
{
	return ray.Distance(*this);
}

// ------------------------------------------------------------------------------------------------
float BoundingBox::Distance(const Sphere& ray) const
{
    throw std::runtime_error("Not implemented: BoundingBox::Distance(Sphere)");
}

// ------------------------------------------------------------------------------------------------
float BoundingBox::Distance(const BoundingBox& ray) const
{
    throw std::runtime_error("Not implemented: BoundingBox::Distance(BoundingBox)");
}

// ------------------------------------------------------------------------------------------------
float BoundingBox::Distance(const Frustum& ray) const
{
    throw std::runtime_error("Not implemented: BoundingBox::Distance(Frustum)");
}

