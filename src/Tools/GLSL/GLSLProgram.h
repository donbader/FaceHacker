#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <array>

// Opengl
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "GLSLShader.h"
#include "GLSLVertexObject.h"

class GLSLProgram
{
public:
	GLSLProgram(const string& vsFile, const string& fsFile, const string& gsFile = "none");
	~GLSLProgram();

	void use(){ glUseProgram(_id); }
	void render();
	int addObj(
	    GLfloat* positions, GLuint positions_size,
	    GLfloat* texcoords, GLuint texcoords_size,
	    GLfloat* normals, GLuint normals_size,
	    GLuint* indices, GLuint indices_size,
	    cv::Mat& img
	);
	void deleteObj(int index);

	GLSLVertexArrayObject* object(int index);
	GLuint id(){return _id;}
	Camera& camera(){return _camera;}


	// uniform setting functions
	GLint getLocation(const char* name);
	void setUniformMat4(const char *name, glm::mat4 &mat);
	void setUniformVec3(const char *name, glm::vec3 &vec);
	void setUniformFloat(const char *name, float f);
	void setUniformInt(const char *name, int i);

	static GLfloat* computeVertexNormals(GLfloat* vertex, GLuint vertex_size, GLuint* indice, GLuint indice_size);

protected:
	void link(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader = -1);


	GLuint _id;
	vector<GLSLVertexArrayObject *> _VAOs;
	Camera _camera;

};


