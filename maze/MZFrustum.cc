// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
Frustum::Frustum()
{
	mPosition = glm::vec3(0.0f);
	mDirection = glm::vec3(0.0f, 0.0f, 1.0f);
	mUp = glm::vec3(0.0f, 1.0f, 0.0f);
	
	mIsOrtho = true;
	mWidth = 1.0f;
	mHeight = 1.0f;
	mNearPlane = -1.0f;
	mFarPlane = 1.0f;

	mDirty = true;
}
    
// ------------------------------------------------------------------------------------------------
Frustum::Frustum(const glm::vec3& p, const glm::vec3& d, const glm::vec3& u,
				 float l, float r, float t, float b, float n, float f)
{
	mPosition = p;
	mDirection = d;
	mUp = u;
	
	mIsOrtho = true;
	mWidth = r - l;
	mHeight = t - b;
	mNearPlane = n;
	mFarPlane = f;

	mDirty = true;
}
	
// ------------------------------------------------------------------------------------------------
Frustum::Frustum(const glm::vec3& p, const glm::vec3& d, const glm::vec3& u,
				 float fov, float a, float n, float f)
{
	mPosition = p;
	mDirection = d;
	mUp = u;

	mIsOrtho = false;
	mFOV = fov;
	mWidth = a;
	mHeight = 1.0f;
	mNearPlane = n;
	mFarPlane = f;

	mDirty = true;
}
// ------------------------------------------------------------------------------------------------
void Frustum::SetPerspective(float fov, float a, float n, float f)
{
	mIsOrtho = false;
	mFOV = fov;
	mWidth = a;
	mHeight = 1.0f;
	mNearPlane = n;
	mFarPlane = f;

	mDirty = true;
}

// ------------------------------------------------------------------------------------------------
void Frustum::SetOrtho(float l, float r, float t, float b, float n, float f)
{	
	mIsOrtho = true;
	mWidth = r - l;
	mHeight = t - b;
	mNearPlane = n;
	mFarPlane = f;

	mDirty = true;
}
	
// ------------------------------------------------------------------------------------------------
Frustum Frustum::Slice(float np, float fp)
{
	Frustum f;

	if (mDirty) { Compute(); }

	np = (np - mNearPlane) / (mFarPlane - mNearPlane);
	fp = (fp - mNearPlane) / (mFarPlane - mNearPlane);

	f.mIsOrtho	 = mIsOrtho;
	f.mNearPlane = np;
	f.mFarPlane	 = fp;
	f.mUp		 = mUp;
	f.mDirection = mDirection;
	f.mPosition	 = mPosition;
	f.mDirty	 = false;

	__m128 tl = _mm_sub_ps(mCorners[4], mCorners[0]);
	__m128 tr = _mm_sub_ps(mCorners[5], mCorners[1]);
	__m128 bl = _mm_sub_ps(mCorners[6], mCorners[2]);
	__m128 br = _mm_sub_ps(mCorners[7], mCorners[3]);

	f.mCorners[0] = _mm_add_ps(mCorners[0], _mm_mul_ps(tl, _mm_set_ps1(np)));
	f.mCorners[1] = _mm_add_ps(mCorners[1], _mm_mul_ps(tr, _mm_set_ps1(np)));
	f.mCorners[2] = _mm_add_ps(mCorners[2], _mm_mul_ps(bl, _mm_set_ps1(np)));
	f.mCorners[3] = _mm_add_ps(mCorners[3], _mm_mul_ps(br, _mm_set_ps1(np)));
	f.mCorners[4] = _mm_add_ps(mCorners[0], _mm_mul_ps(tl, _mm_set_ps1(fp)));
	f.mCorners[5] = _mm_add_ps(mCorners[1], _mm_mul_ps(tr, _mm_set_ps1(fp)));
	f.mCorners[6] = _mm_add_ps(mCorners[2], _mm_mul_ps(bl, _mm_set_ps1(fp)));
	f.mCorners[7] = _mm_add_ps(mCorners[3], _mm_mul_ps(br, _mm_set_ps1(fp)));

	return f;
}

