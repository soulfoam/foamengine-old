layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex_coord;
layout (location = 2) in vec3 a_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 tex_coord;
out float fog_amount;

void 
main()
{
	gl_Position = projection * view * model * vec4(a_pos, 1.0f);
	tex_coord = vec2(a_tex_coord.x, 1.0 - a_tex_coord.y);
}
