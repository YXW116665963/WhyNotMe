set(glog_dll_name "glog$<$<CONFIG:Debug>:d>.dll")
set(glog_lib_name "glog$<$<CONFIG:Debug>:d>.lib")
add_definitions(-DGLOG_USE_GLOG_EXPORT)