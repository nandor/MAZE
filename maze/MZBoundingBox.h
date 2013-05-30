// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZBOUNDINGBOX_H
#define MZBOUNDINGBOX_H
#pragma once

#include <glm/glm.hpp>

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
        
		BoundingBox();
        BoundingBox(const glm::vec3& pos, const glm::vec3& size);
        BoundingBox(float px, float py, float pz, float w, float h, float d);
        
        BoundingBox Move(const glm::vec3& pos);
        BoundingBox Extend(const glm::vec3& dir);
                
        bool Inside(const Ray& ray) const;
        bool Inside(const Sphere& sphere) const;
		bool Inside(const glm::vec3& point) const;
        bool Inside(const BoundingBox& ray) const;
        bool Inside(const Frustum& frustum) const;
        
        bool Outside(const Ray& ray) const;
        bool Outside(const Sphere& sphere) const;
		bool Outside(const glm::vec3& point) const;
        bool Outside(const BoundingBox& ray) const;
        bool Outside(const Frustum& frustum) const;

		float Distance(const Ray& ray) const;
		float Distance(const Sphere& sphere) const;
		float Distance(const BoundingBox& box) const;
		float Distance(const Frustum& frustum) const;
            
	public:

		glm::vec3 GetPosition() const
		{ 
			return mMin; 
		}

		glm::vec3 GetSize() const
		{ 
			return mMax - mMin; 
		}

		glm::vec3 GetMin() const
		{ 
			return mMin; 
		}

		glm::vec3 GetMax() const
		{ 
			return mMax; 
		}

		glm::vec4 GetPlane(size_t idx) const
		{
			if (mDirty) { Compute(); }
			assert(0 <= idx && idx < 6);
			return mPlanes[idx];
		}

		glm::vec3 GetCorner(size_t idx) const
		{
			if (mDirty) { Compute(); }
			assert(0 <= idx && idx < 8);
			return mCorners[idx];
		}

		void SetPosition(const glm::vec3& pos)
		{
			mMax = pos + mMax - mMin;
			mMin = pos;
			mDirty = true;
		}

		void SetSize(const glm::vec3& size) 
		{ 
			mMax = mMin + size; 
			mDirty = true; 
		}

		void SetMin(const glm::vec3& min) 
		{
			mMin = min; 
			mDirty = true; 
		}

		void SetMax(const glm::vec3& max) 
		{
			mMax = max; 
			mDirty = true; 
		}

    private:
    
		/// Cache internal data
		void Compute() const;

	private:

        /// Front - left - bottom
        glm::vec3 mMin;
        
        /// Back - right - top
        glm::vec3 mMax;
        
		/// True if something changed
		mutable bool mDirty;

        /// The 8 corner points
        mutable glm::vec3 mCorners[8];
        
        /// The 6 bounding planes
        mutable glm::vec4 mPlanes[6];

		/// List of friends
		friend class Ray;
		friend class Sphere;
		friend class Frustum;
    
    };
};

#endif
