#include "glad/glad.h"
#include "model.h"
#include "file_util.h"
#include "xml_util.h"

namespace why
{
	Model::Model(const GLuint& uVao, const GLuint& uVbo)
		: m_uVao(uVao)
		, m_uVbo(uVbo)
	{
		
	}

	Model::~Model()
	{

	}

	void Model::DrawTriangle(ShaderProgram* shader)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);// 将顶点数据赋值给VBO		
		glLineWidth(1);// 设置线宽
		
		// 设置VAO，也就是设置如何使用VBO的数据
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);//打开0号通道，即着色器中layout location =0

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);// 设置1号通道，但是着色器未设置，实际无效果

		// 向着色器中传参
		shader->SetVec4f("objectColor", glm::vec4(1.f, 0.2f, 0.2f, 1.f));
		
		//图元装配
		glBindVertexArray(m_uVao);		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}
}

