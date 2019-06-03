uniform sampler2DArray tex;
uniform int tex_layer; 
uniform vec4 color;

in vec2 tex_coord;

out vec4 frag_color;

void 
main()
{
	frag_color = color * texture(tex, vec3(tex_coord, tex_layer));
	if (frag_color.a <= 0.0) discard;
}
