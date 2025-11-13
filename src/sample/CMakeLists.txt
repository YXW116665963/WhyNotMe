add_subdirectory(sample_glfw)
add_subdirectory(sample_wxgl)
add_subdirectory(sample_datacenter)
add_subdirectory(sample_rapidjson)


# S.4 配置编译选择项
list(
    APPEND
    sample_list
	sample_glfw 							# glfw与gl适配示例
	sample_wxgl 							# wxWidget与opengl适配示例
	sample_datacenter 						# datacenter示例
	sample_rapidjson
)
#将所有编译选项都设为On打开
foreach(BUILD_ITEM ${sample_list})
    option(${BUILD_ITEM} "${BUILD_ITEM}" ON)
	#message(STATUS "set ${BUILD_ITEM}：${${BUILD_ITEM}}")
endforeach()