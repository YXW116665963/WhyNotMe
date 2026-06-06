# GLM	opengl专用的数学库，only-head
add_definitions(-DGLM_FORCE_CXX20)
set(glm_inc_dir_path ${dependence_dir_path}/glm)

# glad	opengl函数加载，代码项目中编译
set(glad_lib_name "glad.lib")


# sys
set(opengl_lib_name "opengl32.lib")


# GLFW:管理gl窗口的简单原生框架，实际项目基本用不上
add_definitions(-DGLFW_DLL)
set(glfw_dll_name "glfw3.dll")
set(glfw_lib_name "glfw3dll.lib")
set(glfw_lib_dir ${dependence_dir_path}/glfw/${BuildConfig})

set(glfw_inc
	${dependence_dir_path}/glfw/include
	${dependence_dir_path}/glfw/deps
)


