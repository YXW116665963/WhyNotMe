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
        void InitMenu();


        //File
        void OnMenuOpenFile(wxCommandEvent& event);
        void OnMenuSaveFile(wxCommandEvent& event);

        //Test
        void OnMenuRepaintPage(wxCommandEvent& event);

    private:        





    private:
        wxTimer* m_pTimer{ nullptr };
        wxSize m_lastSize;


    };
}