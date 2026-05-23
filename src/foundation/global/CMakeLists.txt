project(global)

# 递归搜索源码
file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_sourcefile ${project_headers} ${project_cpps})

# if (WIN32)
	# set(all_sourcefile ${all_sourcefile} "./global.def")
# endif()


set(src_path "./")
source_group_by_dir(all_sourcefile ${src_path})

## dep inc
include_directories(${glog_inc_dir_path})
include_directories(${rapidjson_inc_dir_path})
include_directories(${sqlite_inc_dir_path})
##
include_directories(${common_inc_dir_path})
include_directories(${platform_inc_dir_path})
##
include_directories(${data_center_inc_dir_path})
include_directories(${global_inc_dir_path})

link_directories(${glog_lib_dir_path})
link_directories(${lib_dir_path})

add_library(${PROJECT_NAME} SHARED ${all_sourcefile})
add_dependencies(${PROJECT_NAME}
	common 
	sqlite
	data_def
)

target_link_libraries(${PROJECT_NAME} 
	${glog_lib_name} 
	${data_center_lib_name} 
	${common_lib_name} 
	${platform_lib_name}
	${data_def_lib_name}	
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE
)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "foundation")
set_target_properties(${PROJECT_NAME}
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_dir_path}
	ARCHIVE_OUTPUT_DIRECTORY ${lib_dir_path}
	LIBRARY_OUTPUT_DIRECTORY ${lib_dir_path}
)


