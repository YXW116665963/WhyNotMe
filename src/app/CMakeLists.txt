add_subdirectory(WhyNotPlan)
add_subdirectory(WhyNotView)

# S.4 配置编译选择项
list(
    APPEND
    pro_list
	WhyNotView 							# CAD
	WhyNotPlan 							# 绘图
)
#将所有编译选项都设为On打开
foreach(BUILD_ITEM ${pro_list})
    option(${BUILD_ITEM} "${BUILD_ITEM}" ON)
	#message(STATUS "set ${BUILD_ITEM}：${${BUILD_ITEM}}")
endforeach()