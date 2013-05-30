// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZRAY_H
#define MZRAY_H
#pragma once

#include <glm/glm.hpp>

namespace MAZE
{
    class Sphere;
    class Frustum;
    class BoundingBox;
    
    class Ray
    {
    public:
    
        Ray() 
            : mOrigin(0.0f), 
              mDirection(0.0f, 1.0f, 0.0f)
        {
        }
        
        Ray(const glm::vec3& o, const glm::vec3& d)
            : mOrigin(o),
              mDirection(d)
        {
        }
            
        bool Inside(const Ray& ray) const;
        bool Inside(const Sphere& sphere) const;
        bool Inside(const BoundingBox& box) const;
        bool Inside(const Frustum& frustum) const;
        
        bool Outside(const Ray& ray) const;
        bool Outside(const Sphere& sphere) const;
        bool Outside(const BoundingBox& box) const;
        bool Outside(const Frustum& frustum) const;

		float Distance(const Ray& ray) const;
		float Distance(const Sphere& sphere) const;
		float Distance(const BoundingBox& box) const;
		float Distance(const Frustum& frustum) const;
		
	public:

		glm::vec3 GetOrigin()
		{
			return mOrigin;
		}

		glm::vec3 GetDirection()
		{
			return mDirection;
		}

		void SetOrigin(const glm::vec3& origin)
		{
			mOrigin = origin;
		}

		void SetDirection(const glm::vec3& direction)
		{
			mDirection = direction;
		}
        
    public:
    
        /// Origin point of the ray
        glm::vec3 mOrigin;
        
        /// Direction of the ray
        glm::vec3 mDirection;
    
		/// List of friend classes
		friend class Sphere;
		friend class Frustum;
		friend class BoundingBox;
    };
};

#endif
