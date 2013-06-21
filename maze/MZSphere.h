// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZSPHERE_H
#define MZSPHERE_H
#pragma once

namespace MAZE
{
    class Ray;
    class BoundingBox;
    class Frustum;
    
    class Sphere
    {
    public:
    
        Sphere()
        {			
			mCenter = _mm_setr_ps(0.0f, 0.0f, 0.0f, 0.0f);
			mRadius = _mm_set_ps1(1.0f);
        }
        
        Sphere(const glm::vec3 c, float r)
        {
			mCenter = _mm_setr_ps(c.z, c.y, c.x, 0.0f);
			mRadius = _mm_set_ps1(r);
        }
            
        bool Inside(const Ray& ray) const;
        bool Inside(const Sphere& sphere) const;
        bool Inside(const BoundingBox& ray) const;
        bool Inside(const Frustum& frustum) const;
        
        bool Outside(const Ray& ray) const;
        bool Outside(const Sphere& sphere) const;
        bool Outside(const BoundingBox& ray) const;
        bool Outside(const Frustum& frustum) const;
		
		float Distance(const Ray& ray) const;
		float Distance(const Sphere& sphere) const;
		float Distance(const BoundingBox& box) const;
		float Distance(const Frustum& frustum) const;
				
    private:
    
		/// Position of the sphere
		__m128 mCenter;

		/// Radius of the sphere
		__m128 mRadius;
		
	private:

		friend class Ray;
		friend class BoundingBox;
		friend class Frustum;
    
    };
};

#endif
