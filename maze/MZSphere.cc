// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
bool Sphere::Inside(const Ray& ray) const
{
    throw std::runtime_error("Not implemented: Sphere::Inside(Ray)");
}

// ------------------------------------------------------------------------------------------------
bool Sphere::Inside(const Sphere& ray) const
{
    throw std::runtime_error("Not implemented: Sphere::Inside(Sphere)");
}

// ------------------------------------------------------------------------------------------------
bool Sphere::Inside(const BoundingBox& ray) const
{
    throw std::runtime_error("Not implemented: Sphere::Inside(BoundingBox)");
}

// ------------------------------------------------------------------------------------------------
bool Sphere::Inside(const Frustum& ray) const
{
    throw std::runtime_error("Not implemented: Sphere::Inside(Frustum)");
}

// ------------------------------------------------------------------------------------------------
bool Sphere::Outside(const Ray& ray) const
{
    throw std::runtime_error("Not implemented: Sphere::Outside(Ray)");
}

// ------------------------------------------------------------------------------------------------
bool Sphere::Outside(const Sphere& ray) const
{
    throw std::runtime_error("Not implemented: Sphere::Outside(Sphere)");
}

// ------------------------------------------------------------------------------------------------
bool Sphere::Outside(const BoundingBox& ray) const
{
    throw std::runtime_error("Not implemented: Sphere::Outside(BoundingBox)");
}

// ------------------------------------------------------------------------------------------------
bool Sphere::Outside(const Frustum& ray) const
{
    throw std::runtime_error("Not implemented: Sphere::Outside(Frustum)");
}

// ------------------------------------------------------------------------------------------------
float Sphere::Distance(const Ray& ray) const
{
    throw std::runtime_error("Not implemented: Sphere::Distance(Ray)");
}

// ------------------------------------------------------------------------------------------------
float Sphere::Distance(const Sphere& sphere) const
{
    throw std::runtime_error("Not implemented: Sphere::Distance(Sphere)");
}

// ------------------------------------------------------------------------------------------------
float Sphere::Distance(const BoundingBox& box) const
{
    throw std::runtime_error("Not implemented: Sphere::Distance(BoundingBox)");
}

// ------------------------------------------------------------------------------------------------
float Sphere::Distance(const Frustum& frustum) const
{
    throw std::runtime_error("Not implemented: Sphere::Distance(Frustum)");
}