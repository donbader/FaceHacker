#include "Window.h"

Window::Window(unsigned width, unsigned height, const char* name){
    // Initialise GLFW
    if ( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        exit(-1);
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _ptr = glfwCreateWindow( width, height, name, NULL, NULL);

    if ( _ptr == NULL ) {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(_ptr);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        exit(-1);
    }


    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(_ptr, GLFW_STICKY_KEYS, GL_TRUE);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

}

Window::~Window(){
	glfwTerminate();
}

void Window::setBackground(GLfloat r, GLfloat g, GLfloat b){
    glClearColor(r, g, b, 0.0f);
}


void Window::render(){
    glfwSwapBuffers(_ptr);
    // glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

bool Window::shouldClose(){
	return ((glfwGetKey(_ptr, GLFW_KEY_ESCAPE ) == GLFW_PRESS) || glfwWindowShouldClose(_ptr));
}


