// phong-pixel.frag

varying vec3 eye;
varying vec3 normal;

void main (void)
{
	vec4 color = vec4(0.0);

	// already transformed into eye space coordinates by modelview matrix
	vec3 light = normalize(vec3(gl_LightSource[0].position));

	// compute diffuse scalar
	float NdotL = max(dot(normal, light), 0.0);

	// add global and light ambient
	color += gl_FrontMaterial.ambient * (gl_LightModel.ambient + gl_LightSource[0].ambient);

	if (NdotL > 0.0)
	{
		// add diffuse component
		color += NdotL * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;

		// calculate reflection vector
		vec3 reflection = reflect(light, normal);
		float RdotE = max(dot(reflection, eye), 0.0);

		// add specular color
		color += pow(RdotE, gl_FrontMaterial.shininess) *
			gl_LightSource[0].specular * gl_FrontMaterial.specular;
	}

	gl_FragColor = color;
}
