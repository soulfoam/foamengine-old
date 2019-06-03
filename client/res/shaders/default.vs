layout (location = 0) in vec3 fe_a_pos;
layout (location = 1) in vec2 fe_a_tex_coord;
layout (location = 2) in vec3 fe_a_norm;
layout (location = 3) in vec3 fe_a_color;

uniform mat4 fe_model;
uniform mat4 fe_view;
uniform mat4 fe_projection;

out vec2 fe_tex_coord;

void 
main()
{	
	gl_Position = fe_projection * fe_view * fe_model * vec4(fe_a_pos, 1.0f);
	fe_tex_coord = vec2(fe_a_tex_coord.x, 1.0 - fe_a_tex_coord.y);
}
