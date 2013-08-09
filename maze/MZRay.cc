// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
bool Ray::Inside(const BoundingBox& box) const
{
	if (box.mDirty) { box.Compute(); }

	for (size_t i = 0; i < 6; ++i)
	{
		__m128 dot, dist, point, mask;

		dot = _mm_mul_ps(box.mPlanes[i], mDirection);
		dot = _mm_hadd_ps(dot, dot);
		dot = _mm_hadd_ps(dot, dot);
		if (_mm_movemask_ps(_mm_cmpge_ps(dot, _mm_setzero_ps())) == 0xF)
		{
			continue;
		}

		dist = _mm_mul_ps(box.mPlanes[i], mOrigin);
		dist = _mm_hadd_ps(dist, dist);
		dist = _mm_hadd_ps(dist, dist);
		if (_mm_movemask_ps(_mm_cmplt_ps(dist, _mm_setzero_ps())) == 0xF)
		{
			continue;
		}

		point = _mm_sub_ps(mOrigin, _mm_mul_ps(mDirection, _mm_div_ps(dist, dot)));
		mask = _mm_and_ps(_mm_cmple_ps(box.mMin, point), _mm_cmple_ps(point, box.mMax));
		if ((_mm_movemask_ps(mask) & 7) == 7)
		{
			return true;
		}
	}

	return false;
}

// ------------------------------------------------------------------------------------------------
bool Ray::Outside(const BoundingBox& box) const
{
	if (box.mDirty) { box.Compute(); }
	
	for (size_t i = 0; i < 6; ++i)
	{
		__m128 dot, dist, point, mask;

		dot = _mm_mul_ps(box.mPlanes[i], mDirection);
		dot = _mm_hadd_ps(dot, dot);
		dot = _mm_hadd_ps(dot, dot);
		if (_mm_movemask_ps(_mm_cmpeq_ps(dot, _mm_setzero_ps())) == 0xF)
		{
			continue;
		}

		dist = _mm_mul_ps(box.mPlanes[i], mOrigin);
		dist = _mm_hadd_ps(dist, dist);
		dist = _mm_hadd_ps(dist, dist);
		if (_mm_movemask_ps(_mm_cmpeq_ps(dist, _mm_setzero_ps())) == 0xF)
		{
			continue;
		}
		
		point = _mm_sub_ps(mOrigin, _mm_mul_ps(mDirection, _mm_div_ps(dist, dot)));
		mask = _mm_and_ps(_mm_cmple_ps(box.mMin, point), _mm_cmple_ps(point, box.mMax));
		if ((_mm_movemask_ps(mask) & 7) == 7)
		{
			return false;
		}
	}

	return true;
}
		
// ------------------------------------------------------------------------------------------------
__m128 Ray::Distance(const BoundingBox& box) const
{
	__m128 r;

	if (box.mDirty) { box.Compute(); }

	r = _mm_set_ps1(FLT_MAX);
	for (size_t i = 0; i < 6; ++i)
	{
		__m128 dot, dist, point, mask;

		dot = _mm_mul_ps(box.mPlanes[i], mDirection);
		dot = _mm_hadd_ps(dot, dot);
		dot = _mm_hadd_ps(dot, dot);
		if (_mm_movemask_ps(_mm_cmpge_ps(dot, _mm_setzero_ps())) == 0xF)
		{
			continue;
		}

		dist = _mm_mul_ps(box.mPlanes[i], mOrigin);
		dist = _mm_hadd_ps(dist, dist);
		dist = _mm_hadd_ps(dist, dist);
		if (_mm_movemask_ps(_mm_cmplt_ss(dist, _mm_setzero_ps())) == 0xF)
		{
			continue;
		}
		
		point = _mm_sub_ps(mOrigin, _mm_mul_ps(mDirection, _mm_div_ps(dist, dot)));
		mask = _mm_and_ps(_mm_cmple_ps(box.mMin, point), _mm_cmple_ps(point, box.mMax));
		if ((_mm_movemask_ps(mask) & 7) == 7)
		{
			r = _mm_min_ps(r, _mm_sub_ps(_mm_setzero_ps(), _mm_div_ps(dist, dot)));
		}
	}

	return r;
}
