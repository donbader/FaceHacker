##### 設定環境
	cmake_minimum_required(VERSION 3.2)
	project(FACEHACKER)
	add_subdirectory(Tools)

##### 設定 flags
	set(CMAKE_CXX_FLAGS "-std=c++14 -g")
	# set(BUILD_SHARED_LIBS ON)

##### 設定各種路徑
	## 找libraray
		find_package(Thread )
		# 找OpenGL
		find_package( OpenGL REQUIRED )

		# 找gtkmm
		find_package(PkgConfig REQUIRED )
		pkg_check_modules(GTKMM gtkmm-3.0)
		link_directories(${GTKMM_LIBRARY_DIRS}  )
		include_directories(${GTKMM_INCLUDE_DIRS}  )
		
		# 引入 3rd party 函式庫
		include_directories(${CMAKE_SOURCE_DIR}/3rdparty)
		include_directories(${CMAKE_SOURCE_DIR}/3rdparty/eigen3)


	# 設定src路徑
	set(MAIN_SRCS main.cpp)


	# 設定library路徑
	set(MAIN_LIBS
		${OPENGL_gl_LIBRARY}
		${GTKMM_LIBRARIES}
		Tools)


	# 設定執行檔輸出路徑
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})




##### 產生執行檔
	add_executable(main_test ${MAIN_SRCS})

##### link
	target_link_libraries( main_test ${MAIN_LIBS})
	cotire(main_test)

# 複製需要的dataset
file(COPY ${CMAKE_SOURCE_DIR}/dataset DESTINATION ${CMAKE_BINARY_DIR})

# 程式執行時間優化(-O3 -DNDEBUG)
set(CMAKE_BUILD_TYPE RELEASE)



