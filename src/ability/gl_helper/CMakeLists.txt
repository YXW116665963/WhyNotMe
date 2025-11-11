project(gl_helper)

# 递归搜索源码
file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_sourcefile ${project_headers} ${project_cpps})
set(src_path "./")
source_group_by_dir(all_sourcefile ${src_path})


## dep inc
include_directories(${glog_inc})
include_directories(${json_inc})
include_directories(${glfw_inc})
##
include_directories(${common_inc})
include_directories(${platform_inc})

##
include_directories(${ability_dir}/data_center/include)


link_directories(${glog_lib_dir})
link_directories(${lib_dir})

add_library(${PROJECT_NAME} SHARED ${all_sourcefile})
add_dependencies(${PROJECT_NAME} common platform)

target_link_libraries(${PROJECT_NAME} ${glog_lib} "common.lib" "platform.lib")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "ability")
set_target_properties(${PROJECT_NAME}
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_dir}
	ARCHIVE_OUTPUT_DIRECTORY ${lib_dir}
	LIBRARY_OUTPUT_DIRECTORY ${lib_dir}
)