// ------------------------------------------------------------------------------------------------
Frustum Frustum::GetLightVolume(const glm::vec3& lightDir)
{
	Frustum v;

	glm::vec3 z = glm::normalize(lightDir);
	glm::vec3 x = glm::normalize(glm::cross(mUp, z));
	glm::vec3 y = glm::cross(z, x);

	__m128 normal[] = 
	{
		_mm_setr_ps( x.x,  x.y,  x.z,  0.0f),
		_mm_setr_ps( y.x,  y.y,  y.z,  0.0f),
		_mm_setr_ps( z.x,  z.y,  z.z,  0.0f),
		_mm_setr_ps(-x.x, -x.y, -x.z,  0.0f),
		_mm_setr_ps(-y.x, -y.y, -y.z,  0.0f),
		_mm_setr_ps(-z.x, -z.y, -z.z,  0.0f)
	};
		
	for (size_t i = 0; i < 6; ++i)
	{
		for (size_t j = 0; j < 8; ++j)
		{
			__m128 d, c;
			
			d = _mm_mul_ps(normal[i], _mm_add_ps(mCorners[j], _mm_mul_ps(normal[i], _mm_set_ps1(7.0f))));
			d = _mm_hadd_ps(d, d);
			d = _mm_hadd_ps(d, d);
			d = Invert(d);

			bool good = true;
			for (size_t k = 0; k < 8; ++k)
			{
				if (j != k)
				{					
					c = _mm_mul_ps(normal[i], _mm_add_ps(mCorners[k], _mm_mul_ps(normal[i], _mm_set_ps1(7.0f))));
					c = _mm_hadd_ps(c, c);
					c = _mm_hadd_ps(c, c);
					c = _mm_add_ps(c, d);

					if (_mm_movemask_ps(_mm_cmpgt_ps(c, _mm_setzero_ps())) == 0xF)
					{
						good = false;
						break;
					}
				}
			}

			if (good)
			{
				v.mPlanes[i] = normal[i];
				_mm_store_ss((float*)&v.mPlanes[i] + 3, d);
				break;
			}
		}
	}		
	
	v.mCorners[0] = ::Intersect(v.mPlanes[5], v.mPlanes[1],	v.mPlanes[3]);
	v.mCorners[1] = ::Intersect(v.mPlanes[5], v.mPlanes[1],	v.mPlanes[0]);
	v.mCorners[2] = ::Intersect(v.mPlanes[5], v.mPlanes[4], v.mPlanes[3]);
	v.mCorners[3] = ::Intersect(v.mPlanes[5], v.mPlanes[4], v.mPlanes[0]);
	v.mCorners[4] = ::Intersect(v.mPlanes[2], v.mPlanes[1], v.mPlanes[3]);
	v.mCorners[5] = ::Intersect(v.mPlanes[2], v.mPlanes[1], v.mPlanes[0]);
	v.mCorners[6] = ::Intersect(v.mPlanes[2], v.mPlanes[4], v.mPlanes[3]);
	v.mCorners[7] = ::Intersect(v.mPlanes[2], v.mPlanes[4], v.mPlanes[0]);
	
	glm::vec3 v0, v1, v2, v3, v4;

	v0 = glm::vec3(((float*)&v.mCorners[0])[0], ((float*)&v.mCorners[0])[1], ((float*)&v.mCorners[0])[2]);
	v1 = glm::vec3(((float*)&v.mCorners[1])[0], ((float*)&v.mCorners[1])[1], ((float*)&v.mCorners[1])[2]);
	v2 = glm::vec3(((float*)&v.mCorners[2])[0], ((float*)&v.mCorners[2])[1], ((float*)&v.mCorners[2])[2]);
	v3 = glm::vec3(((float*)&v.mCorners[3])[0], ((float*)&v.mCorners[3])[1], ((float*)&v.mCorners[3])[2]);
	v4 = glm::vec3(((float*)&v.mCorners[4])[0], ((float*)&v.mCorners[4])[1], ((float*)&v.mCorners[4])[2]);

	v.mPosition = (v0 + v1 + v2 + v3) / 4.0f;
	v.mDirection = z;
	v.mUp = mUp;

	float width = glm::length(v0 - v1) / 2.0f;
	float height = glm::length(v0 - v2) / 2.0f;

	v.mNearPlane = 0.0f;
	v.mFarPlane = glm::length(v0 - v4);
	v.mProjMat = glm::ortho(-width, width, -height, height, v.mNearPlane, v.mFarPlane);
	v.mViewMat = glm::lookAt(v.mPosition, v.mPosition + v.mDirection, v.mUp);
	v.mDirty = false;

	return v;
}

