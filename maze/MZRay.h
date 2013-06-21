// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZRAY_H
#define MZRAY_H
#pragma once

namespace MAZE
{
    class Sphere;
    class Frustum;
    class BoundingBox;
    
    class Ray
    {
    public:
    
        Ray() 
        {
			mDirection = _mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f);
			mOrigin    = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
        }
        
        Ray(const glm::vec3& o, const glm::vec3& d)
        {
			mDirection = _mm_setr_ps(d.x, d.y, d.z, 0.0f);
			mOrigin    = _mm_setr_ps(o.x, o.y, o.z, 1.0f);
        }
            
        bool Inside(const Ray& ray) const NOT_IMPLEMENTED;
        bool Inside(const Sphere& sphere) const NOT_IMPLEMENTED;
        bool Inside(const BoundingBox& box) const;
        bool Inside(const Frustum& frustum) const NOT_IMPLEMENTED;
        
        bool Outside(const Ray& ray) const NOT_IMPLEMENTED;
        bool Outside(const Sphere& sphere) const NOT_IMPLEMENTED;
        bool Outside(const BoundingBox& box) const;
        bool Outside(const Frustum& frustum) const NOT_IMPLEMENTED;

		__m128 Distance(const Ray& ray) const NOT_IMPLEMENTED;
		__m128 Distance(const Sphere& sphere) const NOT_IMPLEMENTED;
		__m128 Distance(const BoundingBox& box) const;
		__m128 Distance(const Frustum& frustum) const NOT_IMPLEMENTED;
		        
	private:
			
		/// Origin of the ray
		__m128 mOrigin;

		/// Direction of the ray
		__m128 mDirection;
		
	private:

		friend class Sphere;
		friend class Frustum;
		friend class BoundingBox;
    };
};

#endif
