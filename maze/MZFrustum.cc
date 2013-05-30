// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZMath.h"
#include "MZFrustum.h"
#include "MZException.h"
#include "MZBoundingBox.h"
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

	glm::vec3 tl = mCorners[4] - mCorners[0];
	glm::vec3 tr = mCorners[5] - mCorners[1];
	glm::vec3 bl = mCorners[6] - mCorners[2];
	glm::vec3 br = mCorners[7] - mCorners[3];

	f.mIsOrtho	 = mIsOrtho;
	f.mNearPlane = np;
	f.mFarPlane	 = fp;
	f.mUp		 = mUp;
	f.mDirection = mDirection;
	f.mPosition	 = mPosition;
	f.mDirty	 = false;

	np = (np - mNearPlane) / (mFarPlane - mNearPlane);
	fp = (fp - mNearPlane) / (mFarPlane - mNearPlane);

	f.mCorners[0] = mCorners[0] + tl * np;
	f.mCorners[1] = mCorners[1] + tr * np;
	f.mCorners[2] = mCorners[2] + bl * np;
	f.mCorners[3] = mCorners[3] + br * np;
	f.mCorners[4] = mCorners[0] + tl * fp;
	f.mCorners[5] = mCorners[1] + tr * fp;
	f.mCorners[6] = mCorners[2] + bl * fp;
	f.mCorners[7] = mCorners[3] + br * fp;

	return f;
}

