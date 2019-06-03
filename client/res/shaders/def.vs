layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex_coord;
layout (location = 2) in vec3 a_norm;
#ifdef INSTANCING
layout (location = 3) in mat4 a_matrix;
#endif

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 light_pos;

uniform float num_of_tex_rows = 1;
uniform vec2 tex_offset = {0, 0};

out vec2 tex_coord;
out vec3 surface_norm;
out vec3 light_norm;

void 
main()
{	
	mat4 model_p = model;
#ifdef INSTANCING
	model_p = a_matrix;
#endif

	gl_Position = projection * view * model_p * vec4(a_pos, 1.0f);

	tex_coord = vec2(a_tex_coord.x, 1.0 - a_tex_coord.y);
	tex_coord = (tex_coord / num_of_tex_rows) + tex_offset;

	vec4 world_pos = model_p * vec4(a_pos, 1.0);

	surface_norm = normalize((model_p * vec4(a_norm, 0.0)).xyz);
	light_norm = normalize(light_pos - world_pos.xyz);
}
