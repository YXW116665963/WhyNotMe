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

    public:

    private:
        void LoadMainFrame();
        void LoadFrame(const std::string& strFrameXml_filePath);
    private:
        wxTimer* m_pTimer{ nullptr };
    };
}

//MainTrayBar* GetMainTrayBar() { return m_pMainTrayBar; }
//int32_t                         m_nStatusHeight{ 80 };
//int32_t                         m_nToolBarHeight{ 100 };
//int32_t                         m_nSlidebBarWidth{ 180 };
//jca::MainStatusBar* m_pMainStatusBar{ nullptr };
//jca::MainTrayBar* m_pMainTrayBar{ nullptr };
//jca::IDataValue* m_pDataValue{ nullptr };