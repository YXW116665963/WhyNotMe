# release和debug模式区分，系统区分
set(BuildConfig ${OS_NAME}/${CMAKE_CFG_INTDIR})
#

# build path
set(lib_dir ${CMAKE_HOME_DIRECTORY}/lib/${OS_NAME})
set(bin_dir ${CMAKE_HOME_DIRECTORY}/build/${OS_NAME})
#




# dep library dir
set(dependence_dir ${CMAKE_HOME_DIRECTORY}/dependence)
set(wxwidgets_lib_dir ${dependence_dir}/wxwidgets/${BuildConfig})
set(glog_lib_dir ${dependence_dir}/glog/${BuildConfig})
set(glfw_lib_dir ${dependence_dir}/glfw/${BuildConfig})
#

# dep library path


#


# src dir
set(foundation_dir ${CMAKE_HOME_DIRECTORY}/src/foundation)
set(ability_dir ${CMAKE_HOME_DIRECTORY}/src/ability)
set(middleware_dir ${CMAKE_HOME_DIRECTORY}/src/middleware)
set(whynotview_app_dir ${CMAKE_HOME_DIRECTORY}/src/app/WhyNotView)
#



# src include
# head file name set of library

##ability
set(datacenter_inc ${ability_dir}/data_center/include)


##dep inc
set(wxwidgets_inc 
	${dependence_dir}/wxwidgets/include
	${dependence_dir}/wxwidgets/include/mswud
)
set(glog_inc ${dependence_dir}/glog/include)
set(xml_inc ${dependence_dir}/rapidxml/include)
set(json_inc ${dependence_dir}/rapidjson/include)
set(glm_inc ${dependence_dir}/glm)
set(glfw_inc
	${dependence_dir}/glfw/include
	${dependence_dir}/glfw/deps
)


##foundation
set(glad_inc ${foundation_dir}/glad/include)
set(sqlite_inc ${foundation_dir}/sqlite)
set(common_inc ${foundation_dir}/common)
set(platform_inc ${foundation_dir}/platform/include)


##middleware
set(middleware_inc
	${middleware_dir}
	${middleware_dir}/control
	${middleware_dir}/workflow
	${middleware_dir}/async_task
	${middleware_dir}/event
)
#





