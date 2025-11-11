cmake_minimum_required (VERSION 3.10.0...4.0.3)
project(WhyNotMe)
#WhyNotMe
message(STATUS "CMAKE_SYSTEM_NAME： ${CMAKE_SYSTEM_NAME} .")

include(cmake/cxx_stand.cmake)
include(cmake/win.cmake)
include(cmake/linux.cmake)
include(cmake/glog.cmake)
include(cmake/wxwidgets.cmake)
include(cmake/gl_deps.cmake)
include(cmake/short_path.cmake)
include(cmake/function.cmake)
	

add_subdirectory(src)	