/* objects.h pknowles 2010-08-26 15:25:16 */

#ifndef OBJECTS_H
#define OBJECTS_H

#ifdef _WIN32
#include <windows.h>
#endif

/* For vertex buffer objects */
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <stdarg.h>

typedef struct {
	float x, y, z;
} vector_t;

typedef struct {
	vector_t vert;
	vector_t norm;
} vertex_t;

typedef struct ObjectType {
	GLuint vertexBuffer;
	GLuint elementBuffer;
  int numVertices;
	int numElements;
} Object;

typedef vertex_t (*ParametricObjFunc)(float, float, va_list*);

vertex_t parametricSphere(float u, float v, va_list* args); /* args: radius */
vertex_t parametricTorus(float u, float v, va_list* args); /* args: inner-radius, outer-radius */
vertex_t parametricWave(float u, float v, va_list* args); /* args: width, height, time */
vertex_t parametricGrid(float u, float v, va_list* args); /* args: N/A */

void drawAxes(float x, float y, float z, float length);

/*
USAGE:
myobject = createObject(<a parametric function from the list above>, <tessellation x>, <tessellation y>, <function arguments (args)>);
*/
Object* createObject(ParametricObjFunc parametric, int x, int y, ...);
void drawObject(Object* obj);
void drawNormals(Object* obj);
void freeObject(Object* obj);

#endif
