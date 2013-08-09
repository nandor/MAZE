# This file is part of the MAZE project
# Licensing information can be found in the LICENSE file
# (C) 2012 The Maze Project. All rights reserved.

import os
import bpy
import bmesh
from struct import pack
from bpy_extras.io_utils import axis_conversion

FLAGS_VERTICES  = 1 << 0
FLAGS_COLLISION = 1 << 2
FLAGS_SKELETON  = 1 << 3
       
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
    
def save(operator, ctx, filepath = '', 
         use_skeleton = True, 
         use_collision = True):
    """
        Saves the active object to a .mzo file
    """
    
    # Data to be saved
    flags       = 0
    verts       = []
    indices     = []
    phys        = []
    skeleton    = []
    
    # Retrieve the object
    object = ctx.active_object
    if not object:
        raise Exception('No objects selected!')
    
    # Read vertices, indices and bone weights
    mesh = object.to_mesh(ctx.scene, True, 'PREVIEW')
    mesh_prepare(mesh)        
    
    # Retrieve normals, positions and weights
    for vert in mesh.vertices:
        vertex = {}
        
        # Retrieve vertex position
        vertex["x"] = vert.co.x
        vertex["y"] = vert.co.y
        vertex["z"] = vert.co.z
        
        # Retrieve normals
        vertex["nx"] = vert.normal.x
        vertex["ny"] = vert.normal.y
        vertex["nz"] = vert.normal.z
            
        # Retrieve bone weights
        weights = []
        for group in vert.groups:
            weights.append({
                "weight": group.weight,
                "group": group.group
            })
            
        if len(weights) > 4:
            print (len(weights))
            weights = sorted(weights, key = lambda w : w["weight"], reverse = True)
            weights = weights[:3]
        
        for i in range(0, len(weights)):
            vertex["bi" + str(i)] = weights[i]["group"]
            vertex["bw" + str(i)] = weights[i]["weight"]
            
        for i in range(len(weights), 4):
            vertex["bi" + str(i)] = 0
            vertex["bw" + str(i)] = 0
            
        verts.append(vertex)
            
    # Retrieve texture coordinates & indices      
    if len(mesh.tessface_uv_textures) != 1:
        raise Exception('Invalid UV map')
        
    uv_layer = mesh.tessface_uv_textures.active.data    
    for face_idx, face in enumerate(mesh.tessfaces):
        for vert_idx, vert in enumerate(face.vertices):
            verts[vert]["u"] = uv_layer[face_idx].uv[vert_idx][0]
            verts[vert]["v"] = 1.0 - uv_layer[face_idx].uv[vert_idx][1]
            
            indices.append(vert_idx)
            
    bpy.data.meshes.remove(mesh)
    
    # Read the collision mesh
    if object.name + "_phys" in bpy.data.objects and use_collision:
        flags = flags | FLAGS_COLLISION
        
        cobj = bpy.data.objects[object.name + "_phys"]
        cmesh = cobj.to_mesh(ctx.scene, True, 'PREVIEW')            
        mesh_prepare(cmesh)
        
        for face_idx, face in enumerate(cmesh.tessfaces):            
            for vert_idx, vert in enumerate(face.vertices):
                cvertices.append({
                    "x": cmesh.vertices[vert].co.x, 
                    "y": cmesh.vertices[vert].co.y, 
                    "z": cmesh.vertices[vert].co.z
                })  
        
        bpy.data.meshes.remove(cmesh)
    
    # Read information about the skeleton
    if object.name + "_skel" in bpy.data.armatures and use_skeleton:
        flags = flags | FLAGS_SKELETON
        
    # Write the file
    file = open(filepath, 'wb')
    
    file.write(b'MZO\0')
    file.write(pack('<BBBB', 0, 0, 1, 0))
    file.write(pack('<I', flags))
    file.write(pack('<HHHH', len(verts), len(indices), len(phys), len(skeleton)))
    file.write(pack('<HHHH', 0, 0, 0, 0))
    
    for vert in verts:
        file.write(pack('<ffffffff', vert["x"],  vert["y"],  vert["z"],
                                     vert["nx"], vert["ny"], vert["nz"],
                                     vert["u"],  vert["v"]))
    if use_skeleton:
        for vert in verts:
            file.write(pack('<HfHfHfHf', vert["bi0"], vert["bw0"],
                                         vert["bi1"], vert["bw1"],
                                         vert["bi2"], vert["bw2"],
                                         vert["bi3"], vert["bw3"]))
                                
    for index in indices:
        file.write(pack('<H', index))
    
    for vert in phys:
        file.write(pack('<fff', vert["x"], vert["y"], vert["z"]))
    
    file.close()
    
    return {'FINISHED'}