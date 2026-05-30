project(middleware)

file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_sourcefile ${project_headers} ${project_cpps})

#
set(src_path "./")
source_group_by_dir(all_sourcefile ${src_path})

#inc

##
include_directories(${middleware_inc_dir_path})

##
##foundation_inc
include_directories(${common_inc_dir_path})
include_directories(${platform_inc_dir_path})
include_directories(${global_inc_dir_path})
include_directories(${data_def_inc_dir_path})
##

##ability_inc
include_directories(${data_center_inc_dir_path})
##

##dep_inc
include_directories(${wxwidgets_inc_dir_path})
include_directories(${glog_inc_dir_path})
include_directories(${rapidxml_inc_dir_path})

# dep lib path
link_directories(${PROJECT_NAME} PRIVATE ${wxwidgets_lib_dir_path})
link_directories(${PROJECT_NAME} PRIVATE ${glog_lib_dir_path})

# my lib_dir
link_directories(${PROJECT_NAME} PRIVATE ${lib_dir_path})


##
add_library(${PROJECT_NAME} SHARED ${all_sourcefile})

target_link_libraries(${PROJECT_NAME} 
	${glog_lib_name} 
	${wxwidgets_lib_name}
	${common_lib_name}
	${global_lib_name}
	${platform_lib_name}
	${data_def_lib_name}	
)

add_dependencies(${PROJECT_NAME} 
	common 
	platform 
	global
	data_def
	data_center
) 

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "middleware")
CONFIG_OUTPUT_PATHS(${PROJECT_NAME})