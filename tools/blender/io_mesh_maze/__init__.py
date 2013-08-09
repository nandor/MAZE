# This file is part of the MAZE project
# Licensing information can be found in the LICENSE file
# (C) 2012 The Maze Project. All rights reserved.
        
# Package information
bl_info = {
    "name": "MAZE model format",
    "author": "Licker Nandor",
    "blender": (2, 68, 0),
	"version": (0, 0, 2),
    "location": "File > Import-Export",
    "description": "MAZE model exporter with materials and collision mesh",
    "category": "Import-Export"
}

# Reload support
if "bpy" in locals():
    import imp
        
    if "export_mzo" in locals():
        imp.reload(export_mzo)
                
import bpy
from bpy.props import BoolProperty
from bpy_extras.io_utils import ExportHelper

class ExportMAZE(bpy.types.Operator, ExportHelper):
    """
        Exports a MAZE model
    """
    
    bl_idname = "export_mesh.mzo"
    bl_label = "Export MAZE"    
    filename_ext = ".mzo"	
    
    use_skeleton = BoolProperty(
        name = "Export Skeleton",
        description = "Writes bone hierarchy",
        default = True
    )
		
    use_collision = BoolProperty(
        name = "Export Collision Mesh",
        description = "Writes the collision mesh to the file",
        default = True
    )

    def execute(self, context):
        """
            Executes the model export
        """
        
        filepath = bpy.path.ensure_ext(self.filepath, self.filename_ext)
        keywords = self.as_keywords(ignore = ("check_existing", "filter_glob", "export"))   
                
        from . import export_mzo
        return export_mzo.save(self, context, **keywords)
    
def menu_func_export(self, context):
    self.layout.operator(ExportMAZE.bl_idname, text = "MAZE (.mzo)")
    
def register():    
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func_export)

def unregister():    
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
    register()