// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZMATERIAL_H
#define MZMATERIAL_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "MZTexture.h"

namespace MAZE
{
    class Material
    {
    public:
        
        Material();
        virtual ~Material();
        
    private:
    
        // Diffuse color
        glm::vec3 mDiffuse;
        
        // Specular highlight intensity
        float mSpecularIntensity;
    
        // Diffuse map
        Resource::Ptr<Texture> mDiffuseMap;
        
        // Normal map
        Resource::Ptr<Texture> mNormalMap;
    };
};

#endif
