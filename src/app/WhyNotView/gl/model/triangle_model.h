#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "wx/glcanvas.h"

namespace why
{
	struct ModelInfo
	{
		GLuint uVao;
		GLuint uVbo;
	};


	class TriangleModel
	{
	public:
		TriangleModel(ModelInfo& modelInfo);
		~TriangleModel();

		void Draw(const GLuint& uShaderProgramId);

	private:
		


		float vertices[18] = {
			-0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f,
			0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f,
			0.0f,  0.5f, 0.0f, 0.f, 0.f, 1.f
		};


		/* VBO = 装顶点数据的 “显存盒子”
		作用：在显卡上开辟一块内存
		内容：存放顶点坐标（x,y,z）、颜色、法线等
		一句话：VBO = 显卡里的数据集
		*/
		GLuint m_uVbo;

		/* VAO = 记录 “怎么画” 的配置单
		作用：保存 VBO 的格式、读取方式
		内容：数据从哪来？几个数一组？偏移多少？
		一句话：VAO = 绘画配置文件
		*/
		GLuint m_uVao;
	};
}


// VAO / VBO：只管数据怎么存、怎么读
// 着色器：只管数据是什么含义
// 顶点着色器（Vertex Shader）
//const char* vertexShaderSource =
//	//告诉显卡：我用的是 OpenGL 3.3 核心版本语法，必须写在第一行
//	"#version 330 core\n"
//	//layout(location = 0) in vec3 aPos;     // 0 号 = 位置
//	//layout(location = 0) in vec3 aNormal;  // 0 号 = 法线 
//	//输入：从 C++ 传过来的 顶点坐标（x,y,z）aPos
//	"layout (location = 0) in vec3 aPos;\n"
//	//模型矩阵：控制物体 平移 / 旋转 / 缩放
//	"uniform mat4 model;\n"
//	//观察矩阵：控制 相机位置、看哪里
//	"uniform mat4 view;\n"
//	//投影矩阵：做 3D 透视效果（近大远小）
//	"uniform mat4 projection;\n"
//	"void main() {\n"
//	"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"   //计算出这个顶点最终在屏幕上的位置
//	"}\0";
//// 片段着色器（Fragment Shader）
//const char* fragmentShaderSource = "#version 330 core\n"
//	"out vec4 FragColor;\n"                                             //输出：最终像素颜色
//	"void main() {\n"
//	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // 橙色            //格式：(R, G, B, A) → 红、绿、蓝、透明度
//	"}\0";

//const char* vertexShaderSource =
//"#version 450 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"layout (location = 1) in vec3 aNormal;\n"
//"void main() {\n"
//"   gl_Position = vec4(aPos, 1.0);\n"
//"}\0";
//const char* fragmentShaderSource = "#version 450 core\n"
//"out vec4 FragColor;\n"                                             //输出：最终像素颜色
//"void main() {\n"
//"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // 橙色            //格式：(R, G, B, A) → 红、绿、蓝、透明度
//"}\0";