uniform sampler2D tex;
uniform vec4 mesh_color;
uniform vec3 light_color;

in vec2 tex_coord;
in vec3 surface_norm;
in vec3 light_norm;

out vec4 frag_color;

void
main()
{
	float n_dot_l = dot(surface_norm, light_norm);

	float amb = 0.6;
	float brightness = amb + (1.0 - amb) * n_dot_l;

	vec3 diffuse = brightness * light_color;

	//frag_color = (model_color * texture(texture, texture_coord));
	//frag_color = vec4(diffuse, 1.0) * texture(tex, tex_coord) * mesh_color;
	//frag_color = vec4(diffuse, 1.0) * mesh_color;
	frag_color = vec4(vec3(surface_norm) * .5 + .5, 1.0);
	//frag_color = mesh_color;
}

