project(whynotplan)

set(proj_name WhyNotPlan)

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
##
#

# 包含库目录
## 3rd lib dir
link_directories(${glog_lib_dir})
link_directories(${wxwidgets_lib_dir})
##
#
link_directories(${lib_dir})

add_executable(${proj_name} ${all_sourcefile})

# 链接库
target_link_libraries(${proj_name} 
"middleware.lib" 
"common.lib" 
${glog_lib_name} 
${wxwidgets_lib_name} 
)

# 配置exe为窗口程序，而非控制台程序
target_link_options(${proj_name} PRIVATE /SUBSYSTEM:WINDOWS)

# 配置构建依赖
add_dependencies(${proj_name} 
data_center 
middleware 
common 
platform 
sqlite)

# 配置输出路径
CONFIG_OUTPUT_PATHS(${proj_name})

# 配置生成后事件，拷贝第三方库
if(OS_NAME STREQUAL "windows")
add_custom_command(TARGET ${proj_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${glog_bin_path}/${glog_bin}"
        "${bin_dir}/${CMAKE_CFG_INTDIR}")

add_custom_command(TARGET ${proj_name} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${wxwidgets_lib_path}/${wxbaselib}"
	"${bin_dir}/${CMAKE_CFG_INTDIR}")

add_custom_command(TARGET ${proj_name} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${wxwidgets_lib_path}/${wxmswlib}"
	"${bin_dir}/${CMAKE_CFG_INTDIR}")
endif()

# 项目上层文件夹昵称
set_target_properties(${proj_name} PROPERTIES FOLDER "app")

