-- This file is part of the MAZE project
-- Licensing information can be found in the LICENSE file
-- (C) 2013 The MAZE project. All rights reserved.

-------------------------------------------------------------------------------
local player = nil

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
function spawn_pillar(x, y)
    local pillar = scene.create("object")        
    pillar.model = "pillar"
    pillar.position = vec3(x * 5, 0, y * 5)
    pillar.box = box(vec3(-0.75, 0.0, -0.75), vec3(1.5, 4.0, 1.5))  
    pillar.shadow_caster = true        
    pillar.collider = true  
end

-------------------------------------------------------------------------------
function spawn_box()
	local b = scene.create("object")
	
	b.model = "pillar"
	b.position = vec3(math.random() * 100, 0, math.random() * 100.0)
    b.box = box(vec3(-0.75, 0.0, -0.75), vec3(1.5, 4.0, 1.5))  
    b.shadow_caster = false        
    b.pickable = true  
	
	local dir = math.floor(math.random() * 100) % 4
	b.on_update = function(t, dt)
		local p = b.position
		if dir == 0 then
			p.x = p.x + 0.01
			if p.x >= 99.0 then
				dir = 1
			end
		end
		
		if dir == 1 then
			p.x = p.x - 0.01
			if p.x <= 1.0 then
				dir = 0
			end
		end
		
		if dir == 2 then
			p.z = p.z + 0.01
			if p.z >= 99.0 then
				dir = 3
			end
		end
		
		if dir == 3 then
			p.z = p.z - 0.01
			if p.z <= 1.0 then
				dir = 2
			end
		end
		
		b.position = p
	end
	
	b.on_pick = function()
		player.position = vec3(5, 0, 5)
	end
end

-------------------------------------------------------------------------------
function on_world_init()    
	player = scene.get("player", "player")
            
    -- Place walls
    for x = 0, 20 do
        for y = 0, 20 do         
            local id = math.floor(math.random() * 100) % 5
            
            if (x == 0 or x == 20) and y < 20 then
                spawn_wall(x, y, true)
            end
                        
            if (y == 0 or y == 20) and x < 20 then
                spawn_wall(x, y, false)
            end            
        end        
    end
	
	spawn_pillar(0, 0)
	spawn_pillar(20, 0)
	spawn_pillar(0, 20)
	spawn_pillar(20, 20) 
	
	-- Place moveing stuff
	for x = 0, 500 do
		spawn_box()
	end
end
