project(platform)

#
file(GLOB_RECURSE ${platform_inc}/*.h)
file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_sourcefile ${project_headers} ${project_cpps})
set(src_path "./")
source_group_by_dir(all_sourcefile ${src_path})
source_group(include FILES ${platform_include_headers})

set(all_sourcefile ${all_sourcefile} ${platform_include_headers})
add_library(${PROJECT_NAME} STATIC ${all_sourcefile})


include_directories(${foundation_dir}/common)
include_directories(${foundation_dir}/platform/include)
include_directories(${foundation_dir}/platform/${OS_NAME})

# add_dependencies(${PROJECT_NAME} common)
target_link_libraries(${PROJECT_NAME} "common.lib")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "foundation")
set_target_properties(${PROJECT_NAME}
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY ""
	ARCHIVE_OUTPUT_DIRECTORY ${lib_dir}
	LIBRARY_OUTPUT_DIRECTORY ${lib_dir}
)




