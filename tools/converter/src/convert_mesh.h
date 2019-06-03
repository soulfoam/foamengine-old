#pragma once

#include <cglm/cglm.h>
#include <core/ext/io.h>
#include <core/ext/sugar.h>
#include <core/ext/vector.h>
#include <core/ext/logger.h>
#include "conf.h"


//Notes when using Blender, dunno how this would be with other models from other software
//but I'd assume similar, because I think it's something to do with assimp more than the file
//format that is exported.

//Meshes must be aligned in edit mode, because all locations in 
//object mode get set to 0,0,0 at some point in the pipeline. 
//Also scales all must be the same size in object mode. 

//So basically just use edit mode for everything, when resizing or moving to align up parts 
//or whatever.


void convert_mesh(
		const char *input_path, 
		char **mesh_names, u32 mesh_count, 
		const char *output_path);



