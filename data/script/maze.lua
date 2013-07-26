-- This file is part of the MAZE project
-- Licensing information can be found in the LICENSE file
-- (C) 2013 The MAZE project. All rights reserved.

-------------------------------------------------------------------------------
local light_count = 0
local object_count = 0
local finish_spawned = false
local keys_spawned = 0
local player = nil

-------------------------------------------------------------------------------
function spawn_pillar(x, y)
    local pillar = scene.create("object")        
    pillar.model = "pillar"
    pillar.position = vec3(x * 5, 0, y * 5)
    pillar.box = box(vec3(-0.75, 0.0, -0.75), vec3(1.5, 4.0, 1.5))  
    pillar.shadow_caster = true        
    pillar.collider = true  
    
    object_count = object_count + 1
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
    
    object_count = object_count + 1
end

-------------------------------------------------------------------------------
function spawn_door(x, y, o)
    local door = scene.create("object")
    door.model = "gate"
    door.useable  = true
    door.collider = true
    door.shadow_caster = true
    door.use_text = "Open"
    
    if math.floor(math.random() * 10) % 2 == 1 then
        local light = scene.create("light")
        light.type = "point"
        light.diffuse = vec3(0.88, 0.54, 0.13)
        light.specular = vec3(0.88, 0.34, 0.13)
        light.radius = 5.0
        
        if o then
            light.position = vec3(x * 5 - 0.5, 2.0, y * 5 + 2.5)
        else
            light.position = vec3(x * 5 + 2.5, 2.0, y * 5 - 0.5)
        end
        
        light_count = light_count + 1
    end
    
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
    
    object_count = object_count + 2
end

-------------------------------------------------------------------------------
function spawn_finish(x, y)
    if finish_spawned then
        return
    end
    
    finish_spawned = true

    local finish = scene.create("object")
    finish.model = "finish"
    finish.shadow_caster = true
    finish.collider = true
    finish.box = box(vec3(-0.4, 0.0, -0.4), vec3(1.0, 1.0, 1.0))
    finish.position = vec3(x * 5.0 + 2.5, 0.0, y * 5.0 + 2.5)
    finish.useable = true
    finish.use_text = "Next Level"
    finish.on_use = function ()
        scene.delete()
        on_world_init()
        player.position = vec3(2.5, 3.0, 2.5)
        player.keys = 0
    end
    
    local light = scene.create("light")
    light.type = "spot"
    light.diffuse = vec3(1.0, 1.0, 1.0)
    light.specular = vec3(0.88, 0.34, 0.13)
    light.direction = vec3(0.0, -1.0, 0.0)
    light.radius = 5.0
    light.position = vec3(x * 5 + 2.5, 3.25, y * 5 + 2.5)
end

-------------------------------------------------------------------------------
function on_world_init()    
	player = scene.get("player", "player")
    
    finish_spawned = false
    object_count = 0
    light_count = 0
    
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
            
            object_count = object_count + 1
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
                    player.coins = player.coins + 1
					player:play_sound('coin')                    
                    coin:delete()
                    
                    if player.coins >= (keys_spawned + 1) * 10 then
                        local kx = math.floor(math.random() * 100) % 20
                        local ky = math.floor(math.random() * 100) % 20  
                        
                        local key = scene.create("object")
                        local light = scene.create("light")
                        
                        keys_spawned = keys_spawned + 1
                        
                        key.model = "key"
                        key.position = vec3(kx * 5 + 2.5, 1.0, ky * 5 + 2.5)
                        key.box = box(vec3(-0.25, -0.25, -0.25), vec3(0.5, 0.5, 0.5))
                        key.shadow_caster = true
                        key.pickable = true
                                
                        light.type = "point"
                        light.diffuse = vec3(1.0, 1.0, 1.0)
                        light.specular = vec3(0.88, 0.34, 0.13)
                        light.direction = vec3(0.0, -1.0, 0.0)
                        light.radius = 2.0
                        light.position = vec3(kx * 5 + 2.75, 1.25, ky * 5 + 2.75)
                        
                        key.on_pick = function (ent)
                            player.keys = player.keys + 1
                            player:play_sound('coin')
                            key:delete()
                            light:delete()
                            
                            if player.keys >= 3 then
                                spawn_finish(
                                    math.floor(math.random() * 100) % 20,
                                    math.floor(math.random() * 100) % 20 
                                )
                            end
                        end
                        
                        key.on_update = function (t, dt)
                           key.rotation = vec3(0.0, t / 15.0 + (x * 20 + y), 0.0)
                        end
                    end
                end
                
                coin.on_update = function (t, dt)
                   coin.rotation = vec3(0.0, t / 15.0 + (x * 20 + y), 0.0)
                end
            end
        end
    end
	
    -- Place pillars on corners
    for x = 0, 20 do
        for y = 0, 20 do
            spawn_pillar(x, y)  
        end
    end
    
    -- Print statistics
    print("Object count: " .. object_count)
    print("Light count: " .. light_count)
end

