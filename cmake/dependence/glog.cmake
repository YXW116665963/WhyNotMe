set(glog_dll_name "glog$<$<CONFIG:Debug>:d>.dll")
set(glog_lib_name "glog$<$<CONFIG:Debug>:d>.lib")
add_definitions(-DGLOG_USE_GLOG_EXPORT)

set(glog_dir_path ${dependence_dir_path}/glog)

if(OS_NAME STREQUAL "windows")
	add_definitions(-DGLOG_USE_WINDOWS_PORT)	
	set(glog_lib_dir_path ${glog_dir_path}/${BuildConfig})
	set(glog_inc_dir_path ${glog_dir_path}/include)
else()
	#MESSAGE("Not windows platform is not set wxwidgets path")
endif()