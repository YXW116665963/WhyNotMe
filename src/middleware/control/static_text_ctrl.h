#pragma once

#include "gui_import.h"
#include "background_interface.h"

namespace why
{
    class StaticTextCtrl : public wxStaticText
    {
    public:
		StaticTextCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, uint32_t uStyle, const wxString& name);
      
		wxDECLARE_CLASS(StaticTextCtrl);

	public:
        void SetBackground(const std::string& strBackground);
        void OnEraseBackground(wxEraseEvent& event);
		void OnMouseEnter(wxMouseEvent& event);
		void OnMouseLeave(wxMouseEvent& event);
		void OnMouseMove(wxMouseEvent& event);

    private:
        BackgroundPtr           m_backgroundPtr;
		wxString				m_fullLabel;
    };
}
