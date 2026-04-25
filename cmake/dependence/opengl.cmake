# GLM
add_definitions(-DGLM_FORCE_CXX20)
set(glm_inc ${dependence_dir_path}/glm)



# GLFW:管理gl窗口的简单原生框架，实际项目基本用不上
add_definitions(-DGLFW_DLL)
set(glfw_dll_name "glfw3.dll")
set(glfw_lib_name "glfw3dll.lib")
set(glfw_lib_dir ${dependence_dir_path}/glfw/${BuildConfig})

set(glfw_inc
	${dependence_dir_path}/glfw/include
	${dependence_dir_path}/glfw/deps
)


