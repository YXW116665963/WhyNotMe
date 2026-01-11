#pragma once

#include "gui_import.h"
#include "image_button.h"
#include "panel_graphic.h"
#include "wx/calctrl.h"
#include <map>

namespace why
{
    class PopuDateBox : public wxFrame
    {
    public:
		PopuDateBox(wxWindow *parent);
        ~PopuDateBox();

		void SetDateTime(wxDateTime time);
		void ShowForTarget(wxWindow* pTarget);

	private:
		void OnClick(wxCalendarEvent& event);

    private:
		wxWindow*					m_pTarget{ nullptr };
		wxCalendarCtrl*             m_pCalendarCtrl{ nullptr };
    };
}
