// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MATH_H
#define MATH_H
#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace MAZE
{
	const float PI = 3.14159265359f;
	const float PIOVER2 = PI / 2.0f;
	const float PIOVER3 = PI / 3.0f;
	const float PIOVER4 = PI / 4.0f;
	const float PIOVER6 = PI / 6.0f;
	const float PIOVER8 = PI / 8.0f;

	const float E = 2.71828182845f;
	const float EPS = 0.001f;

	/**
		Fast inverse sqrt - from quake 3
	*/
	inline float FastInvSqrt(float number)
	{
		long i;
		float x2, y;
 
		x2 = number * 0.5F;
		y = number;

		i = *((long*)&y);
		i = 0x5f3759df - (i >> 1);
		
		y = *((float*) &i);
		y = y * (1.5f - (x2 * y * y)); 
 
		return y;
	}

	/**
		Fast sqrt
	*/
	inline float FastSqrt(float number)
	{
		return 1.0f / FastInvSqrt(number);
	}

	/**
		Liniarize depth values
		@param z Depth clamped to [0, 1]
		@param n Near plane
		@param f Far plane
	*/
	inline float LinearDepth(float z, float n, float f)
	{
		return (2.0f * n)  / (f + n - z * (f - n));
	}

	/**
		Intersection of 3 planes
		@param a Equation of the first plane
		@param b Equation of the second plane
		@param c Equation of the third plane
	*/
	glm::vec3 Intersect(const glm::vec4& a, 
						const glm::vec4& b, 
						const glm::vec4& c);

	/**
		Check if two points are on the same side of a line
		@param p1 First endpoint of the line
		@param p2 Second endpoint of the line
		@param a First point
		@param b Second point
	*/
	bool SameSide(const glm::vec3& p1, 
				  const glm::vec3& p2, 
				  const glm::vec3& a, 
				  const glm::vec3& b);

	/**
		Check if a point is inside a triangle
	*/
	bool Inside(const glm::vec3& a, 
				const glm::vec3& b, 
				const glm::vec3& c, 
				const glm::vec3& p);
};

#endif
