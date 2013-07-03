// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------   
BoundingBox BoundingBox::Move(const glm::vec3& p)
{
	BoundingBox box;
	
	box.mMin = _mm_add_ps(mMin, _mm_setr_ps(p.x, p.y, p.z, 0.0f));
	box.mMax = _mm_add_ps(mMax, _mm_setr_ps(p.x, p.y, p.z, 0.0f));
	box.mDirty = true;

	return box;
}

// ------------------------------------------------------------------------------------------------   
BoundingBox BoundingBox::Move(const __m128& p)
{
	BoundingBox box;
	
	box.mMin = _mm_add_ps(mMin, p);
	box.mMax = _mm_add_ps(mMax, p);
	box.mDirty = true;

	return box;
}

// ------------------------------------------------------------------------------------------------
BoundingBox BoundingBox::Extend(const glm::vec3& d)
{
	BoundingBox box;

	box.mMin = _mm_add_ps(mMin, _mm_min_ps(_mm_setr_ps(d.x, d.y, d.z, 0.0f), _mm_setzero_ps()));
	box.mMax = _mm_add_ps(mMax, _mm_max_ps(_mm_setr_ps(d.x, d.y, d.z, 0.0f), _mm_setzero_ps()));
	box.mDirty = true;

	return box;
}

// ------------------------------------------------------------------------------------------------
BoundingBox BoundingBox::Extend(const __m128& d)
{
	BoundingBox box;

	box.mMin = _mm_add_ps(mMin, _mm_min_ps(d, _mm_setzero_ps()));
	box.mMax = _mm_add_ps(mMax, _mm_max_ps(d, _mm_setzero_ps()));
	box.mDirty = true;

	return box;
}

// ------------------------------------------------------------------------------------------------
void BoundingBox::Compute() const
{	
	mPlanes[0] = _mm_setr_ps( 1.0f,  0.0f,  0.0f, -((float*)&mMax)[0]);
	mPlanes[1] = _mm_setr_ps( 0.0f,  1.0f,  0.0f, -((float*)&mMax)[1]);
	mPlanes[2] = _mm_setr_ps( 0.0f,  0.0f,  1.0f, -((float*)&mMax)[2]);
	mPlanes[3] = _mm_setr_ps(-1.0f,  0.0f,  0.0f,  ((float*)&mMin)[0]);
	mPlanes[4] = _mm_setr_ps( 0.0f, -1.0f,  0.0f,  ((float*)&mMin)[1]);
	mPlanes[5] = _mm_setr_ps( 0.0f,  0.0f, -1.0f,  ((float*)&mMin)[2]);

	mCorners[0]	= _mm_setr_ps(((float*)&mMin)[0], ((float*)&mMax)[1], ((float*)&mMin)[2], 1.0f);
	mCorners[1]	= _mm_setr_ps(((float*)&mMax)[0], ((float*)&mMax)[1], ((float*)&mMin)[2], 1.0f);
	mCorners[2]	= _mm_setr_ps(((float*)&mMin)[0], ((float*)&mMin)[1], ((float*)&mMin)[2], 1.0f);
	mCorners[3] = _mm_setr_ps(((float*)&mMax)[0], ((float*)&mMin)[1], ((float*)&mMin)[2], 1.0f);
	mCorners[4]	= _mm_setr_ps(((float*)&mMin)[0], ((float*)&mMax)[1], ((float*)&mMax)[2], 1.0f);
	mCorners[5]	= _mm_setr_ps(((float*)&mMax)[0], ((float*)&mMax)[1], ((float*)&mMax)[2], 1.0f);
	mCorners[6]	= _mm_setr_ps(((float*)&mMin)[0], ((float*)&mMin)[1], ((float*)&mMax)[2], 1.0f);
	mCorners[7]	= _mm_setr_ps(((float*)&mMax)[0], ((float*)&mMin)[1], ((float*)&mMax)[2], 1.0f);

	mDirty = false;
}

// ------------------------------------------------------------------------------------------------
bool BoundingBox::Inside(const Ray& ray) const
{
	return ray.Inside(*this);
}

// ------------------------------------------------------------------------------------------------
bool BoundingBox::Inside(const BoundingBox& box) const
{
	__m128 a = _mm_cmplt_ps(mMin, box.mMin);
	__m128 b = _mm_cmplt_ps(box.mMax, mMax);
	
	return (_mm_movemask_ps(_mm_and_ps(a, b)) & 7) == 7;
}

// ------------------------------------------------------------------------------------------------
bool BoundingBox::Inside(const Frustum& frustum) const
{
	return frustum.Inside(*this);
}

// ------------------------------------------------------------------------------------------------
bool BoundingBox::Outside(const Ray& ray) const
{
	return ray.Outside(*this);
}

// ------------------------------------------------------------------------------------------------
bool BoundingBox::Outside(const BoundingBox& box) const
{
	__m128 a = _mm_cmplt_ps(box.mMax, mMin);
	__m128 b = _mm_cmplt_ps(mMax, box.mMin);

	return (_mm_movemask_ps(_mm_or_ps(a, b)) & 7) != 0;
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
			__m128 dot;
			
			dot = _mm_mul_ps(f.mPlanes[i], mCorners[j]);
			dot = _mm_hadd_ps(dot, dot);
			dot = _mm_hadd_ps(dot, dot);

			if ((_mm_movemask_ps(_mm_cmplt_ps(dot, _mm_setzero_ps())) & 7) == 7)
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
__m128 BoundingBox::Distance(const Ray& ray) const
{
	return ray.Distance(*this);
}
