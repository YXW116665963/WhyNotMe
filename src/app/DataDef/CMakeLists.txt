project(data_def)

# 递归搜索源码
file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_sourcefile ${project_headers} ${project_cpps})

set(src_path "./")
source_group_by_dir(all_sourcefile ${src_path})

add_library(${PROJECT_NAME} STATIC ${all_sourcefile})

set_target_properties(${PROJECT_NAME} PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE
)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "app")
set_target_properties(${PROJECT_NAME}
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_dir_path}
	ARCHIVE_OUTPUT_DIRECTORY ${lib_dir_path}
	LIBRARY_OUTPUT_DIRECTORY ${lib_dir_path}
)


