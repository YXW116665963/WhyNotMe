project(sample_glfw)

set(proj_name sample_glfw)

# 递归搜索源码
file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_sourcefile ${project_headers} ${project_cpps})
set(src_path "./")
source_group_by_dir(all_sourcefile ${src_path})


# inc
##foundation
include_directories(${common_inc})
include_directories(${platform_inc})
##
include_directories(${middleware_inc})

## app
include_directories(${app_dir}/view)
include_directories(${app_dir}/panel)
include_directories(${app_dir}/service)
##
## 3rd inc
include_directories(${xml_inc})
include_directories(${glog_inc})
include_directories(${wxwidgets_inc})
include_directories(${json_inc})
include_directories(${glfw_inc})
include_directories(${glm_inc})
##
#

# 包含库目录
## 3rd lib dir
link_directories(${glog_lib_dir})
link_directories(${wxwidgets_lib_dir})
link_directories(${lib_dir})
link_directories(${glfw_lib_dir})
##
#


add_executable(${proj_name} ${all_sourcefile})

# 链接库
target_link_libraries(${proj_name} 
	"middleware.lib" 
	"common.lib" 
	${glog_lib_name} 
	${wxwidgets_lib_name} 
	${glfw_lib_name} 
)

# 配置exe为窗口程序，而非控制台程序
# @why.test，测试gl，暂不开启
#target_link_options(${proj_name} PRIVATE /SUBSYSTEM:WINDOWS)

# 配置构建依赖
add_dependencies(${proj_name} 
	data_center 
	middleware 
	common 
	platform 
	sqlite
)

# 配置输出路径
CONFIG_OUTPUT_PATHS(${proj_name})

# 配置生成后事件，拷贝第三方动态库
if(OS_NAME STREQUAL "windows")
	macro(copy_dll target dll_dir dll_name)
		add_custom_command(TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${dll_dir}/${dll_name}"
				"${bin_dir}/${CMAKE_CFG_INTDIR}")
	endmacro(copy_dll)

	copy_dll(${proj_name} ${glog_lib_dir} ${glog_dll_name})
	copy_dll(${proj_name} ${wxwidgets_lib_dir} ${wxwidgets_dll_name})
	copy_dll(${proj_name} ${glfw_lib_dir} ${glfw_dll_name})
endif()

# 项目上层文件夹昵称
set_target_properties(${proj_name} PROPERTIES FOLDER "sample")