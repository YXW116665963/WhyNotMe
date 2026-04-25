project(common)

file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_sourcefile ${project_headers} ${project_cpps})

#
set(src_path "./")
source_group_by_dir(all_sourcefile ${src_path})


## 3rd
include_directories(${glog_inc_dir_path})
include_directories(${rapidxml_inc_dir_path})
include_directories(${platform_inc_dir_path})
##

## 3rd lib dir
link_directories(${glog_lib_dir_path})
##

## my lib dir
link_directories(${lib_dir_path})
##


add_library(${PROJECT_NAME} STATIC ${all_sourcefile})
# 静态库只是把代码归档到链接他的库，本身不处理自身和其他库的连接关系
# 换言之，使用静态库的项目必须要链接该静态库所依赖的库！！！
target_link_libraries(${PROJECT_NAME} 
	${glog_lib_name} 
	${platform_lib_name}
)

add_dependencies(${PROJECT_NAME} platform)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "foundation")
set_target_properties(${PROJECT_NAME}
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY ""
	ARCHIVE_OUTPUT_DIRECTORY ${lib_dir_path}
	LIBRARY_OUTPUT_DIRECTORY ${lib_dir_path}
)