layout (location = 0) in vec4 vert;

uniform mat4 model;
uniform mat4 projection;

out vec2 tex_coord;

void
main()
{
	tex_coord = vert.zw;
	gl_Position = projection * model * vec4(vert.xy, 0.0, 1.0);
}
