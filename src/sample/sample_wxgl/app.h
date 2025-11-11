#include "gl_util.h"
#include "wx/glcanvas.h"

// the rendering context used by all GL canvases
class TestGLContext : public wxGLContext
{
public:
    TestGLContext(wxGLCanvas* canvas, wxGLContextAttrs* cxtAttrs);

    // render the cube showing it at given angles
    void DrawRotatedCube(float xangle, float yangle);

private:
    // textures for the cube faces
    GLuint m_textures[6];
};

// Define a new application type
class MyApp : public wxApp
{
public:
    MyApp() { m_glContext = nullptr; }
    // Returns the shared context used by all frames and sets it as current for
    // the given canvas.
    TestGLContext& GetContext(wxGLCanvas* canvas);

    // virtual wxApp methods
    virtual bool OnInit() override;
    virtual int OnExit() override;

private:
    // the GL context we use for all our mono rendering windows
    TestGLContext* m_glContext;

    wxGLContextAttrs cxtAttrs;
};

// Define a new frame type
class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void OnClose(wxCommandEvent& event);
    void OnNewWindow(wxCommandEvent& event);
    void OnNewStereoWindow(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

class TestGLCanvas : public wxGLCanvas
{
public:
    TestGLCanvas(wxWindow* parent);

private:
    void OnPaint(wxPaintEvent& event);

    // angles of rotation around x- and y- axis
    float m_xangle,
        m_yangle;

    wxTimer m_spinTimer;

    wxDECLARE_EVENT_TABLE();
};

enum
{
    NEW_STEREO_WINDOW = wxID_HIGHEST
};


GLHandler m_glHandler;
