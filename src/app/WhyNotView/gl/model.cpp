#include "model.h"
#include "file_util.h"
#include "xml_util.h"
#include "data_center_value_util.h"
#include "env_var_data_def.h"
#include "logger.h"
//
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//

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
		// 顶点
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);//打开0号通道，即着色器中layout location =0

		// 颜色
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);// 设置1号通道，但是着色器未设置，实际无效果


		// 纹理
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元，0是被默认激活的单元
		glBindTexture(GL_TEXTURE_2D, m_textureInfo.uTextureId);

		// 向着色器中传参
		shader->SetVec4f("objectColor", glm::vec4(1.f, 0.2f, 0.2f, 1.f));
		
		//图元装配
		glBindVertexArray(m_uVao);		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}

	void Model::DrawTriangle2(ShaderProgram* shader)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);// 将顶点数据赋值给VBO		
		glLineWidth(1);// 设置线宽

		// 设置VAO，也就是设置如何使用VBO的数据
		// 顶点
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);//打开0号通道，即着色器中layout location =0

		// 颜色
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);// 设置1号通道，但是着色器未设置，实际无效果

		//图元装配
		glBindVertexArray(m_uVao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}

	void Model::DrawTriangle3(ShaderProgram* shader)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);// 将顶点数据赋值给VBO		
		glLineWidth(1);// 设置线宽

		// 设置VAO，也就是设置如何使用VBO的数据
		// 顶点
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);//打开0号通道，即着色器中layout location =0

		// 颜色
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);// 设置1号通道，但是着色器未设置，实际无效果

		//图元装配
		glBindVertexArray(m_uVao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}


	void Model::LoadTexture()
	{
		PathAppender pathAppender;
		m_textureInfo.strPicturePath = pathAppender.SetSourcePath(GetStringValue(envVar::g_Domain, envVar::strResource_dirPath))
			.AppendChildPath("GLPictures")
			.AppendChildPath("joy.png")
			.GetPath();
		const char* cStrPicturePath = m_textureInfo.strPicturePath.c_str();

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// 为当前绑定的纹理对象设置环绕、过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// 加载并生成纹理
		int width, height, nrChannels;
		unsigned char* data = stbi_load(cStrPicturePath, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			m_textureInfo.uTextureId = texture;
		}
		else
		{
			LOG_ERROR << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
}

