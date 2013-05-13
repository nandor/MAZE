# This file is part of the MAZE project
# Licensing information can be found in the LICENSE file
# (C) 2012 The Maze Project. All rights reserved.

import os
import struct
import bpy
import bmesh
from mathutils import Vector
from bpy_extras.io_utils import axis_conversion
from bpy.types import ImageTexture

def mesh_prepare(mesh):
    """
        Triangulates the mesh
    """
    
    # Rotate to our coordinate system
    mesh.transform(axis_conversion(
        to_up = "Y",
        to_forward = "Z",
    ).to_4x4())
    
    # Triangulate
    bm = bmesh.new()
    bm.from_mesh(mesh)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(mesh)
    bm.free()
    
    # Compute faces & normals
    mesh.calc_normals()
    mesh.calc_tessface()
    
        
def save(operator, ctx, filepath = ''):
    """
        Saves the active object
    """
    
    # Validate the mesh
    object = ctx.active_object
    if not object or not object.name:
        raise Exception('No mesh selected!')
        
    mesh = object.to_mesh(ctx.scene, True, 'PREVIEW')
    if not mesh:
        raise Exception('Cannot retrieve mesh')
            
    if len(mesh.materials) != 1:
        raise Exception('Too many or missing materials')
        
    if len(mesh.tessface_uv_textures) != 1:
        raise Exception('Too many or missing uv layers')
        
    mesh_prepare(mesh)
    
    vertices = []
    cvertices = []
    boxMin = [0, 0, 0]
    boxMax = [0, 0, 0]
    uv_layer = mesh.tessface_uv_textures.active.data
    
    # Retrieve mesh data
    for face_idx, face in enumerate(mesh.tessfaces):
        for vert_idx, vert in enumerate(face.vertices):
        
            vertex = mesh.vertices[vert]
            
            boxMin[0] = min(boxMin[0], vertex.co.x)
            boxMin[1] = min(boxMin[1], vertex.co.y)
            boxMin[2] = min(boxMin[2], vertex.co.z)
            
            boxMax[0] = max(boxMax[0], vertex.co.x)
            boxMax[1] = max(boxMax[1], vertex.co.y)
            boxMax[2] = max(boxMax[2], vertex.co.z)
            
            vertices.append(struct.pack('<ffffffff', 
                vertex.co.x, vertex.co.y, vertex.co.z,
                face.normal.x, face.normal.y, face.normal.z,
                uv_layer[face_idx].uv[vert_idx][0],
                1.0 - uv_layer[face_idx].uv[vert_idx][1]
            ))
            
    bpy.data.meshes.remove(mesh)
               
    # Retrieve the collision mesh
    if object.name + "_phys" in bpy.data.objects:
        cobj = bpy.data.objects[object.name + "_phys"]
        cmesh = cobj.to_mesh(ctx.scene, True, 'PREVIEW')
        
        if not cmesh:
            raise Exception('Cannot retrieve collision mesh')
            
        mesh_prepare(cmesh)
        for face_idx, face in enumerate(cmesh.tessfaces):
                    
            for vert_idx, vert in enumerate(face.vertices):
            
                vertex = cmesh.vertices[vert]
                
                cvertices.append(struct.pack('<fff', 
                    vertex.co.x, 
                    vertex.co.y, 
                    vertex.co.z
                ))  
        
        bpy.data.meshes.remove(cmesh)
                
    file = open(filepath, 'wb')
    
    # Write the file header
    file.write(b'MZO\0')
    file.write(struct.pack('<BBBB', 0, 0, 1, 0))
    file.write(struct.pack('<HH', len(vertices), int(len(cvertices))))
    file.write(struct.pack('<ffffff',
        boxMin[0], boxMin[1], boxMin[2],
        boxMax[0], boxMax[1], boxMax[2]
    ))
    
    for vert in vertices:
        file.write(vert)
        
    for cvert in cvertices:
        file.write(cvert)
    
    file.close()
    
    return {'FINISHED'}