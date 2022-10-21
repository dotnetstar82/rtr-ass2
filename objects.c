/* objects.c pknowles 2010-08-26 15:25:16 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include "objects.h"

vertex_t parametricSphere(float u, float v, va_list* args)
{
	/* http://mathworld.wolfram.com/Sphere.html */
	float radius;
	float pi = acosf(-1.0f);
	vertex_t ret;

	radius = va_arg(*args, double);
	u *= 2.0f * pi;
	v *= pi;
	ret.norm.x = cos(u) * sin(v);
	ret.norm.y = sin(u) * sin(v);
	ret.norm.z = cos(v);
	ret.vert.x = radius * ret.norm.x;
	ret.vert.y = radius * ret.norm.y;
	ret.vert.z = radius * ret.norm.z;
	return ret;
}

vertex_t parametricTorus(float u, float v, va_list* args)
{
	/* http://mathworld.wolfram.com/Torus.html */
	float R;
	float r;
	float pi = acosf(-1.0f);
	vertex_t ret;

	R = va_arg(*args, double);
	r = va_arg(*args, double);
	u *= 2.0f * pi;
	v *= 2.0f * pi;
	ret.norm.x = cos(u) * cos(v);
	ret.norm.y = sin(u) * cos(v);
	ret.norm.z = sin(v);
	ret.vert.x = (R + r * cos(v)) * cos(u);
	ret.vert.y = (R + r * cos(v)) * sin(u);
	ret.vert.z = r * sin(v);
	return ret;
}

vertex_t parametricWave(float u, float v, va_list* args)
{
	float width, height, time;
	float pi = acosf(-1.0f);
	float phi = pi * 5 * u;
	float theta = pi * 5 * v;
	float x, y, z, m;
	vertex_t ret;
	float amp = .2;// anim = 1;
	width = va_arg(*args, double);
	height = va_arg(*args, double);
	time = va_arg(*args, double);

	//printf("%f\n",time);
		//first vertex start
			z = amp*(sin(theta+time)*sin(phi+time));
			//normals for vertex 1
			x = -amp*cos(theta)*sin(phi);
			y = amp*sin(theta)*cos(phi);
			m = sqrt(x*x+y*y+1);

	ret.norm.x = x / m;
	ret.norm.y = y / m;
	ret.norm.z = 1 / m;
			//end normals for vertex 1
	ret.vert.x = u * width - 1;
	ret.vert.y = v * height -1;
	ret.vert.z = z;


	return ret;
}

vertex_t parametricGrid(float u, float v, va_list *args) {
	vertex_t ret;

	ret.vert.x = u;
	ret.vert.y = v;

	return ret;
}

void drawAxes(float x,float y,float z,float length)
{
	glDisable(GL_DEPTH_TEST);
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(x,y, z);
		glVertex3f(x+length,y, z);
		glColor3f(0, 1, 0);
		glVertex3f(x,y, z);
		glVertex3f(x,y+length, z);
		glColor3f(0, 0, 1);
		glVertex3f(x,y, z);
		glVertex3f(x,y, z+length);
	glEnd();

	glPopAttrib();
	glEnable(GL_DEPTH_TEST);
}

Object* createObject(ParametricObjFunc paramObjFunc, int x, int y, ...)
{
	va_list args;
	unsigned int i, j;
	float u, v;
	int ci = 0; /* current index */
	vertex_t* vertices;
	unsigned int* indices;
	int numVertices;
	int numIndices;
	Object* obj;
#define INDEX(I, J) ((I)*y + (J))

	/* Initialize data */
	numVertices = x * y;
	numIndices = (y-1) * (x * 2 + 2);
	vertices = (vertex_t*)malloc(sizeof(vertex_t) * numVertices);
	indices = (unsigned int*)malloc(sizeof(unsigned int) * numIndices);

	/* Construct vertex data */
	for (i = 0; i < x; ++i)
	{
		u = i/(float)(x-1);
		for (j = 0; j < y; ++j)
		{
			v = j/(float)(y-1);
			va_start(args, y);
			vertices[INDEX(i, j)] = paramObjFunc(u, v, &args);
		}
	}

	/* Construct index data */
	for (j = 0; j < y-1; ++j)
	{
		indices[ci++] = INDEX(0, j);
		for (i = 0; i < x; ++i)
		{
			indices[ci++] = INDEX(i, j);
			indices[ci++] = INDEX(i, j+1);
		}
		indices[ci++] = INDEX(i-1, j+1);
	}

	/* Double check the loops populated the data correctly */
	assert(ci == numIndices);

	/* Create VBOs */
	obj = (Object*)malloc(sizeof(Object));
	glGenBuffers(1, &obj->vertexBuffer);
	glGenBuffers(1, &obj->elementBuffer);

	/* Buffer the vertex data */
	glBindBuffer(GL_ARRAY_BUFFER, obj->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * numVertices, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Buffer the index data */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/* Cleanup and return the object struct */
	obj->numVertices = numVertices;
	obj->numElements = numIndices;
	free(vertices);
	free(indices);
	return obj;
}

void drawObject(Object* obj)
{
	/* Enable vertex arrays and bind VBOs */
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, obj->vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->elementBuffer);

	/* Draw object */
	glVertexPointer(3, GL_FLOAT, sizeof(vertex_t), (void*)0);
	glNormalPointer(GL_FLOAT, sizeof(vertex_t), (void*)sizeof(vector_t));
	glDrawElements(GL_TRIANGLE_STRIP, obj->numElements, GL_UNSIGNED_INT, (void*)0);

	/* Unbind/disable arrays. could also push/pop enables */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void drawNormals(Object* obj)
{
	/* Enable vertex arrays and bind VBOs */
	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, obj->vertexBuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->elementBuffer);

	/* Draw object */
	glVertexPointer(3, GL_FLOAT, 0, (void *) 0);
	//glNormalPointer(GL_FLOAT, sizeof(vertex_t), (void*)sizeof(vector_t));
	//glDrawArray(GL_LINES, obj->numVertices * 2, GL_UNSIGNED_INT, (void*)0);
	glDrawArrays(GL_LINES, 0, obj->numVertices * 2);

	/* Unbind/disable arrays. could also push/pop enables */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
}

void freeObject(Object* obj)
{
	glDeleteBuffers(1, &obj->vertexBuffer);
	glDeleteBuffers(1, &obj->elementBuffer);
	obj->vertexBuffer = 0;
	obj->elementBuffer = 0;
	obj->numElements = 0;
}

