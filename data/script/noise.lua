-- This file is part of the MAZE project
-- Licensing information can be found in the LICENSE file
-- (C) 2013 The MAZE project. All rights reserved.

-- 2D perlin noise generator
perlin2D = {}
perlin2D_meta = {}
perlin2D_cons = {}
setmetatable(perlin2D, perlin2D_cons)
perlin2D_meta.__index = perlin2D_meta

-- Creates a new noise generator
perlin2D_cons.__call = function(self, seed)
    local noise = {}
    setmetatable(noise, perlin2D_meta)
    
    noise.seed = seed
    
    return noise
end

-- Print info about the noise generator
perlin2D_meta.__tostring = function(self)
    return "perlin(" .. self.seed .. ")"
end

-- Return the noise value in a given point
perlin2D_meta.noise = function(self, x, y)
    return 0.0
end
