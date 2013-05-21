-- This file is part of the MAZE project
-- Licensing information can be found in the LICENSE file
-- (C) 2013 The MAZE project. All rights reserved.

local coin_count = 0

function on_world_init()    

    -- Place pillars on corners
    for x = 0, 20 do
        for y = 0, 20 do
            local pillar = scene.create("object")        
            pillar.model = "pillar"
            pillar.position = vec3(x * 5, 0, y * 5)
            pillar.box = box(vec3(-0.5, 0.0, -0.5), vec3(1.0, 4.0, 1.0))  
            pillar.shadow_caster = true        
            pillar.collider = true    
        end
    end
    
    -- Place walls
    for x = 0, 20 do
        for y = 0, 20 do 
        
            local id = math.floor(math.random() * 100) % 3
            
            if (x == 0 or x == 20 or id == 0) and y < 20 then
                local wall = scene.create("object")
                wall.model = "wall"
                wall.shadow_caster = true
                wall.collider = true

                wall.position = vec3(x * 5, 0.0, y * 5 + 2.5)
                wall.rotation = vec3(0.0, 90.0, 0.0)
                wall.box = box(vec3(-0.5, 0.0, -2.5), vec3(1.0, 4.0, 5.0))
            end
            
            if (y == 0 or y == 20 or id == 0) and x < 20 then
                local wall = scene.create("object")
                wall.model = "wall"
                wall.shadow_caster = true
                wall.collider = true

                wall.position = vec3(x * 5 + 2.5, 0.0, y * 5)
                wall.rotation = vec3(0.0, 0.0, 0.0)
                wall.box = box(vec3(-2.5, 0.0, -0.5), vec3(5.0, 4.0, 1.0))
            end
        end
    end

    -- Place powerups
    for x = 0, 19 do
        for y = 0, 19 do
            
            local id = math.floor(math.random() * 100) % 3
        
            if id == 1 then
                local coin = scene.create("object")
                
                coin.model = "coin"     
                coin.position = vec3(x * 5 + 2.5, 1.0, y * 5 + 2.5)
                coin.box = box(vec3(-0.5, -0.5, -0.5), vec3(1.0, 1.0, 1.0))  
                coin.shadow_caster = true        
                coin.pickable = true     
                
                coin.on_pick = function (ent)
                    coin_count = coin_count + 1
                    coin:delete()
                end
                
                coin.on_update = function (t, dt)
                   coin.rotation = vec3(0.0, t / 15.0 + (x * 20 + y), 0.0)
                end
            end
        end
    end
    
    door = scene.create("object")
    door.model = "gate"
    door.position = vec3(10.0, 0.0, 10.0)
    door.box = box(vec3(-2.0, 0.0, 0.0), vec3(2.0, 2.0, 0.2))
    door.useable  = true
    door.collider = true
    door.shadow_caster = true
    door.use_text = "Press F to open"
    
    door_open = false
    
    door.on_use = function ()
        if door_open then
            door.rotation = vec3(0.0, 0.0, 0.0)
            door.box = box(vec3(-2.0, 0.0, 0.0), vec3(2.0, 2.0, 0.2))
            door_open = false
        else
            door.rotation = vec3(0.0, 90.0, 0.0)
            door.box = box(vec3(0.0, 0.0, 0.0), vec3(0.2, 2.0, 2.0))
            door_open = true
        end
    end
end

