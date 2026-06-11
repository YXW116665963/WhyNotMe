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

		m_ptrCamera.reset(new Camera(std::bind(&GLCanvas::Render, this)));
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
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		m_ptrShaderProgram->Use();

		// 模型
		//glm::rotate作用：生成一个旋转矩阵，并应用到输入矩阵上
		// 绕X轴负向55°，等同于头向上仰，vec3(1.0f, 0.0f, 0.0f)表示x轴
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// 视图
		glm::mat4 view = m_ptrCamera->GetViewMat();
		float32_t fZoom = m_ptrCamera->GetZoom();
		// 投影
		glm::mat4 projection = glm::perspective(glm::radians(fZoom), (float)GetRect().width / GetRect().height, 0.1f, 100.0f);

		m_ptrShaderProgram->SetMat4f("model", model);
		m_ptrShaderProgram->SetMat4f("view", view);
		m_ptrShaderProgram->SetMat4f("projection", projection);

		m_ptrTriangleModel->Draw();
		SwapBuffers();
	}

	void GLCanvas::OnKeyDown(wxKeyEvent& event)
	{
		m_ptrCamera->HandleEvent(event);
		event.Skip(false);
	}

	void GLCanvas::OnLeftDown(wxMouseEvent& event)
	{
		m_ptrCamera->HandleEvent(event);
		event.Skip(false);
	}

	void GLCanvas::OnLeftUp(wxMouseEvent& event)
	{
		m_ptrCamera->HandleEvent(event);
		event.Skip(false);
	}

	void GLCanvas::OnMouseMove(wxMouseEvent& event)
	{
		m_ptrCamera->HandleEvent(event);
		event.Skip(false);
	}

	void GLCanvas::OnMouseWheel(wxMouseEvent& event)
	{
		m_ptrCamera->HandleEvent(event);
		event.Skip(false);
	}
}