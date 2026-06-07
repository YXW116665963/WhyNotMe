#include "glad/glad.h"//#include glad.h必须放在所有头文件之前，不管在哪里，不管在.h还是.cpp;
#include "gl_canvas.h"
#include "wx/wx.h"
#include "logger.h"

namespace why
{
	GLCanvas::GLCanvas(wxWindow* parent, const wxRect& rect)
		: wxGLCanvas(parent, wxID_ANY, nullptr, rect.GetPosition(), rect.GetSize(), wxFULL_REPAINT_ON_RESIZE)
	{
		//- 类似于glfw的初始化并指定版本等函数
		wxGLContextAttrs cxtAttrs;
		cxtAttrs.CoreProfile()				// 启用Core核心模式
			.OGLVersion(4, 5)               // 硬性要求OpenGL 4.5
			.Robust()                       // 健壮访问：越界缓冲区不直接程序崩溃
			.ResetIsolation()               // GPU显存重置隔离，一个窗口崩不影响其他GL上下文
			.EndList();                     // 属性列表结束，必须加，否则创建失败黑屏
		cxtAttrs.SetNeedsARB();				// 启用ARB扩展创建上下文（WGL_ARB_create_context）

		m_pContext = new wxGLContext(this, nullptr, &cxtAttrs);
		SetCurrent(*m_pContext);

		InitGL();
		Bind(wxEVT_PAINT, &GLCanvas::OnPaint, this);
	}

	GLCanvas::~GLCanvas()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteProgram(m_shaderProgram);
		delete m_pContext;
	}

	void GLCanvas::OnPaint(wxPaintEvent& event)
	{
		//wxGLCanvas 的官方文档明确要求：在每次绘制开始时，必须重新设置当前上下文。
		SetCurrent(*m_pContext);
		//// 即使 dc 没有被其他地方使用，这也是必需的。
		//wxPaintDC dc(this);
		Render();
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

	const char* vertexShaderSource =
		"#version 450 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aNormal;\n"
		"void main() {\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 450 core\n"
		"out vec4 FragColor;\n"                                             //输出：最终像素颜色
		"void main() {\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // 橙色            //格式：(R, G, B, A) → 红、绿、蓝、透明度
		"}\0";



	void GLCanvas::InitGL()
	{
		// 加载 OpenGL 函数 → GLAD 必须在这里调用
		gladLoadGL();


		wxRect rect = GetRect();
		// 设置视口
		glViewport(rect.x, rect.y, rect.width, rect.height);

		// 立方体顶点,所有的值是在[-1, 1]之间的
		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f,
			0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f,
			0.0f,  0.5f, 0.0f, 0.f, 0.f, 1.f
		};

		// 创建 VAO 和 VBO
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		// 绑定刚刚创建的 VAO 和 VBO到opengl上下文
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		// 将顶点数据赋值给VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// 设置线宽
		glLineWidth(1);

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

		// 编译 shader
		// 指定编译顶点着色器
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertexShaderSource, nullptr);
		glCompileShader(vs);
		// 检查顶点着色器编译错误
		int success;
		char infoLog[512];
		glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vs, 512, NULL, infoLog);
			LOG_ERROR << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}


		// 指定编译片段着色器
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fs);
		glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fs, 512, NULL, infoLog);
			LOG_ERROR << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// 链接GPU程序
		m_shaderProgram = glCreateProgram();
		glAttachShader(m_shaderProgram, vs);
		glAttachShader(m_shaderProgram, fs);
		glLinkProgram(m_shaderProgram);

		// 检查着色器程序链接错误
		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
			LOG_ERROR << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	void GLCanvas::Render()
	{
		//每次画新一帧画面之前，都要把上一帧的画面擦掉
		//glClearColor 就是设置：擦除后，屏幕变成什么颜色
		//红绿蓝、透明度
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		//用 glClearColor 设置好的颜色，把整个屏幕刷一遍，擦掉上一帧的所有画面！
		//GL_COLOR_BUFFER_BIT = 颜色缓冲区
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_shaderProgram);

		// 模型、视图和投影矩阵
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);


		// 设置模型、视图和投影矩阵
		// glGetUniformLocation：给着色器中对应变量赋值
		//GLuint modelLoc = glGetUniformLocation(m_shaderProgram, "model");
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//GLuint viewLoc = glGetUniformLocation(m_shaderProgram, "view");
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//GLuint projectionLoc = glGetUniformLocation(m_shaderProgram, "projection");
		//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glBindVertexArray(m_vao);
		//图元装配
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		SwapBuffers();
	}
}