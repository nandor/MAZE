-- This file is part of the MAZE project
-- Licensing information can be found in the LICENSE file
-- (C) 2013 The MAZE project. All rights reserved.

-------------------------------------------------------------------------------
-- vec3 
-------------------------------------------------------------------------------
vec3_meta = {}
vec3_meta.__index = vec3_meta

function vec3(x, y, z)

    object = setmetatable({}, vec3_meta)
    object.__type = "vec3"
    
    object.x = x
    object.y = y
    object.z = z
    
    return object
end

function vec3_meta:length()
    return math.sqrt(self.x * self.x + self.y * self.y + self.z * self.z)
end

function vec3_meta:__tostring()
    return string.format("vec3(%.2f %.2f %.2f)", self.x, self.y, self.z)
end

-------------------------------------------------------------------------------
-- box
-------------------------------------------------------------------------------
box_meta = {}
box_meta.__index = box_meta

function box(pos, size)
    
    object = setmetatable({}, box_meta)
    object.__type = "box"
    
    object.pos_x = pos.x
    object.pos_y = pos.y
    object.pos_z = pos.z
    
    object.size_x = size.x
    object.size_y = size.y
    object.size_z = size.z
    
    return object
end

function box_meta:__tostring()
    return string.format(
        "box((%.2f, %.2f, %.2f), (%.2f, %.2f, %.2f))", 
        self.pos_x, self.pos_y, self.pos_z,
        self.size_x, self.size_y, self.size_z
    )
end
