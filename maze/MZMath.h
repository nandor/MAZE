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
		Fast inverse sqrt
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
		Intersection of 3 planes
	*/
	inline glm::vec3 Intersect(glm::vec4 a, glm::vec4 b, glm::vec4 c)
	{
		glm::vec3 c1(a.x, b.x, c.x), c2(a.y, b.y, c.y), c3(a.z, b.z, c.z);

		glm::mat3 A = glm::mat3(c1, c2, c3);
		glm::vec3 cc = glm::vec3(-a.w, -b.w, -c.w);

		float det = glm::determinant(A);
		if (abs(det) <= EPS)
		{
			return glm::vec3(0.0f, 0.0f, 0.0f);
		}

		float x = glm::determinant(glm::mat3(cc, c2, c3)) / det;
		float y = glm::determinant(glm::mat3(c1, cc, c3)) / det;
		float z = glm::determinant(glm::mat3(c1, c2, cc)) / det;

		return glm::vec3(x, y, z);
	}

	/**
		Liniarize depth values
	*/
	inline float LinearDepth(float z, float n, float f)
	{
		return (2.0f * n)  / (f + n - z * (f - n));
	}
};

#endif
