/* pknowles Wed 18 Aug 2010 09:21:32 PM EST */

#ifndef SHADERS_H
#define SHADERS_H

#define CHECKERROR oglError(__LINE__, __FILE__)

int oglError(int line, const char* file);
GLuint getShader(const char* vertexFile, const char* fragmentFile);

#endif
