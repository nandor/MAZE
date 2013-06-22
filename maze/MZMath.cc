// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
glm::vec3 MAZE::Intersect(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c)
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

// ------------------------------------------------------------------------------------------------
bool MAZE::SameSide(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& a, const glm::vec3& b)
{
	return glm::dot(glm::cross(b - a, p1 - a), glm::cross(b - a, p2 - a)) >= -EPS;
}
	
// ------------------------------------------------------------------------------------------------
bool MAZE::Inside(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p)
{
	return SameSide(p, a, b, c) && SameSide(p, b, a, c) && SameSide(p, c, a, b);
}