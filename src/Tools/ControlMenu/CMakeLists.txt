##### 設定環境
	cmake_minimum_required(VERSION 3.2)
	project(ControlMenu_LIB)

##### 設定 flags
	set(CMAKE_CXX_FLAGS "-std=c++14 -g")

##### 設定各種路徑
	## 找libraray
		# 找gtkmm
		find_package(PkgConfig REQUIRED )
		pkg_check_modules(GTKMM gtkmm-3.0)
		link_directories(${GTKMM_LIBRARY_DIRS}  )
		include_directories(${GTKMM_INCLUDE_DIRS}  )


	set(SRCS ControlMenu.cpp)

##### link相關lib
	link_libraries(${GTKMM_LIBRARIES})
##### 加入自己的tools to lib
	add_library(ControlMenu_LIB ${SRCS})
	set(CMAKE_BUILD_TYPE RELEASE)
