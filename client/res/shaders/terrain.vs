layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex_coord;
layout (location = 2) in vec3 a_norm;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 light_pos;

out vec2 tex_coord;
out vec3 surface_norm;
out vec3 light_norm;

void 
main()
{	
	gl_Position = projection * view * model * vec4(a_pos, 1.0f);

	tex_coord = vec2(a_tex_coord.x, 1.0 - a_tex_coord.y);

	vec4 world_pos = model * vec4(a_pos, 1.0);

	surface_norm = normalize((model * vec4(a_norm, 0.0)).xyz);
	light_norm = normalize(light_pos - world_pos.xyz);

}
