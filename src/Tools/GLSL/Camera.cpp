#include "Camera.h"


Camera::Camera(){
	refreshMat();
}


void Camera::setAngle(GLfloat angle){
	_perspective_Angle = angle;
	refreshMat();
}

void Camera::setRation(GLfloat ratio){
	_perspective_Ratio = ratio;
	refreshMat();
}

void Camera::setPosition(GLfloat x, GLfloat y, GLfloat z){
	_view_At = glm::vec3(x,y,z);
	refreshMat();
}
void Camera::setLookAt(GLfloat x, GLfloat y, GLfloat z){
	_view_LookAt = glm::vec3(x,y,z);
	refreshMat();
}
void Camera::setHeadDir(GLfloat x, GLfloat y, GLfloat z){
	_view_HeadDir = glm::vec3(x,y,z);
	refreshMat();
}

void Camera::refreshMat(){
    // Projection matrix : 45° Field of View, 16:9 ratio, display range : 0.1 unit <-> 100 units
    _ProjectionMat = glm::perspective(
    							_perspective_Angle,
    							_perspective_Ratio,
    							 0.1f, 1000.0f);
    // Camera matrix
    _ViewMat = glm::lookAt(
                                _view_At, // Camera is at ...
                                _view_LookAt, // and looks at the origin
                                _view_HeadDir  // Head is up (set to 0,1,0 to look upside-down)
                           );


    _Mat = _ProjectionMat * _ViewMat ;
}
