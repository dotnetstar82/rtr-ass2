// phong vertex shader
// assumes single directional light

void main(void)
{
	vec4 color = vec4(0.0);

	// normalized vertex normal
	vec3 normal = normalize(vec3(gl_NormalMatrix * gl_Normal));

	// unit vector in direction of light, light source position/direction
	// already transformed into eye space coordinates by modelview matrix
	vec3 light = normalize(vec3(gl_LightSource[0].position));

	// compute diffuse scalar
	float NdotL = max(dot(normal, light), 0.0);

	// add global and light ambient
	color += gl_FrontMaterial.ambient * (gl_LightModel.ambient + gl_LightSource[0].ambient);

	// add diffuse component
	if (NdotL > 0.0)
	{
		color += NdotL * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;

		// unit vector from eye to vertex
		vec3 eye = normalize(vec3(gl_ModelViewMatrix * gl_Vertex));

		// calculate reflection vector
		vec3 reflection = reflect(light, normal);
		float RdotE = max(dot(reflection, eye), 0.0);

		color += pow(RdotE, gl_FrontMaterial.shininess) *
			gl_LightSource[0].specular * gl_FrontMaterial.specular;
	}

	// set the color
	gl_FrontColor = color;

	// apply matrix transforms to vertex position
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

