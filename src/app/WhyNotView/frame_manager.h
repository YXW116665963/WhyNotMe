#pragma once

#include "gui_import.h"
#include "child_panel.h"
#include <string>
#include <list>
#include "panel_factory.h"
#include "main_frame.h"

// 指定Frame模块的控件绑定响应函数
#define BIND_FUNC_TO_FRAME_BY_WND(eModule, ptr, key, control, func)\
ptr = dynamic_cast<control *>(Singleton<FrameManager>::Instance()->FindPanelItem(eModule, key));\
if (nullptr != ptr){ptr->Bind(wxEVT_BUTTON, &func, this);}

// 根据关键字绑定Frame控件的响应函数（关键字，函数）
#define BIND_FUNC_TO_FRAME_BY_KEY(eModule, key, func)\
{wxWindow* pWnd = Singleton<FrameManager>::Instance()->FindPanelItem(eModule, key); \
if (pWnd) { pWnd->Bind(wxEVT_BUTTON, &func, this); }}

#define BIND_BUTTON_TO_FRAME_MODULE(eModule, key, func)\
	Singleton<FrameManager>::Instance()->BindEvent(eModule, key, wxEVT_BUTTON, std::bind(func, this, std::placeholders::_1));

namespace why
{
	wxDECLARE_EVENT(JCA_CONFIRM_CLICK_EVENT, wxCommandEvent);

    class FrameManager
    {
    private:
        struct WindowInfo
        {
            std::string                 m_strWindowName;
			std::vector<ChildPanel*>    m_vecPanels;
            bool				        m_bReadOnly{ false };
        };
    public:
        void SetMainFrame(wxFrame* pMainFrame) { m_pMainFrame = pMainFrame; };
        wxFrame* GetMainFrame() { return m_pMainFrame; };

    public:
        FrameManager();
        ~FrameManager();

        void OpenWindow(const std::string& strWindowName);
		void CloseWindow(const WindowInfo& windowInfo);

        // 响应窗口大小变化@why.todo
        void OnSize();
    private:

        WindowInfo m_curWindowInfo;

        typedef std::map<std::string, IPanelFactory*> WindowFactoryMap;
		WindowFactoryMap m_mapPanelFactory;

    private:
        wxFrame*                    m_pMainFrame{ nullptr };        
        wxRect                      m_rcMainWnd;
    };
}
