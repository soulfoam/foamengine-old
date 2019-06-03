uniform sampler2D fe_tex;
uniform vec4 fe_mdl_color;

in fe_tex_coord;

out vec4 frag_color;

vec4 fe_color;

void 
main()
{

	fe_color = (fe_mdl_color * texture(fe_tex, fe_tex_coord));

	frag_color = fe_color;

	//if (frag_color.a <= 0.0) discard;
}
