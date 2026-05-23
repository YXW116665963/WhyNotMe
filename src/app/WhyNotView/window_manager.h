#pragma once

#include "gui_import.h"
#include "child_panel.h"
#include <string>
#include <list>
#include "window_factory.h"
#include "main_frame.h"

// 指定Frame模块的控件绑定响应函数
#define BIND_FUNC_TO_FRAME_BY_WND(eModule, ptr, key, control, func)\
ptr = dynamic_cast<control *>(Singleton<WindowManager>::Instance()->FindPanelItem(eModule, key));\
if (nullptr != ptr){ptr->Bind(wxEVT_BUTTON, &func, this);}

// 根据关键字绑定Frame控件的响应函数（关键字，函数）
#define BIND_FUNC_TO_FRAME_BY_KEY(eModule, key, func)\
{wxWindow* pWnd = Singleton<WindowManager>::Instance()->FindPanelItem(eModule, key); \
if (pWnd) { pWnd->Bind(wxEVT_BUTTON, &func, this); }}

#define BIND_BUTTON_TO_FRAME_MODULE(eModule, key, func)\
	Singleton<WindowManager>::Instance()->BindEvent(eModule, key, wxEVT_BUTTON, std::bind(func, this, std::placeholders::_1));

namespace why
{
	wxDECLARE_EVENT(JCA_CONFIRM_CLICK_EVENT, wxCommandEvent);

    class WindowManager
    {
    private:
        struct WindowInfo
        {
            std::string         m_strWindowName;
            ChildPanel*         m_pPanel{ nullptr };
            bool				m_bReadOnly{ false };
        };
    public:
        void SetMainFrame(wxFrame* pMainFrame) { m_pMainFrame = pMainFrame; };
        wxFrame* GetMainFrame() { return m_pMainFrame; };

    public:
        WindowManager();
        ~WindowManager();

        void OpenWindow(const std::string& strWindowName);
		void CloseWindow(const WindowInfo& windowInfo);

        // 响应窗口大小变化@why.todo
        void OnSize();
    private:

        WindowInfo m_curWindowInfo;

        typedef std::map<std::string, IFrameFactory*> WindowFactoryMap;
		WindowFactoryMap m_mapWindowFactory;

    private:
        wxFrame*                    m_pMainFrame{ nullptr };        
        wxRect                      m_rcMainWnd;
    };
}
