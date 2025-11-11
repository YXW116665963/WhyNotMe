project(glad)
set(src_path
    "./"
)
file(GLOB_RECURSE project_headers *.h *.hpp)
file(GLOB_RECURSE project_cpps *.cpp *.c)
set(all_sourcefile ${project_headers} ${project_cpps})

source_group_by_dir(all_sourcefile ${src_path})

add_library(${PROJECT_NAME} STATIC ${all_sourcefile})
include_directories(include)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "foundation")
set_target_properties(${PROJECT_NAME}
		PROPERTIES RUNTIME_OUTPUT_DIRECTORY ""
		ARCHIVE_OUTPUT_DIRECTORY ${lib_dir}
		LIBRARY_OUTPUT_DIRECTORY ${lib_dir}
		)
