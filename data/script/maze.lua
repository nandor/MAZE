-- This file is part of the MAZE project
-- Licensing information can be found in the LICENSE file
-- (C) 2013 The MAZE project. All rights reserved.

local score = 0

function on_coin_update(self, t, dt)
    --[[ Collision means that the player pickup up the coin
    for event in self.events do
        if event.IsCollision() then            
            score = score + 1          
            self.destroy()
            return
        end
    end
    
    -- Rotate the object around y axis
    self.rotation = vec3(0.0, self.handle + t, 0.0)
    ]]
end

function on_world_init()
    -- Populate the world with coins    
    local coinBox = box(vec3(-0.5, -0.5, -0.5), vec3(1.0, 1.0, 1.0))
   
    for i = 0, 10 do    
    
        --[[
        local coin = scene.create("object")  
        coin.model = "coin"
        coin.bbox = coinBox
        coin.position = vec3(i, 0.5, i)
        coin.on_update = OnCoinUpdate        
        ]]
    end
end
