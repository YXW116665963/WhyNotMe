project(whynotview)

set(proj_name WhyNotView)

# 递归搜索源码
file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_sourcefile ${project_headers} ${project_cpps})
set(src_path "./")
source_group_by_dir(all_sourcefile ${src_path})


# inc
##foundation
include_directories(${common_inc_dir_path})
include_directories(${platform_inc_dir_path})
include_directories(${global_inc_dir_path})
include_directories(${data_def_inc_dir_path})

## ability
include_directories(${middleware_inc_dir_path})
include_directories(${data_center_inc_dir_path})


## app
include_directories(${whynotview_app_dir_path})
include_directories(${whynotview_app_dir_path}/panel)
include_directories(${whynotview_app_dir_path}/gl)
##
## 3rd inc
include_directories(${rapidxml_inc_dir_path})
include_directories(${glog_inc_dir_path})
include_directories(${wxwidgets_inc_dir_path})
include_directories(${glad_inc_dir_path})
include_directories(${glm_inc_dir_path})
##
#

# 包含库目录
## 3rd lib dir
link_directories(${glog_lib_dir_path})
link_directories(${wxwidgets_lib_dir_path})

##
link_directories(${lib_dir_path})
#


add_executable(${proj_name} ${all_sourcefile})

# 链接库
target_link_libraries(${proj_name} 
	${data_center_lib_name}
	${global_lib_name}
	${middleware_lib_name}
	${common_lib_name}
	${glog_lib_name} 
	${wxwidgets_lib_name} 
	${platform_lib_name}
	${data_def_lib_name}
	${glad_lib_name}	
)

# 配置exe为窗口程序，而非控制台程序
target_link_options(${proj_name} PRIVATE /SUBSYSTEM:WINDOWS)
#
# 配置构建依赖
add_dependencies(${proj_name} 
	global
	data_def
	data_center 
	middleware 
	common 
	platform 
	sqlite
	glad
)

# 配置输出路径
CONFIG_OUTPUT_PATHS(${proj_name})

# 配置生成后事件，拷贝第三方库
if(OS_NAME STREQUAL "windows")
	
COPY_FILE_TO_BIN(${proj_name} ${glog_lib_dir_path}/${glog_dll_name})
COPY_FILE_TO_BIN(${proj_name} ${wxwidgets_lib_dir_path}/${wxwidgets_dll_name})

endif()

# 拷贝资源文件
add_custom_command(TARGET ${proj_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${whynotview_app_dir_path}/Resource"
    "${bin_dir_path}/${CMAKE_CFG_INTDIR}/Resource")


# 拷贝数据def
file(GLOB_RECURSE all_data_def_files
	RELATIVE "${CMAKE_HOME_DIRECTORY}/dataDef"
	"${CMAKE_HOME_DIRECTORY}/dataDef/*")	
foreach(file in lists all_data_def_files)
	get_filename_component(name "${file}" NAME)
	
	list(FIND exclude_files "${name}" name_idx)
	if (NOT name_idx EQUAL -1)
		add_custom_command(TARGET ${proj_name} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${CMAKE_HOME_DIRECTORY}/data/${file}"
				"${bin_dir_path}/${CMAKE_CFG_INTDIR}")
	endif()
endforeach()



# 项目上层文件夹昵称
set_target_properties(${proj_name} PROPERTIES FOLDER "app")

