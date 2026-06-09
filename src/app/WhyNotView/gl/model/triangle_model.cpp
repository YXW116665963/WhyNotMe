#include "glad/glad.h"
#include "triangle_model.h"
#include "file_util.h"
#include "xml_util.h"

namespace why
{
	TriangleModel::TriangleModel(ModelInfo& modelInfo)
		: m_uVao(modelInfo.uVao)
		, m_uVbo(modelInfo.uVbo)
		, strShadersProgramName("triangle")
	{		
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);// 将顶点数据赋值给VBO		
		glLineWidth(1);// 设置线宽


		//#glVertexAttribPointer
		// 第一个参数为，索引开始位置
		// 第二个参数为，每个顶点属性的组件数量
		// 第五个参数为，步长（stride），即每个顶点属性之间的字节偏移量。有时顶点数组中可能包含法线数据
		//
		// 着色器中设置location = 0的属性就是决定这个设置是法线还是颜色
		// 对应第一个参数起始索引“0”！！！
		// 
		// 
		// 设置VAO，也就是设置如何使用VBO的数据
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		// 作用：打开 0 号顶点属性通道，让显卡能读取 VBO 里的顶点数据。相当于让显卡“打开文件”
		glEnableVertexAttribArray(0);


		// 设置1号通道，但是着色器未设置，实际无效果
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		GetShaderInfo(strShadersProgramName, m_vecShaderInfo);		
		CompileLinkShader(m_vecShaderInfo, modelInfo.uShaderProgramId);
	}

	TriangleModel::~TriangleModel()
	{

	}



	void TriangleModel::Draw(const GLuint& uShaderProgramId)
	{
		//每次画新一帧画面之前，都要把上一帧的画面擦掉
		//glClearColor 就是设置：擦除后，屏幕变成什么颜色
		//红绿蓝、透明度
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		//用 glClearColor 设置好的颜色，把整个屏幕刷一遍，擦掉上一帧的所有画面！
		//GL_COLOR_BUFFER_BIT = 颜色缓冲区
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(uShaderProgramId);

		// 模型、视图和投影矩阵
		// 模型和视图矩阵要正常变化均需要设置为单位矩阵的初值
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);


		/*
			glm::rotate
			作用：生成一个旋转矩阵，并应用到输入矩阵上
		*/
		// 绕X轴负向55°，等同于头向上仰
		// vec3(1.0f, 0.0f, 0.0f)表示x轴
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		/*
			glm::translate
			作用：生成一个平移矩阵，并应用到输入矩阵上
			ps:方向和如何应用到矩阵的计算参看learnOpengl
		*/
		// 相机后移3个单位
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		/*
			定义一个摄像机位置，一个目标位置和一个表示世界空间中的上向量的向量（我们计算右向量使用的那个上向量）。
			接着GLM就会创建一个LookAt矩阵
			eye:方向向量，center到摄像机位置的向量
			center：视点，摄像机看向的点，本例中为坐标原点
			up:自定义的上向量，本例中为(0.0f, 1.0f, 0.0f)，即Y轴正方向
		*/

		float radius = 10.0f;
		float camX = sin(getTime()) * radius;
		float camZ = cos(getTime()) * radius;

		view = glm::lookAt(glm::vec3(camX, 0.0, camZ),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1800 / 1000, 0.1f, 100.0f);

		//// 设置模型、视图和投影矩阵
		//// glGetUniformLocation：给着色器中对应变量赋值
		GLuint modelLoc = glGetUniformLocation(uShaderProgramId, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GLuint viewLoc = glGetUniformLocation(uShaderProgramId, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		GLuint projectionLoc = glGetUniformLocation(uShaderProgramId, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		//图元装配
		glBindVertexArray(m_uVao);		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}
}