// ------------------------------------------------------------------------------------------------
void Frustum::Compute() const
{
	glm::vec3 x, y, z;

	z = glm::normalize(mDirection);
	x = glm::normalize(glm::cross(mUp, z));
	y = glm::cross(z, x);

	mViewMat = glm::lookAt(mPosition, mPosition + mDirection, mUp);

	if (mIsOrtho)
	{
		glm::vec3 nc, fc;
		__m128 dot;
		
		nc = mPosition + z * mNearPlane;
		fc = mPosition + z * mFarPlane;

		mProjMat = glm::ortho(0.0f, mWidth, mHeight, 0.0f);
		
		mCorners[0]	= _mm_loadu_ps(glm::value_ptr(glm::vec4(nc + y * mHeight * 0.5f - x * mWidth * 0.5f, 1.0f)));
		mCorners[1]	= _mm_loadu_ps(glm::value_ptr(glm::vec4(nc + y * mHeight * 0.5f + x * mWidth * 0.5f, 1.0f)));
		mCorners[2]	= _mm_loadu_ps(glm::value_ptr(glm::vec4(nc - y * mHeight * 0.5f - x * mWidth * 0.5f, 1.0f)));
		mCorners[3]	= _mm_loadu_ps(glm::value_ptr(glm::vec4(nc - y * mHeight * 0.5f + x * mWidth * 0.5f, 1.0f)));
		mCorners[4]	= _mm_loadu_ps(glm::value_ptr(glm::vec4(fc + y * mHeight * 0.5f - x * mWidth * 0.5f, 1.0f)));
		mCorners[5]	= _mm_loadu_ps(glm::value_ptr(glm::vec4(fc + y * mHeight * 0.5f + x * mWidth * 0.5f, 1.0f)));
		mCorners[6]	= _mm_loadu_ps(glm::value_ptr(glm::vec4(fc - y * mHeight * 0.5f - x * mWidth * 0.5f, 1.0f)));
		mCorners[7]	= _mm_loadu_ps(glm::value_ptr(glm::vec4(fc - y * mHeight * 0.5f + x * mWidth * 0.5f, 1.0f)));
			
		mPlanes[0] = _mm_setr_ps(x.x, x.y, x.z, 0.0f);
		dot = _mm_mul_ps(mPlanes[0], mCorners[5]);
		dot = _mm_mul_ps(dot, dot);
		dot = _mm_mul_ps(dot, dot);
		_mm_store_ss((float*)&mPlanes[0] + 3, dot);
				
		mPlanes[1] = _mm_setr_ps(y.x, y.y, y.z, 0.0f);
		dot = _mm_mul_ps(mPlanes[1], mCorners[5]);
		dot = _mm_mul_ps(dot, dot);
		dot = _mm_mul_ps(dot, dot);
		_mm_store_ss((float*)&mPlanes[1] + 3, dot);
		
		mPlanes[2] = _mm_setr_ps(z.x, z.y, z.z, 0.0f);
		dot = _mm_mul_ps(mPlanes[2], mCorners[5]);
		dot = _mm_mul_ps(dot, dot);
		dot = _mm_mul_ps(dot, dot);
		_mm_store_ss((float*)&mPlanes[2] + 3, dot);
		
		mPlanes[3] = _mm_setr_ps(-x.x, -x.y, -x.z, 0.0f);
		dot = _mm_mul_ps(mPlanes[3], mCorners[2]);
		dot = _mm_mul_ps(dot, dot);
		dot = _mm_mul_ps(dot, dot);
		_mm_store_ss((float*)&mPlanes[3] + 3, dot);
		
		mPlanes[4] = _mm_setr_ps(-y.x, -y.y, -y.z, 0.0f);
		dot = _mm_mul_ps(mPlanes[4], mCorners[2]);
		dot = _mm_mul_ps(dot, dot);
		dot = _mm_mul_ps(dot, dot);
		_mm_store_ss((float*)&mPlanes[4] + 3, dot);

		mPlanes[5] = _mm_setr_ps(-z.x, -z.y, -z.z, 0.0f);
		dot = _mm_mul_ps(mPlanes[5], mCorners[2]);
		dot = _mm_mul_ps(dot, dot);
		dot = _mm_mul_ps(dot, dot);
		_mm_store_ss((float*)&mPlanes[5] + 3, dot);
	}
	else
	{
		float tg, nh, fh, nw, fw;
		glm::vec3 nc, fc;
		const float* m;

		mProjMat = glm::perspective(mFOV, mWidth / mHeight, mNearPlane, mFarPlane);
		m = glm::value_ptr(mProjMat * mViewMat);

		nc = mPosition + z * mNearPlane;
		fc = mPosition + z * mFarPlane;
		
		tg = tan(mFOV * PI / 360.0f);
		nh = mNearPlane * tg;
		nw = nh * mWidth / mHeight;
		fh = mFarPlane * tg;
		fw = fh * mWidth / mHeight;
			
		mCorners[0] = _mm_loadu_ps(glm::value_ptr(glm::vec4(nc + y * nh - x * nw, 1.0f)));
		mCorners[1]	= _mm_loadu_ps(glm::value_ptr(glm::vec4(nc + y * nh + x * nw, 1.0f)));
		mCorners[2]	= _mm_loadu_ps(glm::value_ptr(glm::vec4(nc - y * nh - x * nw, 1.0f)));
		mCorners[3]	= _mm_loadu_ps(glm::value_ptr(glm::vec4(nc - y * nh + x * nw, 1.0f)));
		mCorners[4]	= _mm_loadu_ps(glm::value_ptr(glm::vec4(fc + y * fh - x * fw, 1.0f)));
		mCorners[5]	= _mm_loadu_ps(glm::value_ptr(glm::vec4(fc + y * fh + x * fw, 1.0f)));
		mCorners[6]	= _mm_loadu_ps(glm::value_ptr(glm::vec4(fc - y * fh - x * fw, 1.0f)));
		mCorners[7]	= _mm_loadu_ps(glm::value_ptr(glm::vec4(fc - y * fh + x * fw, 1.0f)));

		mPlanes[0] = _mm_setr_ps(-m[3] - m[2], -m[7] - m[6], -m[11] - m[10], -m[15] - m[14]);
		mPlanes[1] = _mm_setr_ps(-m[3] + m[2], -m[7] + m[6], -m[11] + m[10], -m[15] + m[14]);
		mPlanes[2] = _mm_setr_ps(-m[3] + m[1], -m[7] + m[5], -m[11] + m[9],  -m[15] + m[13]);
		mPlanes[3] = _mm_setr_ps(-m[3] - m[1], -m[7] - m[5], -m[11] - m[9],  -m[15] - m[13]);
		mPlanes[4] = _mm_setr_ps(-m[3] + m[0], -m[7] + m[4], -m[11] + m[8],  -m[15] + m[12]);
		mPlanes[5] = _mm_setr_ps(-m[3] - m[0], -m[7] - m[4], -m[11] - m[8],  -m[15] - m[12]);
	}

	mDirty = false;
}

