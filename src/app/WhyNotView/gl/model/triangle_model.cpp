#include "glad/glad.h"
#include "triangle_model.h"
#include "file_util.h"
#include "xml_util.h"

namespace why
{
	TriangleModel::TriangleModel(const GLuint& uVao, const GLuint& uVbo)
		: m_uVao(uVao)
		, m_uVbo(uVbo)
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
	}

	TriangleModel::~TriangleModel()
	{

	}

	void TriangleModel::Draw()
	{
		//图元装配
		glBindVertexArray(m_uVao);		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}
}

