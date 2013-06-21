// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZFRUSTUM_H
#define MZFRUSTUM_H
#pragma once

namespace MAZE
{
    class Ray;
    class Sphere;
    class BoundingBox;
    
    class Frustum
    {
    public:

		Frustum();
    
		Frustum(const glm::vec3& p, const glm::vec3& d, const glm::vec3& u,
				float l, float r, float t, float b, float n, float f);

		Frustum(const glm::vec3& p, const glm::vec3& d, const glm::vec3& u,
				float fov, float a, float n, float f);

		Frustum Slice(float near, float far);
        Frustum GetLightVolume(const glm::vec3& lightDir);
                
        bool Inside(const Ray& ray) const NOT_IMPLEMENTED;
        bool Inside(const Sphere& sphere) const NOT_IMPLEMENTED;
        bool Inside(const Frustum& frustum) const NOT_IMPLEMENTED;
        bool Inside(const BoundingBox& box) const;
        
        bool Outside(const Ray& ray) const NOT_IMPLEMENTED;
        bool Outside(const Sphere& sphere) const NOT_IMPLEMENTED;
        bool Outside(const Frustum& frustum) const NOT_IMPLEMENTED;
        bool Outside(const BoundingBox& box) const;

		__m128 Distance(const BoundingBox& box) const NOT_IMPLEMENTED;
		__m128 Distance(const Sphere& sphere) const NOT_IMPLEMENTED;
		__m128 Distance(const Ray& ray) const NOT_IMPLEMENTED;
		__m128 Distance(const Frustum& frustum) const NOT_IMPLEMENTED;
    
	public:

		void SetPerspective(float fov, float aspect, float n, float f);
		void SetOrtho(float l, float r, float t, float b, float n, float f);

		void SetPosition(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up)
		{
			mPosition = pos;
			mDirection = dir;
			mUp = up;
		}

		const glm::mat4& GetProjMat() const
		{
			if (mDirty) { Compute(); }
			return mProjMat;
		}

		const glm::mat4& GetViewMat() const
		{
			if (mDirty) { Compute(); }
			return mViewMat;
		}

		float GetNearPlane() const
		{
			return mNearPlane;
		}

		float GetFarPlane() const
		{
			return mFarPlane;
		}
		
    private:   

		/// Computes internal data
		void Compute() const;

	private:
		
		/// Camera position
		glm::vec3 mPosition;

		/// Z (forwards)
		glm::vec3 mDirection;

		/// Y (up)
		glm::vec3 mUp;

		/// Type of the volume
        bool mIsOrtho;

		/// Near plane distance
		float mNearPlane;

		/// Far plane distance
		float mFarPlane;

		/// Width of the near plane
		float mWidth;

		/// Height of the near plane
		float mHeight;

		/// FOV for projection matrix
		float mFOV;
        
		/// True if needs update
		mutable bool mDirty;

		/// Projection matrix
		mutable glm::mat4 mProjMat;

		/// View matrix
		mutable glm::mat4 mViewMat;
			
		/// List of bounding planes
		mutable __m128 mPlanes[6];

		/// List of corner points
		mutable __m128 mCorners[8];	

	private:

		friend class Ray;
		friend class Sphere;
		friend class BoundingBox;
    };
};

#endif
