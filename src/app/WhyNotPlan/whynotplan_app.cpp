#include "wx/wx.h"
#include "wx/notebook.h"

// 主窗口类
class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title);

private:
    wxNotebook* m_notebook; // 用于切换界面的 notebook 控件
};

// 第一个页面类
class Page1 : public wxPanel
{
public:
    Page1(wxNotebook* parent);

private:
    wxButton* m_button1;
};

// 第二个页面类
class Page2 : public wxPanel
{
public:
    Page2(wxNotebook* parent);

private:
    wxButton* m_button2;
};

// 应用程序类
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

// 主窗口的构造函数
MyFrame::MyFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(500, 400))
{
    m_notebook = new wxNotebook(this, wxID_ANY);

    // 创建两个页面，并添加到 notebook
    Page1* page1 = new Page1(m_notebook);
    Page2* page2 = new Page2(m_notebook);

    m_notebook->AddPage(page1, "Page 1");
    m_notebook->AddPage(page2, "Page 2");
}

// 第一个页面的构造函数
Page1::Page1(wxNotebook* parent)
    : wxPanel(parent)
{
    m_button1 = new wxButton(this, wxID_ANY, "Go to Page 2", wxPoint(10, 10));

    // 按钮点击事件，切换到第二个页面
    m_button1->Bind(wxEVT_BUTTON, [parent](wxCommandEvent& event) {
        parent->SetSelection(1); // 切换到第二个标签页
        });
}

// 第二个页面的构造函数
Page2::Page2(wxNotebook* parent)
    : wxPanel(parent)
{
    m_button2 = new wxButton(this, wxID_ANY, "Go to Page 1", wxPoint(10, 10));

    // 按钮点击事件，切换到第一个页面
    m_button2->Bind(wxEVT_BUTTON, [parent](wxCommandEvent& event) {
        parent->SetSelection(0); // 切换到第一个标签页
        });
}

// 应用程序初始化
bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame("wxWidgets Page Switch Example");
    frame->Show(true);
    return true;
}

// 程序入口点
wxIMPLEMENT_APP(MyApp);