// ------------------------------------------------------------------------------------------------
bool Frustum::Inside(const BoundingBox& box) const
{
	if (mDirty) { Compute(); }
	if (box.mDirty) { box.Compute(); }

	for (size_t i = 0; i < 6; ++i)
	{
		size_t in = 0, out = 0;

		for (size_t j = 0; j < 8; ++j)
		{
			__m128 dot;

			dot = _mm_mul_ps(mPlanes[i], box.mCorners[j]);
			dot = _mm_hadd_ps(dot, dot);
			dot = _mm_hadd_ps(dot, dot);

			if (_mm_movemask_ps(_mm_cmple_ps(dot, _mm_setzero_ps())) == 0xF)
			{
				++in;
				if (out)
				{
					return false;
				}
			}
			else
			{
				++out;
				if (in || out == 8)
				{
					return false;
				}
			}
		}
	}

	return true;
}

// ------------------------------------------------------------------------------------------------
bool Frustum::Outside(const BoundingBox& box) const
{
	if (mDirty) { Compute(); }
	if (box.mDirty) { box.Compute(); }

	for (size_t i = 0; i < 6; ++i)
	{
		bool outside = true;
		for (size_t j = 0; j < 8 && outside; ++j)
		{
			__m128 dot;

			dot = _mm_mul_ps(mPlanes[i], box.mCorners[j]);
			dot = _mm_hadd_ps(dot, dot);
			dot = _mm_hadd_ps(dot, dot);
			
			if (_mm_movemask_ps(_mm_cmple_ps(dot, _mm_setzero_ps())) == 0xF)
			{
				outside = false;
			}
		}

		if (outside)
		{
			return true;
		}
	}

	return false;
}
