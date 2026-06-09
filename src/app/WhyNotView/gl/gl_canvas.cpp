#include "glad/glad.h"//#include glad.h必须放在所有头文件之前，不管在哪里，不管在.h还是.cpp;
#include "gl_canvas.h"
#include "wx/wx.h"
#include "logger.h"
#include "viewer_setting.h"
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
		ModelInfo modelInfo(m_vao,m_vbo,0);
		m_ptrTriangleModel.reset(new TriangleModel(modelInfo));

		m_shaderProgram = modelInfo.uShaderProgramId;
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
		m_ptrTriangleModel->Draw(m_shaderProgram);
		SwapBuffers();
	}
}