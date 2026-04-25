# release和debug模式区分，系统区分
set(BuildConfig ${OS_NAME}/${CMAKE_CFG_INTDIR})
#

# build path
set(lib_dir_path ${CMAKE_HOME_DIRECTORY}/lib/${OS_NAME})
set(bin_dir_path ${CMAKE_HOME_DIRECTORY}/build/WhyNotView/${OS_NAME})
#

# dep library dir
set(dependence_dir_path ${CMAKE_HOME_DIRECTORY}/dependence)


# src dir
set(foundation_dir_path ${CMAKE_HOME_DIRECTORY}/src/foundation)
set(ability_dir_path ${CMAKE_HOME_DIRECTORY}/src/ability)
set(middleware_dir_path ${CMAKE_HOME_DIRECTORY}/src/middleware)
set(whynotview_app_dir_path ${CMAKE_HOME_DIRECTORY}/src/app/WhyNotView)

# src include

##ability
set(data_center_inc_dir_path ${ability_dir_path}/data_center/include)

##foundation
set(glad_inc_dir_path ${foundation_dir_path}/glad/include)
set(sqlite_inc_dir_path ${foundation_dir_path}/sqlite)
set(common_inc_dir_path ${foundation_dir_path}/common)
set(platform_inc_dir_path ${foundation_dir_path}/platform/include)
set(global_inc_dir_path ${foundation_dir_path}/global)
set(data_def_inc_dir_path ${foundation_dir_path}/global/dataDef)
##middleware
set(middleware_inc_dir_path
	${middleware_dir_path}
	${middleware_dir_path}/control
	${middleware_dir_path}/workflow
	${middleware_dir_path}/async_task
	${middleware_dir_path}/event
)





