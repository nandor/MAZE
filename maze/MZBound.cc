// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZMath.h"
#include "MZBound.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
BoundingBox::BoundingBox()
{
	mMin = glm::vec3(0.0f);
	mMax = glm::vec3(0.0f);
	mDirty = true;
}

// ------------------------------------------------------------------------------------------------
BoundingBox::BoundingBox(float x, float y, float z, float w, float h, float d)
{
	mMin.x = x;
	mMin.y = y;
	mMin.z = z;

	mMax.x = x + w;
	mMax.y = y + h;
	mMax.z = z + d;

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
void BoundingBox::operator = (const BoundingBox& box)
{
	mMin = box.mMin;
	mMax = box.mMax;

	mDirty = true;
}

// ------------------------------------------------------------------------------------------------
void BoundingBox::SetPosition(const glm::vec3& pos)
{
	glm::vec3 size;

	size = mMax - mMin;
	mMin = pos;
	mMax = pos + size;

	mDirty = true;
}

// ------------------------------------------------------------------------------------------------
void BoundingBox::SetPosition(float x, float y, float z)
{
	glm::vec3 size;

	size = mMax - mMin;

	mMin.x = x;
	mMin.y = y;
	mMin.z = z;

	mMax = mMin + size;
	mDirty = true;
}

// ------------------------------------------------------------------------------------------------
void BoundingBox::SetSize(const glm::vec3& size)
{
	mMax = mMin + size;
	mDirty = true;
}

// ------------------------------------------------------------------------------------------------
void BoundingBox::SetSize(float size)
{
	mMax = mMin + glm::vec3(size);
	mDirty = true;
}

// ------------------------------------------------------------------------------------------------
void BoundingBox::SetSize(float w, float h, float d)
{
	mMax = mMin + glm::vec3(w, h, d);
	mDirty = true;
}

// ------------------------------------------------------------------------------------------------
void BoundingBox::Compute() const
{
	mCorners[NEAR_BOTTOM_LEFT]	= glm::vec3(mMin.x, mMin.y, mMin.z);
	mCorners[NEAR_BOTTOM_RIGHT] = glm::vec3(mMax.x, mMin.y, mMin.z);
	mCorners[NEAR_TOP_LEFT]		= glm::vec3(mMin.x, mMax.y, mMin.z);
	mCorners[NEAR_TOP_RIGHT]	= glm::vec3(mMax.x, mMax.y, mMin.z);
	mCorners[FAR_BOTTOM_LEFT]	= glm::vec3(mMin.x, mMin.y, mMax.z);
	mCorners[FAR_BOTTOM_RIGHT]	= glm::vec3(mMax.x, mMin.y, mMax.z);
	mCorners[FAR_TOP_LEFT]		= glm::vec3(mMin.x, mMax.y, mMax.z);
	mCorners[FAR_TOP_RIGHT]		= glm::vec3(mMax.x, mMax.y, mMax.z);
}

// ------------------------------------------------------------------------------------------------
BoundingBox BoundingBox::Extend(const glm::vec3& dir) const
{
	BoundingBox box;

	box.mMin.x = mMin.x + std::min(dir.x, 0.0f);
	box.mMin.y = mMin.y + std::min(dir.y, 0.0f);
	box.mMin.z = mMin.z + std::min(dir.z, 0.0f);

	box.mMax.x = mMax.x + std::max(dir.x, 0.0f);
	box.mMax.y = mMax.y + std::max(dir.y, 0.0f);
	box.mMax.z = mMax.z + std::max(dir.z, 0.0f);

	box.mDirty = true;

	return box;
}

// ------------------------------------------------------------------------------------------------
Intersection BoundingBox::Intersect(const BoundingBox& f) const
{
	if (f.mMax.x <= mMin.x || mMax.x <= f.mMin.x ||
		f.mMax.y <= mMin.y || mMax.y <= f.mMin.y ||
		f.mMax.z <= mMin.z || mMax.z <= f.mMin.z)
	{
		return OUTSIDE;
	}

	if (mMin.x <= f.mMin.x && f.mMax.x <= mMax.x &&
		mMin.y <= f.mMin.y && f.mMax.y <= mMax.y && 
		mMin.z <= f.mMin.z && f.mMax.z <= mMax.z)
	{
		return INSIDE;
	}

	return INTERSECT;
}

// ------------------------------------------------------------------------------------------------
Intersection BoundingBox::Intersect(const BoundingSphere& f) const
{
	return INSIDE;
}

// ------------------------------------------------------------------------------------------------
Intersection BoundingBox::Intersect(const ViewFrustum& f) const
{
	return INSIDE;
}

// ------------------------------------------------------------------------------------------------
void ViewFrustum::SetPosition(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up)
{
	mPosition = pos;
	mDirection = dir;
	mUp = up;

	mDirty = true;
}

// ------------------------------------------------------------------------------------------------
void ViewFrustum::SetPerspective(float fov, float aspect, float near, float far)
{
	mType = PERSPECTIVE;
	
	mFOV = fov;
	mWidth = aspect;
	mHeight = 1.0f;
	mNearPlane = near;
	mFarPlane = far;
	
	mDirty = true;
}

// ------------------------------------------------------------------------------------------------
void ViewFrustum::SetOrtho(float width, float height, float near, float far)
{
	mType = ORTHOGRAPHIC;

	mWidth = width;
	mHeight = height;
	mNearPlane = near;
	mFarPlane = far;

	mDirty = true;
}


// ------------------------------------------------------------------------------------------------
void ViewFrustum::Compute() const
{
	const float *m;
	float tg, nh, fh, nw, fw;	
	glm::mat4 mvp;
	
	glm::vec3 z = glm::normalize(-mDirection);
	glm::vec3 x = glm::normalize(glm::cross(mUp, z));
	glm::vec3 y = glm::cross(z, x);

	mView = glm::lookAt(mPosition, mPosition + mDirection, mUp);
	
	if (mType == PERSPECTIVE)
	{
		mProjection = glm::perspective(mFOV, mWidth / mHeight, mNearPlane, mFarPlane);
		m = glm::value_ptr(mProjection * mView * -1.0f);

		mPlanes[LEFT]   = glm::vec4(m[3] + m[0], m[7] + m[4], m[11] + m[8] , m[15] + m[12]);
		mPlanes[RIGHT]  = glm::vec4(m[3] - m[0], m[7] - m[4], m[11] - m[8] , m[15] - m[12]);
		mPlanes[BOTTOM] = glm::vec4(m[3] + m[1], m[7] + m[5], m[11] + m[9] , m[15] + m[13]);
		mPlanes[TOP]	= glm::vec4(m[3] - m[1], m[7] - m[5], m[11] - m[9] , m[15] - m[13]);
		mPlanes[FRONT]  = glm::vec4(m[3] + m[2], m[7] + m[6], m[11] + m[10], m[15] + m[14]);
		mPlanes[BACK]   = glm::vec4(m[3] - m[2], m[7] - m[6], m[11] - m[10], m[15] - m[14]);
		
		tg = tan(mFOV * PI / 360.0f);
		nh = mNearPlane * tg;
		nw = nh * mWidth / mHeight;
		fh = mFarPlane * tg;
		fw = fh * mWidth / mHeight;

		glm::vec3 nc = mPosition - z * mNearPlane;
		glm::vec3 fc = mPosition - z * mFarPlane;

		mCorners[NEAR_TOP_RIGHT]	= nc + y * nh + x * nw;
		mCorners[NEAR_TOP_LEFT]		= nc + y * nh - x * nw;
		mCorners[NEAR_BOTTOM_RIGHT]	= nc - y * nh + x * nw;
		mCorners[NEAR_BOTTOM_LEFT]	= nc - y * nh - x * nw;
		mCorners[FAR_TOP_RIGHT]		= fc + y * fh + x * fw;
		mCorners[FAR_TOP_LEFT]		= fc + y * fh - x * fw;
		mCorners[FAR_BOTTOM_RIGHT]	= fc - y * fh + x * fw;
		mCorners[FAR_BOTTOM_LEFT]	= fc - y * fh - x * fw;
	}
	else
	{
		mProjection = glm::ortho(0.0f, mWidth, mHeight, 0.0f);
		
		glm::vec3 nc = mPosition - z * mNearPlane;
		glm::vec3 fc = mPosition - z * mFarPlane;
		
		mCorners[NEAR_BOTTOM_LEFT]  = nc + y * mHeight / 2.0f + x * mWidth / 2.0f;	
		mCorners[NEAR_TOP_RIGHT]	= nc + y * mHeight / 2.0f + x * mWidth / 2.0f;
		mCorners[NEAR_TOP_LEFT]		= nc + y * mHeight / 2.0f - x * mWidth / 2.0f;
		mCorners[NEAR_BOTTOM_RIGHT]	= nc - y * mHeight / 2.0f + x * mWidth / 2.0f;
		mCorners[NEAR_BOTTOM_LEFT]	= nc - y * mHeight / 2.0f - x * mWidth / 2.0f;
		mCorners[FAR_TOP_RIGHT]		= fc + y * mHeight / 2.0f + x * mWidth / 2.0f;
		mCorners[FAR_TOP_LEFT]		= fc + y * mHeight / 2.0f - x * mWidth / 2.0f;
		mCorners[FAR_BOTTOM_RIGHT]	= fc - y * mHeight / 2.0f + x * mWidth / 2.0f;
		mCorners[FAR_BOTTOM_LEFT]	= fc - y * mHeight / 2.0f - x * mWidth / 2.0f;

		mPlanes[FRONT]	= glm::vec4(-z, -glm::dot(-z, mCorners[NEAR_BOTTOM_LEFT] ));
		mPlanes[BACK]	= glm::vec4( z, -glm::dot( z, mCorners[FAR_BOTTOM_LEFT]	 ));
		mPlanes[TOP]	= glm::vec4( y, -glm::dot( y, mCorners[NEAR_TOP_LEFT]	 ));
		mPlanes[BOTTOM] = glm::vec4(-y, -glm::dot(-y, mCorners[NEAR_BOTTOM_LEFT] ));
		mPlanes[RIGHT]	= glm::vec4( x, -glm::dot( x, mCorners[NEAR_BOTTOM_RIGHT]));
		mPlanes[LEFT]	= glm::vec4(-x, -glm::dot(-x, mCorners[NEAR_BOTTOM_LEFT] ));
	}
}

// ------------------------------------------------------------------------------------------------
ViewFrustum ViewFrustum::Slice(float near, float far)
{
	ViewFrustum f;

	if (mDirty)
	{
		Compute();
	}
	
	glm::vec3 tl = mCorners[FAR_TOP_LEFT]	  - mCorners[NEAR_TOP_LEFT];
	glm::vec3 bl = mCorners[FAR_BOTTOM_LEFT]  - mCorners[NEAR_BOTTOM_LEFT];
	glm::vec3 tr = mCorners[FAR_TOP_RIGHT]	  - mCorners[NEAR_TOP_RIGHT];
	glm::vec3 br = mCorners[FAR_BOTTOM_RIGHT] - mCorners[NEAR_BOTTOM_RIGHT];
	
	f.mType		 = mType;
	f.mNearPlane = near;
	f.mFarPlane	 = far;
	f.mUp		 = mUp;
	f.mDirection = mDirection;
	f.mPosition	 = mPosition;
	f.mDirty = false;

	near = (near - mNearPlane) / (mFarPlane - mNearPlane);
	far = (far - mNearPlane) / (mFarPlane - mNearPlane);
	
	f.mCorners[NEAR_TOP_LEFT]  = mCorners[NEAR_TOP_LEFT]  + tl * near;
	f.mCorners[FAR_TOP_LEFT]   = mCorners[NEAR_TOP_LEFT]  + tl * far;
	f.mCorners[NEAR_TOP_RIGHT] = mCorners[NEAR_TOP_RIGHT] + tr * near;
	f.mCorners[FAR_TOP_RIGHT]  = mCorners[NEAR_TOP_RIGHT] + tr * far;

	f.mCorners[NEAR_BOTTOM_LEFT]  = mCorners[NEAR_BOTTOM_LEFT]  + bl * near;
	f.mCorners[FAR_BOTTOM_LEFT]	  = mCorners[NEAR_BOTTOM_LEFT]  + bl * far;
	f.mCorners[NEAR_BOTTOM_RIGHT] = mCorners[NEAR_BOTTOM_RIGHT] + br * near;
	f.mCorners[FAR_BOTTOM_RIGHT]  = mCorners[NEAR_BOTTOM_RIGHT] + br * far;

	return f;
}

// ------------------------------------------------------------------------------------------------
ViewFrustum ViewFrustum::GetLightVolume(const glm::vec3& lightDir)
{
	ViewFrustum v;

	if (mDirty)
	{
		Compute();
	}

	glm::vec3 z = glm::normalize(lightDir);
	glm::vec3 x = glm::normalize(glm::cross(mUp, z));
	glm::vec3 y = glm::cross(z, x);

	glm::vec3 normal[] = {-z, z,  y, -y,  x, -x};
	for (size_t i = 0; i < NUM_PLANES; ++i)
	{
		for (size_t j = 0; j < NUM_CORNERS; ++j)
		{
			float d = -glm::dot(mCorners[j] + 20.0f * normal[i], normal[i]);

			bool good = true;
			for (size_t k = 0; k < NUM_CORNERS; ++k)
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

	v.mCorners[NEAR_TOP_LEFT]	  = ::Intersect(v.mPlanes[FRONT], v.mPlanes[TOP],	v.mPlanes[LEFT]);
	v.mCorners[NEAR_TOP_RIGHT]	  = ::Intersect(v.mPlanes[FRONT], v.mPlanes[TOP],	v.mPlanes[RIGHT]);
	v.mCorners[NEAR_BOTTOM_LEFT]  = ::Intersect(v.mPlanes[FRONT], v.mPlanes[BOTTOM], v.mPlanes[LEFT]);
	v.mCorners[NEAR_BOTTOM_RIGHT] = ::Intersect(v.mPlanes[FRONT], v.mPlanes[BOTTOM], v.mPlanes[RIGHT]);

	v.mCorners[FAR_TOP_LEFT]	  = ::Intersect(v.mPlanes[BACK],  v.mPlanes[TOP],	v.mPlanes[LEFT]);
	v.mCorners[FAR_TOP_RIGHT]	  = ::Intersect(v.mPlanes[BACK],  v.mPlanes[TOP],	v.mPlanes[RIGHT]);
	v.mCorners[FAR_BOTTOM_LEFT]   = ::Intersect(v.mPlanes[BACK],  v.mPlanes[BOTTOM], v.mPlanes[LEFT]);
	v.mCorners[FAR_BOTTOM_RIGHT]  = ::Intersect(v.mPlanes[BACK],  v.mPlanes[BOTTOM], v.mPlanes[RIGHT]);

	float width = glm::length(v.mCorners[NEAR_TOP_LEFT] - v.mCorners[NEAR_TOP_RIGHT]);
	float height = glm::length(v.mCorners[NEAR_TOP_LEFT] - v.mCorners[NEAR_BOTTOM_LEFT]);

	v.mPosition = (v.mCorners[0] + v.mCorners[1] + v.mCorners[2] + v.mCorners[3]) / 4.0f;

	v.mDirection = z;
	v.mUp = mUp;
	v.mNearPlane = 0.0f;
	v.mDirty = false;
	v.mFarPlane = glm::length(v.mCorners[NEAR_BOTTOM_LEFT] - v.mCorners[FAR_BOTTOM_LEFT]);
	v.mProjection = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, v.mNearPlane, v.mFarPlane);
	v.mView = glm::lookAt(v.mPosition, v.mPosition + v.mDirection, v.mUp);
	
	return v;
}

// ------------------------------------------------------------------------------------------------
Intersection ViewFrustum::Intersect(const ViewFrustum& f) const
{
	return INSIDE;
}

// ------------------------------------------------------------------------------------------------
Intersection ViewFrustum::Intersect(const BoundingBox& f) const
{
	Intersection result = INSIDE;

	if (mDirty)
	{
		Compute();
	}

	if (f.mDirty)
	{
		f.Compute();
	}

	for (size_t i = 0; i < 6; ++i)
	{
		size_t in = 0, out = 0;

		for (size_t j = 0; j < 8; ++j)
		{
			float dot = mPlanes[i].x * f.mCorners[j].x + 
						mPlanes[i].y * f.mCorners[j].y +
						mPlanes[i].z * f.mCorners[j].z +
						mPlanes[i].w;

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
			return OUTSIDE;
		}
		else if (out > 0)
		{
			result = INTERSECT;
		}
	}

	return result;
}

// ------------------------------------------------------------------------------------------------
Intersection ViewFrustum::Intersect(const BoundingSphere& f) const
{
	return INSIDE;
}
