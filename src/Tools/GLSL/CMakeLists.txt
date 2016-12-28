##### 設定環境
	cmake_minimum_required(VERSION 3.2)
	project(GLSL_LIB)

##### 設定 flags
	set(CMAKE_CXX_FLAGS "-std=c++14 -g -O3")

##### 設定各種路徑
	## 找libraray
		# 找OpenGL
		find_package( OpenGL REQUIRED )

		# 找GLEW
		find_package( GLEW REQUIRED)

		# 找OpenCV
		find_package( OpenCV REQUIRED )

		# 找Boost
		find_package(Boost 1.50.0 COMPONENTS system filesystem program_options REQUIRED)


		# 引入3rdparty
		include_directories(${CMAKE_SOURCE_DIR}/3rdparty)
		include_directories(${CMAKE_SOURCE_DIR}/3rdparty/eigen3)


	set(LIBS
		${OPENGL_gl_LIBRARY}
		${OPENGL_glu_LIBRARY}
		${GLEW_LIBRARIES}
		${OpenCV_LIBS}
		${Boost_LIBRARIES}
		)

	set(SRCS
		GLSLProgram.cpp
		GLSLShader.cpp
		Camera.cpp
		FaceProgram.cpp
		)

##### link相關lib
	link_libraries(${LIBS})
##### 加入自己的tools to lib
	add_library(GLSL_LIB ${SRCS})


