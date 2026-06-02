#pragma once

#include "gui_import.h"

#include <memory>

#include "data_observer_helper.h"

namespace why
{
    class MainFrame : public wxFrame
    {
    public:
        MainFrame(wxFrame* frame, wxWindowID id, const wxString& title,
            const wxPoint& pos, const wxSize& size, long style);
        virtual ~MainFrame();

    private:
        void OnCloseWindow(wxCloseEvent& event);
        void OnTimer(wxTimerEvent& event);
        void OnSize(wxSizeEvent& event);
        void LoadMainFrame();
        void LoadFrame(const std::string& strFrameXml_filePath);
    public:
        

    private:
        // 菜单条目结构体：存储一个菜单项的所有信息
        struct MenuItem
        {
            int id;                                             // 菜单ID
            const char* text;                                   // 菜单显示文字
            void (MainFrame::* handler)(wxCommandEvent&);       // 处理函数指针
        };

        void OnMenuOpen(wxCommandEvent& event);
        void OnMenuOpen2(wxCommandEvent& event);
        void CreateMenu();
    private:
        wxTimer* m_pTimer{ nullptr };
        wxSize m_lastSize;
    };
}