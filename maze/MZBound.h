// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZBOUND_H
#define MZBOUND_H
#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace MAZE
{
	class BoundingBox;
	class BoundingSphere;
	class ViewFrustum;
	class Ray;
	
	/**
		Result of intersection queries
	*/
	enum Intersection
	{
		OUTSIDE,
		INTERSECT,
		INSIDE
	};

	/**
		Generic collision object
	*/
	class Shape
	{
	public:
					
		virtual Intersection Intersect(const BoundingBox& target) const = 0;
		virtual Intersection Intersect(const BoundingSphere& target) const = 0;
		virtual Intersection Intersect(const ViewFrustum& target) const = 0;
		virtual Intersection Intersect(const Ray& target) const = 0;

	};

	/**
		Ray: origin and direction
	*/
	class Ray : public Shape
	{
	public:

		Ray()
			: mOrigin(0.0f, 0.0f, 0.0f, 1.0f),
			  mDirection(1.0f, 0.0f, 0.0f, 0.0f)
		{
		}

		Ray(const glm::vec3& origin, const glm::vec3& direction)
			: mOrigin(origin, 1.0f),
			  mDirection(glm::normalize(direction), 0.0f)
		{
		}

		glm::vec3 GetOrigin() const
		{
			return glm::vec3(mOrigin);
		}

		glm::vec3 GetDirection() const
		{
			return glm::vec3(mDirection);
		}

		float Distance(const BoundingBox& box) const;

		Intersection Intersect(const BoundingBox& target) const;
		Intersection Intersect(const BoundingSphere& target) const;
		Intersection Intersect(const ViewFrustum& target) const;
		Intersection Intersect(const Ray& target) const;

	private:

		/// Origin point of the ray
		glm::vec4 mOrigin;

		/// Direction of the ray
		glm::vec4 mDirection;

	};

	/**
		Axis-aligned bounding box
	*/
	class BoundingBox : public Shape
	{
	public:
		
		enum
		{
			NEAR_BOTTOM_LEFT	= 0,
			NEAR_BOTTOM_RIGHT	= 1,
			NEAR_TOP_LEFT		= 2,
			NEAR_TOP_RIGHT		= 3,
			FAR_BOTTOM_LEFT		= 4,
			FAR_BOTTOM_RIGHT	= 5,
			FAR_TOP_LEFT		= 6,
			FAR_TOP_RIGHT		= 7,

			NUM_CORNERS			= 8
		};

	public:

		BoundingBox();
		BoundingBox(float x, float y, float z, float w, float h, float d);
		BoundingBox(const glm::vec3& min, const glm::vec3& size);

		void operator = (const BoundingBox& box);

		void SetPosition(const glm::vec3& position);
		void SetPosition(float x, float y, float z);

		void SetSize(const glm::vec3& size);
		void SetSize(float size);
		void SetSize(float width, float height, float depth);

		inline glm::vec3 GetPosition() const
		{
			return mMin;
		}
		
		inline glm::vec3 GetSize() const
		{
			return mMax - mMin;
		}

		inline glm::vec3 GetMax() const
		{
			return mMax;
		}

		inline glm::vec3 GetMin() const
		{
			return mMin;
		}

		BoundingBox Extend(const glm::vec3& dir) const;
		
		Intersection Intersect(const BoundingBox& target) const;
		Intersection Intersect(const BoundingSphere& target) const;
		Intersection Intersect(const ViewFrustum& target) const;
		Intersection Intersect(const Ray& target) const;
		
	private:

		/**
			Computes the bounding points
		*/
		void Compute() const;

	private:

		/// Near-bottom-left
		glm::vec3 mMin;

		/// Far-top-right
		glm::vec3 mMax;
		
		/// Flag to indicate if internal data needs to be updated
		mutable bool mDirty;

		/// List of corner points
		mutable glm::vec3 mCorners[8];
	
		friend class ViewFrustum;
		friend class Ray;
	};

	/**
		Bounding sphere
	*/
	class BoundingSphere : public Shape
	{
	public:

		void SetPosition(const glm::vec3& position)
		{
			mPosition = position;
		}

		glm::vec3 GetPosition()
		{
			return mPosition;
		}

		void SetRadius(float radius)
		{
			mRadius = radius;
		}

		float GetRadius()
		{
			return mRadius;
		}
						
		Intersection Intersect(const BoundingBox& target) const;
		Intersection Intersect(const BoundingSphere& target) const;
		Intersection Intersect(const ViewFrustum& target) const;
		Intersection Intersect(const Ray& target) const;

	private:
		
		glm::vec3 mPosition;
		float mRadius;

		friend class ViewFrustum;
	};

	/**
		View volume
	*/
    class ViewFrustum : public Shape
    {
	public:
		
		/**
			List of bounding planes
		*/
		enum
		{
			FRONT = 0,
			BACK,
			TOP,
			BOTTOM,
			RIGHT,
			LEFT,

			NUM_PLANES
		};

		/**
			List of corner points
		*/
		enum
		{
			NEAR_TOP_LEFT = 0,
			NEAR_TOP_RIGHT,
			NEAR_BOTTOM_LEFT,
			NEAR_BOTTOM_RIGHT,
			FAR_TOP_LEFT,
			FAR_TOP_RIGHT,
			FAR_BOTTOM_LEFT,
			FAR_BOTTOM_RIGHT,

			NUM_CORNERS
		};

		/**
			Types of frustum
		*/
		enum Type
		{
			ORTHOGRAPHIC,
			PERSPECTIVE
		};

	public:

		ViewFrustum()
			: mDirty(false)
		{
		}

		void SetPosition(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up);
		void SetPerspective(float fov, float aspect, float near, float far);
		void SetOrtho(float width, float height, float near, float far);

		ViewFrustum Slice(float near, float far);
		ViewFrustum Expand(float p);
		ViewFrustum GetLightVolume(const glm::vec3& lightDir);

		float GetNearPlane()
		{
			return mNearPlane;
		}

		float GetFarPlane()
		{
			return mFarPlane;
		}

		glm::mat4 GetProjection() const;
		glm::mat4 GetView() const;
						
		Intersection Intersect(const BoundingBox& target) const;
		Intersection Intersect(const BoundingSphere& target) const;
		Intersection Intersect(const ViewFrustum& target) const;
		Intersection Intersect(const Ray& target) const;
				
	private:

		/**
			Computes the bounding points
		*/
		void Compute() const;

	public:

		glm::vec3 mPosition;
		glm::vec3 mDirection;
		glm::vec3 mUp;

		Type mType;
		float mNearPlane;
		float mFarPlane;
		float mWidth;
		float mHeight;
		float mFOV;

		mutable bool mDirty;
		mutable glm::mat4 mProjection;
		mutable glm::mat4 mView;
		mutable glm::vec4 mPlanes[NUM_PLANES];
		mutable glm::vec3 mCorners[NUM_CORNERS];
	};
};

#endif
