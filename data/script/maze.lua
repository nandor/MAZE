-- This file is part of the MAZE project
-- Licensing information can be found in the LICENSE file
-- (C) 2012 The MAZE project. All rights reserved.

local coin_count = 0

function on_world_init()
    for i = 0, 100 do        
        local coin = scene.create("object")      
        
        coin.model = "coin"     
        coin.position = vec3(math.random(100), 1.0, math.random(100))
        coin.box = box(vec3(-0.5, -0.5, -0.5), vec3(1.0, 1.0, 1.0))  
        coin.shadow_caster = true        
        coin.pickable = true     
        
        coin.on_pick = function (ent)
            coin:delete()
        end
        
        coin.on_update = function (t, dt)
           coin.rotation = vec3(0.0, t / 15.0, 0.0)
        end
    end
        
    rock = scene.create("object")
    rock.model = "pillar"
    rock.position = vec3(20.0, 0.0, 20.0)
    rock.box = box(vec3(-2.5, 0.0, -2.5), vec3(5.0, 4.0, 5.0))
    rock.useable  = true
    rock.use_text = "Pick up this huge thing!"
    rock.on_use = function ()
    
    end
end

function on_world_update(t, dt)
end
