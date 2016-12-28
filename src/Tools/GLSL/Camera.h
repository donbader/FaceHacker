#pragma once

// Opengl
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
public:
	Camera();
	~Camera(){}

	void setAngle(GLfloat angle);
	void setRation(GLfloat ratio);

	void setPosition(GLfloat x, GLfloat y, GLfloat z);
	void setLookAt(GLfloat x, GLfloat y, GLfloat z);
	void setHeadDir(GLfloat x, GLfloat y, GLfloat z);

	glm::mat4& mat(){return _Mat;}
	glm::mat4& View(){return _ViewMat;}
	glm::mat4& Projection(){return _ProjectionMat;}

private:
	void refreshMat();


	GLfloat _perspective_Angle = 45.f;
	GLfloat _perspective_Ratio = 16.f / 9.f;

	glm::vec3 _view_At = glm::vec3(0,0,16);
	glm::vec3 _view_LookAt = glm::vec3(0,0,0);
	glm::vec3 _view_HeadDir = glm::vec3(0,1,0);

	glm::mat4 _Mat;
	glm::mat4 _ViewMat;
	glm::mat4 _ProjectionMat;
};
