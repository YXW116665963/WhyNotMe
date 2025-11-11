#pragma once

//#include <windows.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string>

// 向量数学计算
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLHandler
{
public:
	GLHandler();
	~GLHandler() {};


	void GLPrepare();
	void GLWhileDo();
	void GLEnd();

private:
	unsigned int shaderProgram;
	unsigned int VAO;
	unsigned int VBO;

	glm::mat4 model     ;
	glm::mat4 view      ;
	glm::mat4 projection;
};




// 创建着色器程序
unsigned int CreateShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);

// 初始化 GLFW 和 OpenGL
bool InitOpenGL(const int WIDTH, const int HEIGHT);