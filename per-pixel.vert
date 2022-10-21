// vertex shader for per-pixel lighting
// assumes single directional light

varying vec3 eye;
varying vec3 normal;

void main(void)
{
	// pass position and normal to fragment shaders
	eye = normalize(vec3(gl_ModelViewMatrix * gl_Vertex));
	normal = normalize(vec3(gl_NormalMatrix * gl_Normal));

	// apply matrix transforms to vertex position
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
