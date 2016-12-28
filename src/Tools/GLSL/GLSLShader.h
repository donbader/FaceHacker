#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <GL/glew.h>

using namespace std;



class GLSLShader
{
public:
	GLSLShader(const string& fileName);
	GLuint id(){return _id;}

private:
	GLint type(const string& fileName);
	GLchar* readFile(const string& fileName);
	GLuint compile(const string& file);
	GLuint _id;
};

