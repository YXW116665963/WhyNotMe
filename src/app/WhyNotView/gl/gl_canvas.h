#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "wx/glcanvas.h"

#include "child_panel.h"

namespace why
{
	class GLCanvas : public wxGLCanvas
	{
	public:
		GLCanvas(wxWindow* parent, const wxRect& rect);
		~GLCanvas();
	private:
		void OnPaint(wxPaintEvent& event);
		void InitGL();



		void Render();
	private:
		//保存你所有 OpenGL 状态、资源、绘制环境的对象
		//你创建的 VAO / VBO / Shader；纹理、缓冲区；状态机（清屏颜色、开启深度测试等
		wxGLContext* m_pContext;

		GLuint m_shaderProgram;

		/* VBO = 装顶点数据的 “显存盒子”
		作用：在显卡上开辟一块内存
		内容：存放顶点坐标（x,y,z）、颜色、法线等
		一句话：VBO = 显卡里的数据集
		*/
		GLuint m_vbo;

		/* VAO = 记录 “怎么画” 的配置单
		作用：保存 VBO 的格式、读取方式
		内容：数据从哪来？几个数一组？偏移多少？
		一句话：VAO = 绘画配置文件
		*/
		GLuint m_vao;
	};
}

