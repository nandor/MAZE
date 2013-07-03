-- This file is part of the MAZE project
-- Licensing information can be found in the LICENSE file
-- (C) 2013 The MAZE project. All rights reserved.

-------------------------------------------------------------------------------
local player = nil

-------------------------------------------------------------------------------
function spawn_pillar(x, y)
    local pillar = scene.create("object")        
    pillar.model = "pillar"
    pillar.position = vec3(x * 5, 0, y * 5)
    pillar.box = box(vec3(-0.75, 0.0, -0.75), vec3(1.5, 4.0, 1.5))  
    pillar.shadow_caster = true        
    pillar.collider = true  
end

-------------------------------------------------------------------------------
function spawn_wall(x, y, o)
    local wall = scene.create("object")
    wall.model = "wall"
    wall.shadow_caster = true
    wall.collider = true

    if o then
        wall.position = vec3(x * 5, 0.0, y * 5 + 2.5)
        wall.rotation = vec3(0.0, 90.0, 0.0)
        wall.box = box(vec3(-0.5, 0.0, -2.5), vec3(1.0, 4.0, 5.0))
    else
        wall.position = vec3(x * 5 + 2.5, 0.0, y * 5)
        wall.rotation = vec3(0.0, 0.0, 0.0)
        wall.box = box(vec3(-2.5, 0.0, -0.5), vec3(5.0, 4.0, 1.0))
    end
end

-------------------------------------------------------------------------------
function spawn_door(x, y, o)
    local door = scene.create("object")
    door.model = "gate"
    door.useable  = true
    door.collider = true
    door.shadow_caster = true
    door.use_text = "Open"
    
    local door_open = false
    
    if o then
        door.position = vec3(x * 5, 0.0, y * 5 + 1.5)
        door.rotation = vec3(0.0, 90.0, 0.0)
        door.box = box(vec3(-0.2, 0.0, 0.0), vec3(0.4, 2.3, 2.0))
    else
        door.position = vec3(x * 5 + 3.5, 0.0, y * 5)
        door.rotation = vec3(0.0, 0.0, 0.0)
        door.box = box(vec3(-2.0, 0.0, -0.2), vec3(2.0, 2.3, 0.4))
    end
    
    door.on_use = function ()
        door.useable = false
        door.collider = false
        
        door:play_sound("gate")
        
        if o then
            door.position = vec3(x * 5, 0.0, y * 5 + 1.5)
            door.rotation = vec3(0.0, 0.0, 0.0)
            door.box = box(vec3(-2.0, 0.0, -0.2), vec3(2.0, 2.0, 0.4))
        else
            door.position = vec3(x * 5 + 3.5, 0.0, y * 5)
            door.rotation = vec3(0.0, 90.0, 0.0)
            door.box = box(vec3(-0.2, 0.0, 0.0), vec3(0.4, 2.0, 2.0))
        end
        
    end    
    
    local wall = scene.create("object")
    wall.model = "wall_gate"
    wall.shadow_caster = true
    wall.collider = true

    if o then
        wall.position = vec3(x * 5, 0.0, y * 5 + 2.5)
        wall.rotation = vec3(0.0, 90.0, 0.0)
        wall.box = box(vec3(-1.0, 0.0, -2.5), vec3(2.0, 4.0, 5.0))
    else
        wall.position = vec3(x * 5 + 2.5, 0.0, y * 5)
        wall.rotation = vec3(0.0, 0.0, 0.0)
        wall.box = box(vec3(-2.5, 0.0, -1.0), vec3(5.0, 4.0, 2.0))
    end
end

-------------------------------------------------------------------------------
function on_world_init()    
	player = scene.get("player", "player")
	
    -- Place pillars on corners
    for x = 0, 20 do
        for y = 0, 20 do
            spawn_pillar(x, y)  
        end
    end
    
    -- Place walls
    for x = 0, 20 do
        for y = 0, 20 do         
            local id = math.floor(math.random() * 100) % 5
            
            if (x == 0 or x == 20 or id == 0) and y < 20 then
                spawn_wall(x, y, true)
            end
                        
            if (y == 0 or y == 20 or id == 0) and x < 20 then
                spawn_wall(x, y, false)
            end
            
            if (x ~= 0 and x ~= 20 and id == 1) and y < 20 then 
                spawn_door(x, y, true)
            end
            
            if (y ~= 0 and y ~= 20 and id == 1) and x < 20 then 
                spawn_door(x, y, false)
            end
        end
    end
				
    -- Place powerups
	local count = 0
    for x = 0, 19 do
        for y = 0, 19 do
            
            local id = math.floor(math.random() * 100) % 3
        
            if id == 1 then
                local coin = scene.create("object", "coin" .. count)
				count = count + 1
                
                coin.model = "coin"     
                coin.position = vec3(x * 5 + 2.5, 1.0, y * 5 + 2.5)
                coin.box = box(vec3(-0.5, -0.5, -0.5), vec3(1.0, 1.0, 1.0))  
                coin.shadow_caster = true        
                coin.pickable = true     
                
                coin.on_pick = function (ent)
					player:add_coin()
					player:play_sound('coin')
                    coin:delete()
                end
                
                coin.on_update = function (t, dt)
                   coin.rotation = vec3(0.0, t / 15.0 + (x * 20 + y), 0.0)
                end
            end
        end
    end
end

