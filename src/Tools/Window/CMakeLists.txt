##### 設定環境
	cmake_minimum_required(VERSION 3.2)
	project(Window_LIB)

##### 設定 flags
	set(CMAKE_CXX_FLAGS "-std=c++14 -g -O3")

##### 設定各種路徑
	## 找libraray
		# 找OpenGL
		find_package( OpenGL REQUIRED )

		# 找GLEW
		find_package( GLEW REQUIRED)

		# 找glfw
		find_package( glfw3 REQUIRED)

	set(LIBS
		${OPENGL_gl_LIBRARY}
		${OPENGL_glu_LIBRARY}
		${GLEW_LIBRARIES}
		glfw)

	set(SRCS Window.cpp)

##### link相關lib
	link_libraries(${LIBS})
##### 加入自己的tools to lib
	add_library(Window_LIB ${SRCS})
	set(CMAKE_BUILD_TYPE RELEASE)
