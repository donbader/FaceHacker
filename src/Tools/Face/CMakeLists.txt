##### 設定環境
	cmake_minimum_required(VERSION 3.2)
	project(FACE_LIB)

##### 設定 flags
	set(CMAKE_CXX_FLAGS "-std=c++14 -g -O3")

##### 設定各種路徑
	## 找libraray

		# 找OpenCV
		find_package( OpenCV REQUIRED )

		# 找X11
		find_package( X11 REQUIRED )
		if(X11_FOUND)
			include_directories(${X11_INCLUDE_DIR})
		endif()

		# 找BLAS
		find_package( BLAS REQUIRED )


		# 引入3rdparty
		include_directories(${CMAKE_SOURCE_DIR}/3rdparty)
		include_directories(${CMAKE_SOURCE_DIR}/3rdparty/eigen3)
		# include(${CMAKE_SOURCE_DIR}/3rdparty/dlib/cmake)

	set(LIBS
		${X11_LIBRARIES}
		${BLAS_LIBRARIES}
		GLSL_LIB
		)

	set(SRCS
		Face.cpp
		${CMAKE_SOURCE_DIR}/3rdparty/dlib/all/source.cpp
		)

##### link相關lib
	link_libraries(${LIBS})
##### 加入自己的tools to lib
	add_library(FACE_LIB ${SRCS})

