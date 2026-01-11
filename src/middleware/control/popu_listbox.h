#pragma once

#include "gui_import.h"
#include "image_button.h"
#include "panel_graphic.h"
#include <map>

namespace why
{
    class PopuListBox : public wxFrame
    {
    public:
		PopuListBox(wxWindow *parent);
        ~PopuListBox();

		void Clear() { m_pListBox->Clear(); }
		void Append(const wxString& item) { m_pListBox->Append(item); }
		void ShowForTarget(wxWindow* pTarget, const wxSize &szSize);

	private:
		void OnSize(wxSizeEvent& event);
		//void OnTimer(wxTimerEvent& event);
		void OnSelectionChanged(wxCommandEvent& event);

	private:
		void ShowForTarget();

    private:
		//wxTimer*					m_pTimer{ nullptr };
		wxListBox*					m_pListBox{ nullptr };
		wxWindow*					m_pTarget{ nullptr };
    };
}
