cmake_minimum_required (VERSION 3.10.0...4.0.3)
project(WhyNotDo)
#WhyNotMe
message(STATUS "CMAKE_SYSTEM_NAME： ${CMAKE_SYSTEM_NAME} .")

include(cmake/function.cmake)
include(cmake/cxx_stand.cmake)
include(cmake/win.cmake)
include(cmake/linux.cmake)
include(cmake/short_path.cmake)
include(cmake/local_compile_lib_name.cmake)

include(cmake/dependence/glog.cmake)
include(cmake/dependence/wxwidgets.cmake)
include(cmake/dependence/opengl.cmake)
include(cmake/dependence/header-only.cmake)
add_subdirectory(src)