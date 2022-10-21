/* pknowles Wed 18 Aug 2010 09:21:32 PM EST */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#ifdef _WIN32
#pragma warning(disable:4996)
#include <windows.h>
#endif

#include "shaders.h"

int oglError(int line, const char* file)
{
	GLenum glErr;
	int retCode = 0;
	
	/* extract file name from path */
	const char* p = strrchr(file, '\\');
	if (p != NULL) 
		file = p+1;

	/* Get all opengl errors */
	while ((glErr = glGetError()) != GL_NO_ERROR)
	{
		/* Print each error message */
		printf("glError 0x%x in %s at %i: %s\n", glErr, file, line, gluErrorString(glErr));
		retCode = 1;
	}
	return retCode;
}

int shaderError(GLuint shader, const char* name)
{
	int infologLength = 0;
	int charsWritten = 0;
	int success = 0;
	GLchar *infoLog;
	CHECKERROR;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);
	CHECKERROR;
	if (!success)
	{
		if (infologLength > 1)
		{
			infoLog = (GLchar *)malloc(infologLength);
			glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
			printf("Shader InfoLog (%s):\n%s", name, infoLog);
			free(infoLog);
		}
		else
			printf("Shader InfoLog (%s): <no info log>\n", name);
		return 1;
	}
	CHECKERROR;
	return 0;
}

int programError(GLuint program, const char* vert, const char* frag)
{
	int infologLength = 0;
	int charsWritten = 0;
	int success = 0;
	GLchar *infoLog;
	CHECKERROR;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength);
	CHECKERROR;
	if (!success)
	{
		if (infologLength > 1)
		{
			infoLog = (GLchar *)malloc(infologLength);
			glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
			printf("Program InfoLog (%s/%s):\n%s", vert, frag, infoLog);
			free(infoLog);
		}
		else
			printf("Program InfoLog (%s/%s): <no info log>\n", vert, frag);
		return 1;
	}
	CHECKERROR;
	return 0;
}

char* readFile(const char* filename)
{
	int size;
	char* data;
	FILE* file = fopen(filename, "rb");
	if (!file) 
		return NULL;
		
	/* Find the size of the file */
	fseek(file, 0, 2);
	size = ftell(file);
	fseek(file, 0, 0);
	
	/* Read the whole file */
	data = (char*)malloc(size+1);
	fread(data, sizeof(char), size, file);
	fclose(file);
	data[size] = '\0';
	return data;
}

GLuint createShader(const char* filename, GLenum type)
{
	char* source;
	GLuint shader;

	/* Read the contents of the source files */
	source = readFile(filename);
	if (!source)
	{
		printf("Error reading shader %s\n", filename);
		return 0;
	}
	
	/* Create the shader */
	shader = glCreateShader(type);
	
	/* Pass in the source code for the shader */
	glShaderSource(shader, 1, (const GLchar**)&source, NULL);
	
	/* Compile and check each for errors */
	glCompileShader(shader);
	if (shaderError(shader, filename))
	{
		glDeleteShader(shader);
		shader = 0;
	}
	
	free(source);
	return shader;
}

GLuint getShader(const char* vertexFile, const char* fragmentFile)
{
	GLuint vert, frag, program;

	/* If the error points here, it's before this function is called */
	CHECKERROR;
	
	/* Create the shaders */
	vert = createShader(vertexFile, GL_VERTEX_SHADER);
	frag = createShader(fragmentFile, GL_FRAGMENT_SHADER);
	if (!vert && !frag) 
		return 0;

	/* Create program, attach shaders, link and check for errors */
	program = glCreateProgram();
	if (vert) 
		glAttachShader(program, vert);
	if (frag) 
		glAttachShader(program, frag);
	glLinkProgram(program);
	if (programError(program, vertexFile, fragmentFile))
	{
		glDeleteProgram(program);
		program = 0;
	}

	/* Clean up intermediates and return the program */
	if (vert) 
		glDeleteShader(vert);
	if (frag) 
		glDeleteShader(frag);
	return program; /* NOTE: use glDeleteProgram to free resources */
}

