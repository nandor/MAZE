// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZSPHERE_H
#define MZSPHERE_H
#pragma once

#include <glm/glm.hpp>

namespace MAZE
{
    class Ray;
    class Sphere;
    class Frustum;
    
    class Sphere
    {
    public:
    
        Sphere()
            : mCenter(0.0f),
              mRadius(1.0f)
        {
        }
        
        Sphere(const glm::vec3 c, float r)
            : mCenter(c),
              mRadius(r)
        {
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

	public:

		void SetCenter(const glm::vec3& c)
		{
			mCenter = c;
		}

		void SetRadius(float r)
		{
			mRadius = r;
		}

		glm::vec3 GetCenter()
		{
			return mCenter;
		}

		float GetRadius()
		{
			return mRadius;
		}
		
    private:
    
        /// Center of the sphere
        glm::vec3 mCenter;
        
        /// Radius of the sphere
        float mRadius;

		/// List of friends
		friend class Ray;
		friend class Sphere;
		friend class Frustum;
    
    };
};

#endif
