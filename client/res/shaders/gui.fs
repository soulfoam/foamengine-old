uniform sampler2D tex;
uniform vec4 color;

in vec2 tex_coord;

out vec4 frag_color;

void 
main()
{
	frag_color = color * texture(tex, tex_coord);
	if (frag_color.a <= 0.0) discard;
}
