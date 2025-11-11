#include "glad/glad.h"//#include glad.h必须放在所有头文件之前，不管在哪里，不管在.h还是.cpp;
#include "wx/wx.h"
#include "app.h"


// control ids
enum
{
    SpinTimer = wxID_HIGHEST
};

static void CheckGLError()
{
    GLenum errLast = GL_NO_ERROR;

    for (;; )
    {
        GLenum err = glGetError();
        if (err == GL_NO_ERROR)
            return;
        if (err == errLast)
        {
            wxLogError("OpenGL error state couldn't be reset.");
            return;
        }

        errLast = err;

        wxLogError("OpenGL error %d", err);
    }
}
// ----------------------------------------------------------------------------
// TestGLContext
// ----------------------------------------------------------------------------

TestGLContext::TestGLContext(wxGLCanvas* canvas, wxGLContextAttrs* cxtAttrs)
    : wxGLContext(canvas,nullptr,cxtAttrs)
{
    //- 指定画布的当前上下文
    bool flag = SetCurrent(*canvas);

    char* procAddress;
    //- 加载gl函数指针
    gladLoadGL();

    m_glHandler.GLPrepare();
    CheckGLError();
}

void TestGLContext::DrawRotatedCube(float xangle, float yangle)
{
    
}


// ----------------------------------------------------------------------------
// MyApp: the application object
// ----------------------------------------------------------------------------

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    new MyFrame();

    return true;
}

int MyApp::OnExit()
{
    delete m_glContext;

    return wxApp::OnExit();
}

TestGLContext& MyApp::GetContext(wxGLCanvas* canvas)
{
    TestGLContext* glContext;

    if (!m_glContext)
    {        
        //- 类似于glfw的初始化并指定版本等函数，为啥要放在窗口框架层来做？
        cxtAttrs.CoreProfile()
            .OGLVersion(4, 5)
            .Robust()
            .ResetIsolation()
            .EndList();
        cxtAttrs.SetNeedsARB();
        m_glContext = new TestGLContext(canvas, &cxtAttrs);
    }
    glContext = m_glContext;

    glContext->SetCurrent(*canvas);

    return *glContext;
}

// ----------------------------------------------------------------------------
// TestGLCanvas
// ----------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(TestGLCanvas, wxGLCanvas)
EVT_PAINT(TestGLCanvas::OnPaint)
wxEND_EVENT_TABLE()

TestGLCanvas::TestGLCanvas(wxWindow* parent)
    : m_xangle(30.0),
    m_yangle(30.0),
    m_spinTimer(this, SpinTimer)
{
    wxGLAttributes attribs = wxGLAttributes().Defaults();
    attribs.EndList();

    if (!wxGLCanvas::Create(parent, attribs, wxID_ANY,
        wxDefaultPosition, wxDefaultSize,
        wxFULL_REPAINT_ON_RESIZE))
    {
        wxLogError("Creating OpenGL window failed.");
    }
}

void TestGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    // 即使 dc 没有被其他地方使用，这也是必需的。
    wxPaintDC dc(this);

    // 根据画布大小修改gl视口大小,onSize不适合
    const wxSize ClientSize = GetClientSize() * GetContentScaleFactor();

    TestGLContext& canvas = wxGetApp().GetContext(this);
    glViewport(0, 0, ClientSize.x, ClientSize.y);

    //- draw
    m_glHandler.GLWhileDo();


    // Render the graphics and swap the buffers.
    canvas.DrawRotatedCube(m_xangle, m_yangle);
    SwapBuffers();
}

wxString glGetwxString(GLenum name)
{
    const GLubyte* v = glGetString(name);
    if (v == nullptr)
    {
        glGetError();

        return wxString();
    }

    return wxString((const char*)v);
}


// ----------------------------------------------------------------------------
// MyFrame: main application window
// ----------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(wxID_NEW, MyFrame::OnNewWindow)
EVT_MENU(wxID_CLOSE, MyFrame::OnClose)
wxEND_EVENT_TABLE()

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "wxWidgets OpenGL Cube Sample")
{
    new TestGLCanvas(this);

    // Make a menubar
    wxMenu* menu = new wxMenu;
    menu->Append(wxID_NEW);
    menu->Append(NEW_STEREO_WINDOW, "New Stereo Window");
    menu->AppendSeparator();
    menu->Append(wxID_CLOSE);
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menu, "&Cube");

    SetMenuBar(menuBar);
    
    CreateStatusBar();

    SetClientSize(400, 400);
    Show();

    // test IsDisplaySupported() function:
    wxGLAttributes attribs;
    attribs.RGBA().DoubleBuffer().EndList();
    wxLogStatus("Double-buffered display %s supported",
        wxGLCanvas::IsDisplaySupported(attribs) ? "is" : "not");
}

void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnNewWindow(wxCommandEvent& WXUNUSED(event))
{
    new MyFrame();
}