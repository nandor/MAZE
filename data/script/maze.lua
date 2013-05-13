-- This file is part of the MAZE project
-- Licensing information can be found in the LICENSE file
-- (C) 2013 The MAZE project. All rights reserved.

local coin_count = 0

function on_world_init()
    for i = 0, 100 do        
        local coin = scene.create("object")
        
        coin.model = "coin"     
        coin.position = vec3(math.random(100), 1.0 + math.random(30) / 10.0, math.random(100))
        coin.box = box(vec3(-0.5, -0.5, -0.5), vec3(1.0, 1.0, 1.0))  
        coin.shadow_caster = true        
        coin.pickable = true     
        
        coin.on_pick = function (ent)
            coin_count = coin_count + 1
            coin:delete()
        end
        
        coin.on_update = function (t, dt)
           coin.rotation = vec3(0.0, t / 15.0 + i, 0.0)
        end
    end
    
    for i = 0, 300 do
        local pillar = scene.create("object")
        
        pillar.model = "pillar"
        pillar.position = vec3(math.random(100), 0, math.random(100))
        pillar.box = box(vec3(-1.0, 0.0, -1.0), vec3(2.0, 3.0, 2.0))  
        pillar.shadow_caster = true        
        pillar.collider = true
        
    end
    
    for i = 0, 10 do
        local light = scene.create("light")
        light.type = "point"
        light.diffuse = vec3(0.0, 1.0, 0.0)
        light.position = vec3(math.random(100), math.random(30) / 10.0, math.random(100))
        light.radius = math.random(100) / 10.0
        light.shadow_caster = false
    end
        
    rock = scene.create("object")
    rock.model = "pillar"
    rock.position = vec3(20.0, 0.0, 20.0)
    rock.box = box(vec3(-1.0, 0.0, -1.0), vec3(2.0, 3.1, 2.0))
    rock.useable  = true
    rock.collider = true
    rock.shadow_caster = true
    rock.use_text = "Press F to pick me up"
    rock.on_use = function ()
        rock:delete()
    end
end

