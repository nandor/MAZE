# This file is part of the MAZE project
# Licensing information can be found in the LICENSE file
# (C) 2012 The Maze Project. All rights reserved.

import bpy
import bmesh
import struct
import mathutils

def mesh_triangulate(mesh):
    """
        Triangulates the mesh
    """
    
    bm = bmesh.new()
    bm.from_mesh(mesh)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(mesh)
    bm.free()
    
def save(operator, ctx, filepath = ''):
    """
        Saves the active object
    """
    
    # Retrieve the mesh
    obj = ctx.active_object
    if not obj or obj.type != 'MESH':
        raise Exception('A mesh must be selected')

    mesh = obj.to_mesh(ctx.scene, True, 'PREVIEW')
    if not mesh:
        raise Exception('Cannot retrieve mesh')
        
    # Triangulate & compute normals
    mesh_triangulate(mesh)
    mesh.calc_normals()
    mesh.calc_tessface()
    
    # Retrieve UV map
    uv_layer = None    
    if len(mesh.tessface_uv_textures) > 0:
        uv_layer = mesh.tessface_uv_textures.active.data
        
    # Retrieve vertex data
    groups = {}
    materials = {}
    poly_count = 0
    vertex_count = 0
    sphere_radius = 0.0
    box = mathutils.Vector((0, 0, 0))
    
    for key, mat in enumerate(mesh.materials):
        materials[key] = mat
    
    for face_idx, face in enumerate(mesh.tessfaces):
        
        if not face.material_index in groups:
            groups[face.material_index] = []
            
        group = groups[face.material_index]
        normal = face.normal
        
        poly_count += 1
                
        for vert_idx, vert in enumerate(face.vertices):
        
            vertex = mesh.vertices[vert]
            
            u = v = 0.0
            if uv_layer is not None:
                u = uv_layer[face_idx].uv[vert_idx][0]
                v = uv_layer[face_idx].uv[vert_idx][1]
            
            sphere_radius = max(sphere_radius, vertex.co.length)
            box.x = max(box.x, abs(vertex.co.x))
            box.y = max(box.y, abs(vertex.co.y))
            box.z = max(box.z, abs(vertex.co.z))
            
            group.append(struct.pack('<ffffffff', 
                vertex.co.x, vertex.co.y, vertex.co.z,
                normal.x, normal.y, normal.z,
                u, v
            ))
            
            vertex_count += 1
    
    box.x *= 2
    box.y *= 2
    box.z *= 2
    
    file = open(filepath, 'wb')
    
    # Signature
    file.write(b'MZO\0')
    
    # Version information
    file.write(struct.pack('<BBBB', 0, 0, 1, 0))
    
    # Vertex & group count
    file.write(struct.pack('<HHHH',
        len(groups),
        poly_count,
        vertex_count,
        0
    ))        
    
    # Collision data
    file.write(struct.pack('<ffff',
        sphere_radius,
        box.x, box.y, box.z
    ))
    
    # Write vertex groups
    for group_idx, group in groups.items():
        if group_idx not in materials:
            file.write(struct.pack('<fff', 
                0.0, 0.0, 0.0      # Diffuse
            ))
        else:
            mtl = materials[group_idx]
            file.write(struct.pack('<fff',
                mtl.diffuse_color.r, 
                mtl.diffuse_color.g, 
                mtl.diffuse_color.b
            ))
        
        file.write(struct.pack('<I', len(group)))
        for vertex in group:
            file.write(vertex)
    
    file.close()
    bpy.data.meshes.remove(mesh)
    
    return {'FINISHED'}
    