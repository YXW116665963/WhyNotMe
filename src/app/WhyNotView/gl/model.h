#pragma once
#include "glad/glad.h"
#include "util.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "wx/glcanvas.h"
#include "shader.h"
namespace why
{
	// 纹理信息
	struct TextureInfo
	{
		GLuint				uTextureId;
		std::string			strPicturePath;
	};

	class Model
	{
	public:
		Model(const GLuint& uVao,const GLuint& uVbo);
		~Model();

		void DrawTriangle(ShaderProgram* shader);
		void LoadTexture();
	private:
		TextureInfo m_textureInfo;
		
		//float vertices[18] = {
		//	-0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f,
		//	0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f,
		//	0.0f,  0.5f, 0.0f, 0.f, 0.f, 1.f
		//};

		//		 哈市雪花
		//float vertices[18] = {
		//	-0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,
		//	0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,
		//	0.0f,  0.5f, 1.f, 0.f, 0.f, 1.f
		//};

		// 哈市雪花纹理
		float vertices[24] = {
			-0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,     0.0f, 0.0f, // 左下角
			0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,      1.0f, 0.0f, // 右下角
			0.0f,  0.5f, 1.f, 0.f, 0.f, 1.f,      0.5f, 1.0f  // 上中
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