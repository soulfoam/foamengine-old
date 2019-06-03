#include <stdlib.h>
#include <stdio.h>
#include <core/ext/sugar.h>
#include "convert_skeleton.h"
#include "convert_mesh.h"

int32_t
main(int32_t argc, char **argv)
{

	if (argc < 4)
	{
		printf("Usage convert input output (type) (opt)\n"
				"convert file.fbx newfile skeleton\n"
				"convert file.fbx newfile mesh (this will include every mesh)\n"
				"convert file.fbx newfile mesh cube sphere cone (include mesh one by one)\n");
		exit(1);
	}
	
	u32 mesh_count = argc - 4;

	const char *input_file = argv[1];
	const char *output_file = argv[2];
	const char *type = argv[3];
		
	if (strcmp("mesh", type) == 0)
	{	
		if (mesh_count > 0)
		{
			char **mesh_names = malloc((mesh_count) * sizeof (*mesh_names));

			for (u32 i = 0; i < mesh_count; i++)
			{
				char *name = malloc(strlen(argv[4+i])+1);
				strcpy(name, argv[4+i]);
				mesh_names[i] = name;
			}

			convert_mesh(input_file, mesh_names, mesh_count, output_file);

			for (u32 i = 0; i < mesh_count; i++) free(mesh_names[i]);
			free(mesh_names);
		}
		else
		{
			convert_mesh(input_file, NULL, 0, output_file);
		}
	}
	else if (strcmp("skeleton", type) == 0)
	{	
		convert_skeleton(input_file, output_file);
	}


	exit(0);

}

