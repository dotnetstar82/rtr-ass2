// vertex shader for per-pixel lighting
// assumes single directional light

#define M_PI 3.1415926535897932384626433832795

varying vec3 eye;
varying vec3 normal;

/* objects:
 *  0 = torus
 *  1 = wave
 */
uniform int object;

/* lighting model:
 *  0 = phong
 *  1 = blinn-phong
 */
uniform int lightingModel;

/* light type:
 *  0 = point
 *  1 = directional
 */
uniform bool lightType;

uniform bool isLocalViewer;
uniform bool isPerPixelLighting;

uniform float time;

void main(void) {

	const int Torus = 0;
	const int Wave  = 1;

	const int Phong = 0;
	const int BlinnPhong = 1;

	vec4 vertex;

	if (object == Torus) {

		const float R = 1.0;
		const float r = 0.5;

		float u = gl_Vertex.x * 2.0 * M_PI;
		float v = gl_Vertex.y * 2.0 * M_PI;

		normal = vec3(
				cos(u) * cos(v),
				sin(u) * cos(v),
				sin(v));

		vertex = vec4(
				(R + r * cos(v)) * cos(u),
				(R + r * cos(v)) * sin(u),
				r * sin(v),
				1);

	} else /* object == Wave */ {

		const float Width     = 2.0;
		const float Height    = 2.0;
		const float Amplitude = 0.2;
		const float Frequency = 5.0;

		float u = gl_Vertex.x;
		float v = gl_Vertex.y;

		float phi = M_PI * Frequency * u;
		float theta = M_PI * Frequency * v;

		float x = -Amplitude * cos(theta) * sin(phi);
		float y =  Amplitude * sin(theta) * cos(phi);
		float z =  Amplitude * sin(theta + time) * sin (phi +time);
		float m = sqrt(x * x + y * y + 1.0);

		normal = vec3(
				x / m,
				y / m,
				1 / m);

		vertex = vec4(
				(u - 0.5) * Width,
				(v - 0.5) * Height,
				z,
				1);
	}

	// set eye and normal vectors
	eye = isLocalViewer ? normalize(vec3(gl_ModelViewMatrix * vertex)) : vec3(0.0, 0.0, -1.0);
	normal = normalize(vec3(gl_NormalMatrix * normal));

	// if vertex lit, set vertex color
	if (!isPerPixelLighting) {

		vec4 color = vec4(0.0);

		// unit vector in direction of light, light source position/direction
		// already transformed into eye space coordinates by modelview matrix
		vec3 light = normalize(vec3(gl_LightSource[0].position));

		// compute diffuse scalar
		float NdotL = max(dot(normal, light), 0.0);

		// add global and light ambient
		color += gl_FrontMaterial.ambient * (gl_LightModel.ambient + gl_LightSource[0].ambient);

		if (NdotL > 0.0) {
			// add diffuse component
			color += NdotL * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;

			// add specular color depending on light model
			if (lightingModel == Phong) {

				// calculate reflection vector
				vec3 reflection = reflect(light, normal);
				float RdotE = max(dot(reflection, eye), 0.0);

				color += pow(RdotE, gl_FrontMaterial.shininess) *
					gl_LightSource[0].specular * gl_FrontMaterial.specular;

			} else /* lightingModel == BlinnPhong */ {

				float NdotHV = max(dot(normal, gl_LightSource[0].halfVector.xyz), 0.0);
				color += gl_FrontMaterial.specular * gl_LightSource[0].specular *
					pow(NdotHV, gl_FrontMaterial.shininess);

			}
		}

		// set the color
		gl_FrontColor = color;
	}

	// apply matrix transforms to vertex position
	gl_Position = gl_ModelViewProjectionMatrix * vertex;
}
