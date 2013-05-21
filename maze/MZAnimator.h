// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZANIMATOR_H
#define MZANIMATOR_H
#pragma once

namespace MAZE
{
    class Animator
    {
    public:
    
        Animator();
        ~Animator();
        
    private:
    
        glm::vec3* mField;
    
    };
};

#endif
