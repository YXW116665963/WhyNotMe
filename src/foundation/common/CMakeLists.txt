project(common)

file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_sourcefile ${project_headers} ${project_cpps})

#
set(src_path "./")
source_group_by_dir(all_sourcefile ${src_path})


## 3rd
include_directories(${glog_inc})
include_directories(${xml_inc})
include_directories(${platform_inc})
##

## 3rd lib dir
link_directories(${glog_lib_dir})
##

## my lib dir
link_directories(${lib_dir})
##


add_library(${PROJECT_NAME} SHARED ${all_sourcefile})
target_link_libraries(${PROJECT_NAME} 
${glog_lib_name} 
"platform.lib"
)

add_dependencies(${PROJECT_NAME} platform)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "foundation")
CONFIG_OUTPUT_PATHS(${PROJECT_NAME})