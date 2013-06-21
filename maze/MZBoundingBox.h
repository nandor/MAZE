// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZBOUNDINGBOX_H
#define MZBOUNDINGBOX_H
#pragma once

namespace MAZE
{
    class Ray;
    class Sphere;
    class Frustum;
    
    /**
        Axis - aligned bounding box
    */
    class BoundingBox
    {
    public:
        
		BoundingBox()
		{
			mMin = _mm_setr_ps(0.0f, 0.0f, 0.0f, 0.0f);
			mMax = _mm_setr_ps(1.0f, 1.0f, 1.0f, 0.0f);
			mDirty = true;
		}

		BoundingBox(const __m128& p, const __m128& s)
		{
			mMin = p;
			mMax = _mm_add_ps(p, s);
			mDirty = true;
		}

        BoundingBox(const glm::vec3& p, const glm::vec3& s)
		{
			mMin = _mm_setr_ps(p.x, p.y, p.z, 0.0f);
			mMax = _mm_add_ps(mMin, _mm_setr_ps(s.x, s.y, s.z, 1.0f));
			mDirty = true;
		}

        BoundingBox(float x, float y, float z, float w, float h, float d)
		{
			mMin = _mm_setr_ps(x, y, z, 0.0f);
			mMax = _mm_add_ps(mMin, _mm_setr_ps(w, h, d, 0.0f));
			mDirty = true;
		}
        
        BoundingBox Move(const glm::vec3& p);
        BoundingBox Move(const __m128& p);
        BoundingBox Extend(const glm::vec3& d);
        BoundingBox Extend(const __m128& d);
                
        bool Inside(const Ray& ray) const;
		bool Inside(const Sphere& sphere) const NOT_IMPLEMENTED;
        bool Inside(const BoundingBox& box) const;
        bool Inside(const Frustum& frustum) const;
        
        bool Outside(const Ray& ray) const;
		bool Outside(const Sphere& sphere) const NOT_IMPLEMENTED;
        bool Outside(const BoundingBox& box) const;
        bool Outside(const Frustum& frustum) const;

		__m128 Distance(const Ray& ray) const;
		__m128 Distance(const Sphere& sphere) const NOT_IMPLEMENTED;
		__m128 Distance(const BoundingBox& box) const NOT_IMPLEMENTED;
		__m128 Distance(const Frustum& frustum) const NOT_IMPLEMENTED;
            
	public:

		__m128 GetPosition() const
		{
			return mMin;
		}

		__m128 GetSize() const
		{
			return _mm_sub_ps(mMax, mMin);
		}

		__m128 GetMin() const
		{
			return mMin;
		}

		__m128 GetMax() const
		{
			return mMax;
		}

		__m128 GetPlane(size_t idx) const
		{
			if (mDirty) { Compute(); }
			assert(0 <= idx && idx < 6);
			return mPlanes[idx];
		}

		__m128 GetCorner(size_t idx) const
		{
			if (mDirty) { Compute(); }
			assert(0 <= idx && idx < 8);
			return mCorners[idx];
		}
		
    private:
    
		/// Cache internal data
		void Compute() const;

	private:
		
		/// Lower - Left - Front
		__m128 mMin;

		/// Upper - Right - Back
		__m128 mMax;
		
		/// Corner point coordinates
		mutable __m128 mCorners[8];
        
		/// Bounding plane equations
		mutable __m128 mPlanes[6];

		/// True if something changed
		mutable bool mDirty;
        
	private:

		friend class Ray;
		friend class Sphere;
		friend class Frustum;
    
    };
};

#endif
