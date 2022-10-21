// diffuse lighting shader
// assumes single directional light

void main(void)
{
	vec4 colour = vec4(0.0);

	// normalized vertex normal
	vec3 normal = normalize(vec3(gl_NormalMatrix * gl_Normal));

	// unit vector in direction of light, light source position/direction
	// already transformed into eye space coordinates by modelview matrix
	vec3 light = normalize(vec3(gl_LightSource[0].position));

	// compute diffuse scalar
	float NdotL = max(dot(normal, light), 0.0);

	// add global and light ambient
	colour += gl_FrontMaterial.ambient * (gl_LightModel.ambient + gl_LightSource[0].ambient);

	// add diffuse component
	if (NdotL > 0.0)
	{
		colour += NdotL * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	}

	// set the colour
	gl_FrontColor = colour;

	// apply matrix transforms to vertex position
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

