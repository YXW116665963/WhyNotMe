project(middleware)

file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_sourcefile ${project_headers} ${project_cpps})

#
set(src_path "./")
source_group_by_dir(all_sourcefile ${src_path})

#inc

##
include_directories(${middleware_inc})

##
##foundation_inc
include_directories(${common_inc})
include_directories(${platform_inc})
##

##ability_inc
include_directories(${datacenter_inc})
##

##dep_inc
include_directories(${wxwidgets_inc})
include_directories(${glog_inc})
include_directories(${xml_inc})

# dep lib path
link_directories(${PROJECT_NAME} PRIVATE ${wxwidgets_lib_dir})
link_directories(${PROJECT_NAME} PRIVATE ${glog_lib_dir})

# my lib_dir
link_directories(${PROJECT_NAME} PRIVATE ${lib_dir})


##
add_library(${PROJECT_NAME} SHARED ${all_sourcefile})




target_link_libraries(${PROJECT_NAME} 
	${glog_lib_name} 
	${wxwidgets_lib_name}
	"common.lib"
)

add_dependencies(${PROJECT_NAME} common platform) 

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "middleware")
CONFIG_OUTPUT_PATHS(${PROJECT_NAME})