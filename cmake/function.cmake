#定义宏，查找当前文件夹下所有文件，用于设置工程目录结构
macro(source_group_by_dir source_files strip_head)
    set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
    foreach (sgbd_file ${${source_files}})
        string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
        string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
        string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
        string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
        string(COMPARE EQUAL ${sgbd_group_name} "." root_path)
        if (root_path)
            set(sgbd_group_name "\\")
        else()
            if (sgbd_nogroup)
                set(sgbd_group_name "\\")
            else ()
                set(strip_len 0)
                string(LENGTH ${strip_head} strip_len)
				
                set(group_len 0)
                string(LENGTH ${sgbd_group_name}, group_len)

                if (NOT (${strip_head} STREQUAL "./"))
                    string(SUBSTRING ${sgbd_group_name} ${strip_len} ${group_len} sgbd_group_name)
                endif ()
            endif (sgbd_nogroup)
        endif()    
        source_group(${sgbd_group_name} FILES ${sgbd_file})
    endforeach (sgbd_file)
endmacro(source_group_by_dir)


# 对目标指定生成位置
# dll和pdb也会被配置到bin文件！！！
macro(CONFIG_OUTPUT_PATHS TARGETNAME)
  set_target_properties(${TARGETNAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_dir}
    ARCHIVE_OUTPUT_DIRECTORY ${lib_dir}
    LIBRARY_OUTPUT_DIRECTORY ${lib_dir}
  )
endmacro(CONFIG_OUTPUT_PATHS)

macro(STRING_APPEND var str)
    set(${var} ${${var}}${str})
endmacro()

# function(configure_exe proj)
	# CONFIG_OUTPUT_PATHS(${proj})
	# # if(WIN32)
		# # configure_file(
					# # ${CMAKE_HOME_DIRECTORY}/CMake/VSUserFile.vcxproj.user.in
					# # ${CMAKE_CURRENT_BINARY_DIR}/${proj}.vcxproj.user
					# # @ONLY
				# # )
	# # endif()
# endfunction()