// ------------------------------------------------------------------------------------------------
Frustum Frustum::GetLightVolume(const glm::vec3& lightDir)
{
	Frustum v;

	if (mDirty) { Compute(); }

	glm::vec3 z = glm::normalize(lightDir);
	glm::vec3 x = glm::normalize(glm::cross(mUp, z));
	glm::vec3 y = glm::cross(z, x);

	glm::vec3 normal[] = {x, y, z, -x, -y, -z};
	for (size_t i = 0; i < 6; ++i)
	{
		for (size_t j = 0; j < 8; ++j)
		{
			float d = -glm::dot(mCorners[j] + 20.0f * normal[i], normal[i]);

			bool good = true;
			for (size_t k = 0; k < 8; ++k)
			{
				if (j != k)
				{
					float dist = glm::dot(mCorners[k], normal[i]) + d;
					if (dist > 0.0f)
					{
						good = false;
						break;
					}
				}
			}

			if (good)
			{
				v.mPlanes[i] = glm::vec4(normal[i], d);
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

	float width  = glm::length(v.mCorners[0] - v.mCorners[1]) / 2.0f;
	float height = glm::length(v.mCorners[0] - v.mCorners[2]) / 2.0f;

	v.mPosition = (v.mCorners[0] + v.mCorners[1] + v.mCorners[2] + v.mCorners[3]) / 4.0f;
	v.mDirection = z;
	v.mUp = mUp;

	v.mNearPlane = 0.0f;
	v.mFarPlane = glm::length(v.mCorners[2] - v.mCorners[6]);
	v.mProjMat = glm::ortho(-width, width, -height, height, v.mNearPlane, v.mFarPlane);
	v.mViewMat = glm::lookAt(v.mPosition, v.mPosition + v.mDirection, v.mUp);
	v.mDirty = false;

	return v;
}

// ------------------------------------------------------------------------------------------------
void Frustum::Compute() const
{
	glm::vec3 z = glm::normalize(-mDirection);
	glm::vec3 x = glm::normalize(glm::cross(mUp, z));
	glm::vec3 y = glm::cross(z, x);
	mViewMat = glm::lookAt(mPosition, mPosition + mDirection, mUp);
		
	if (mIsOrtho)
	{
		glm::vec3 nc = mPosition - z * mNearPlane;
		glm::vec3 fc = mPosition - z * mFarPlane;
		
		mProjMat = glm::ortho(0.0f, mWidth, mHeight, 0.0f);

		mCorners[0]	= nc + y * mHeight / 2.0f - x * mWidth / 2.0f;
		mCorners[1]	= nc + y * mHeight / 2.0f + x * mWidth / 2.0f;
		mCorners[2]	= nc - y * mHeight / 2.0f - x * mWidth / 2.0f;
		mCorners[3]	= nc - y * mHeight / 2.0f + x * mWidth / 2.0f;
		mCorners[4]	= fc + y * mHeight / 2.0f - x * mWidth / 2.0f;
		mCorners[5]	= fc + y * mHeight / 2.0f + x * mWidth / 2.0f;
		mCorners[6]	= fc - y * mHeight / 2.0f - x * mWidth / 2.0f;
		mCorners[7]	= fc - y * mHeight / 2.0f + x * mWidth / 2.0f;

		mPlanes[0] = glm::vec4( x, -glm::dot( x, mCorners[5]));
		mPlanes[1] = glm::vec4( y, -glm::dot( y, mCorners[5]));
		mPlanes[2] = glm::vec4( z, -glm::dot( z, mCorners[5]));
		mPlanes[3] = glm::vec4(-x, -glm::dot(-x, mCorners[2]));
		mPlanes[4] = glm::vec4(-y, -glm::dot(-y, mCorners[2]));
		mPlanes[5] = glm::vec4(-z, -glm::dot(-z, mCorners[2]));
	}
	else
	{
		float tg, nh, fh, nw, fw;
		const float* m;

		mProjMat = glm::perspective(mFOV, mWidth / mHeight, mNearPlane, mFarPlane);
		m = glm::value_ptr(mProjMat * mViewMat * -1.0f);

		glm::vec3 nc = mPosition - z * mNearPlane;
		glm::vec3 fc = mPosition - z * mFarPlane;
		
		tg = tan(mFOV * PI / 360.0f);
		nh = mNearPlane * tg;
		nw = nh * mWidth / mHeight;
		fh = mFarPlane * tg;
		fw = fh * mWidth / mHeight;
				
		mCorners[0]	= nc + y * nh - x * nw;
		mCorners[1]	= nc + y * nh + x * nw;
		mCorners[2]	= nc - y * nh - x * nw;
		mCorners[3]	= nc - y * nh + x * nw;
		mCorners[4]	= fc + y * fh - x * fw;
		mCorners[5]	= fc + y * fh + x * fw;
		mCorners[6]	= fc - y * fh - x * fw;
		mCorners[7]	= fc - y * fh + x * fw;

		mPlanes[0] = glm::vec4(m[3] + m[2], m[7] + m[6], m[11] + m[10], m[15] + m[14]);
		mPlanes[1] = glm::vec4(m[3] - m[2], m[7] - m[6], m[11] - m[10], m[15] - m[14]);
		mPlanes[2] = glm::vec4(m[3] - m[1], m[7] - m[5], m[11] - m[9] , m[15] - m[13]);
		mPlanes[3] = glm::vec4(m[3] + m[1], m[7] + m[5], m[11] + m[9] , m[15] + m[13]);
		mPlanes[4] = glm::vec4(m[3] - m[0], m[7] - m[4], m[11] - m[8] , m[15] - m[12]);
		mPlanes[5] = glm::vec4(m[3] + m[0], m[7] + m[4], m[11] + m[8] , m[15] + m[12]);
	}

	mDirty = false;
}

// ------------------------------------------------------------------------------------------------
bool Frustum::Inside(const Ray& ray) const
{
    throw std::runtime_error("Not implemented: Frustum::Inside(Ray)");
}

// ------------------------------------------------------------------------------------------------
bool Frustum::Inside(const Sphere& ray) const
{
    throw std::runtime_error("Not implemented: Frustum::Inside(Sphere)");
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
			float dot = mPlanes[i].x * box.mCorners[j].x + 
						mPlanes[i].y * box.mCorners[j].y +
						mPlanes[i].z * box.mCorners[j].z +
						mPlanes[i].w;

			if (dot <= 0.0f)
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
bool Frustum::Inside(const Frustum& ray) const
{
    throw std::runtime_error("Not implemented: Frustum::Inside(Frustum)");
}

// ------------------------------------------------------------------------------------------------
bool Frustum::Outside(const Ray& ray) const
{
    throw std::runtime_error("Not implemented: Frustum::Outside(Ray)");
}

// ------------------------------------------------------------------------------------------------
bool Frustum::Outside(const Sphere& ray) const
{
    throw std::runtime_error("Not implemented: Frustum::Outside(Sphere)");
}

// ------------------------------------------------------------------------------------------------
bool Frustum::Outside(const BoundingBox& box) const
{
	if (mDirty) { Compute(); }
	if (box.mDirty) { box.Compute(); }

	for (size_t i = 0; i < 6; ++i)
	{
		size_t in = 0, out = 0;

		for (size_t j = 0; j < 8; ++j)
		{
			float dot = mPlanes[i].x * box.mCorners[j].x + 
						mPlanes[i].y * box.mCorners[j].y +
						mPlanes[i].z * box.mCorners[j].z +
						mPlanes[i].w;

			if (dot <= 0.0f)
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
bool Frustum::Outside(const Frustum& ray) const
{
    throw std::runtime_error("Not implemented: Frustum::Outside(Frustum)");
}
