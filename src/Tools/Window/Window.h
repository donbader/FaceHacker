#pragma once

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>


class Window {
public:
	Window(unsigned width = 960, unsigned height = 540, const char* name = "FaceHacker");
	~Window();

	bool shouldClose();
	void setBackground(GLfloat r, GLfloat g, GLfloat b);
	void render();
	GLFWwindow* get(){return _ptr;}
private:
	GLFWwindow* _ptr;
};

