#include "glad/glad.h"//#include glad.h必须放在所有头文件之前，不管在哪里，不管在.h还是.cpp;
#include "gl_canvas.h"
#include "wx/wx.h"
#include "logger.h"
#include "viewer_setting.h"
#include "camera.h"
namespace why
{
	GLCanvas::GLCanvas(wxWindow* parent, const wxRect& rect)
		: wxGLCanvas(parent, wxID_ANY, nullptr, rect.GetPosition(), rect.GetSize(), wxFULL_REPAINT_ON_RESIZE)
		, m_fCameraPitch(0.0f)
		, m_fCameraYaw(-90.0f)
		, m_bFirstMouse(true)
		, m_fFov(45.0)
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

		m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		Init();
		Bind(wxEVT_PAINT, &GLCanvas::OnPaint, this);
		Bind(wxEVT_LEFT_DOWN, &GLCanvas::OnLeftDown, this);
		Bind(wxEVT_LEFT_UP, &GLCanvas::OnLeftUp, this);
		Bind(wxEVT_MOTION, &GLCanvas::OnMouseMove, this);

		Bind(wxEVT_KEY_DOWN, &GLCanvas::OnKeyDown, this);
		// 让窗口能接收键盘焦点，否则收不到按键
		SetFocus();
		// 允许窗口捕获键盘
		SetCanFocus(true);


		Bind(wxEVT_MOUSEWHEEL, &GLCanvas::OnMouseWheel, this);

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
		//// 即使 dc 没有被其他地方使用，这也是必需的。
		//wxPaintDC dc(this);
		Render();
	}

	// 获取主显示器缩放因子，等效 Qt devicePixelRatio
	double GetGlobalDevicePixelRatio()
	{
		wxDisplay primaryDisplay; // 0 = 主屏
		double res = primaryDisplay.GetScaleFactor();
		return res;
	}


	void GLCanvas::Init()
	{
		ViewerSetting::devicePixelRatio = GetGlobalDevicePixelRatio();
		InitOpenGLFunc();

		ShaderProgram* pShaderProgram;

		std::string strShaderProgramName = "triangle";

		if (CreateShader(strShaderProgramName, pShaderProgram))
		{
			m_ptrShaderProgram.reset(pShaderProgram);
			m_ptrShaderProgram->Load();
		}
		
		m_ptrTriangleModel.reset(new TriangleModel(m_vao, m_vbo));
	}

	void GLCanvas::InitOpenGLFunc()
	{		
		gladLoadGL();// 加载 OpenGL 函数 → GLAD 必须在这里调用

		// 设置视口
		wxRect rect = GetRect();		
		glViewport(rect.x, rect.y, rect.width, rect.height);

		// 创建 VAO 和 VBO
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		// 绑定刚刚创建的 VAO 和 VBO到opengl上下文
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	}

	void GLCanvas::Render()
	{
		//wxGLCanvas 的官方文档明确要求：在每次绘制开始时，必须重新设置当前上下文。
		SetCurrent(*m_pContext);
		//每次画新一帧画面之前，都要把上一帧的画面擦掉
		//glClearColor 就是设置：擦除后，屏幕变成什么颜色
		//红绿蓝、透明度
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//用 glClearColor 设置好的颜色，把整个屏幕刷一遍，擦掉上一帧的所有画面！
		//GL_COLOR_BUFFER_BIT = 颜色缓冲区
		glClear(GL_COLOR_BUFFER_BIT);

		m_ptrShaderProgram->Use();

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

		glm::vec3 front;
		front.x = cos(glm::radians(m_fCameraYaw)) * cos(glm::radians(m_fCameraPitch));
		front.y = sin(glm::radians(m_fCameraPitch));
		front.z = sin(glm::radians(m_fCameraYaw)) * cos(glm::radians(m_fCameraPitch));
		m_cameraFront = glm::normalize(front);
		view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
		glm::mat4 projection = glm::perspective(glm::radians(m_fFov), (float)1800 / 1000, 0.1f, 100.0f);

		m_ptrShaderProgram->SetMat4f("model", model);
		m_ptrShaderProgram->SetMat4f("view", view);
		m_ptrShaderProgram->SetMat4f("projection", projection);

		m_ptrTriangleModel->Draw();
		SwapBuffers();
	}

	void GLCanvas::OnKeyDown(wxKeyEvent& event)
	{
		int key = event.GetKeyCode();
		float cameraSpeed = 0.05f; // adjust accordingly
		switch (key)
		{
		case 'W':
		case 'w':
			// W 按下逻辑
			m_cameraPos += cameraSpeed * m_cameraFront;
			break;
		case 'S':
		case 's':
			// S 按下逻辑
			m_cameraPos -= cameraSpeed * m_cameraFront;
			break;
		case 'A':
		case 'a':
			// A 按下逻辑
			m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
			break;

		case 'D':
		case 'd':
			// D 按下逻辑
			m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
			break;
		default:
			event.Skip(); // 其他按键交给系统处理
			return;
		}

		Render();


		event.Skip(false); // 消费掉WASD按键，不向上传递
	}

	void GLCanvas::OnLeftDown(wxMouseEvent& event)
	{
		m_bLeftDown = true;
		m_lastPoint = event.GetPosition(); // 记录起点
	}

	void GLCanvas::OnLeftUp(wxMouseEvent& event)
	{
		m_bLeftDown = false;

	}

	void GLCanvas::OnMouseMove(wxMouseEvent& event)
	{
		if (m_bLeftDown && event.Dragging())
		{
			wxPoint currentPos = event.GetPosition();

			if (m_bFirstMouse)
			{
				//疑似没啥用
				m_lastPoint = currentPos;
				m_bFirstMouse = false;
			}


			float deltaX = currentPos.x - m_lastPoint.x;
			float deltaY = m_lastPoint.y - currentPos.y;

			m_lastPoint = currentPos;

			 m_fCameraYaw	+=	deltaX * 0.1f;
			 m_fCameraPitch +=	deltaY * 0.1f;

			 if (m_fCameraPitch > 89.0f)
				 m_fCameraPitch = 89.0f;
			 if (m_fCameraPitch < -89.0f)
				 m_fCameraPitch = -89.0f;

			 Render();
		}
	}

	void GLCanvas::OnMouseWheel(wxMouseEvent& event)
	{
		// 滚轮旋转量：正数向上，负数向下
		int rot = event.GetWheelRotation();
		int delta = event.GetWheelDelta();
		float step = 0.5f; // 每次滚动缩放步长

		if (rot > 0)
		{
			// 滚轮向上：放大
			m_fFov += step;
		}
		else
		{
			// 滚轮向下：缩小
			m_fFov -= step;
		}



		// 刷新画面（OpenGL绘图/双缓冲重绘）
		Render();

		// 消费事件，不传递给父窗口
		event.Skip(false);
	}
}