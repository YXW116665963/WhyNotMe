#pragma once

// <windows.h>放在glfw前面防止报错宏重定义
#include <windows.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include "linmath.h"

// 向量数学计算
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 创建着色器程序
unsigned int CreateShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);

// 在 glfw 窗口标题显示中文
void SetWindowTitleUTF8(GLFWwindow* window, const char* title);

// 初始化 GLFW 和 OpenGL
bool InitOpenGL(const int WIDTH, const int HEIGHT);

// 帧率控制函数（why），在opengl循环中调用
bool ShouldRedraw(int fps);

// 帧率控制函数（感觉没啥屁用）
double AdjustFrameRate(int& fps, double& frameTimeLimit, double standard_frame_rate, double& timeFrameLast, double& timeFramePrevious);