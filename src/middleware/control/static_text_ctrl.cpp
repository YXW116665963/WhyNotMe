#include "static_text_ctrl.h"
#include "singleton.h"
#include "resource_manager.h"

namespace why
{
	wxIMPLEMENT_CLASS(StaticTextCtrl, wxStaticText);

	StaticTextCtrl::StaticTextCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos,const wxSize& size, uint32_t uStyle, const wxString& name)
        : wxStaticText(parent, id, wxT(""), pos, size, uStyle | wxALIGN_CENTER_VERTICAL, name),
		m_fullLabel(wxT(""))
    {
		Bind(wxEVT_ENTER_WINDOW, &StaticTextCtrl::OnMouseEnter, this);
		Bind(wxEVT_LEAVE_WINDOW, &StaticTextCtrl::OnMouseLeave, this);
		Bind(wxEVT_MOTION, &StaticTextCtrl::OnMouseMove, this);
    }

    void StaticTextCtrl::SetBackground(const std::string& strBackground)
    {
        bool            bNullptr = (nullptr == m_backgroundPtr.get());
        
		m_backgroundPtr = Singleton<ResourceMgr>::Instance()->GetBackground(strBackground.c_str());
        if (nullptr != m_backgroundPtr.get())
        {
			if (bNullptr)
				Bind(wxEVT_ERASE_BACKGROUND, &StaticTextCtrl::OnEraseBackground, this);
        
            Refresh();
        }
        else
        {
			if (!bNullptr)
				Refresh();
        }
    }

	void StaticTextCtrl::OnMouseEnter(wxMouseEvent& event)
	{
		m_fullLabel = GetLabel();
		wxSize LabelSize = GetTextExtent(m_fullLabel);
		if (LabelSize.GetWidth()> GetSize().GetWidth())
		{
			SetToolTip(m_fullLabel);
		}
		event.Skip();
	}

	void StaticTextCtrl::OnMouseLeave(wxMouseEvent& event)
	{
		UnsetToolTip();
		event.Skip();
	}

	void StaticTextCtrl::OnMouseMove(wxMouseEvent& event)
	{
		event.Skip();
	}

    void StaticTextCtrl::OnEraseBackground(wxEraseEvent& event)
    {
        wxClientDC		dc(this);
        wxSize			szClient = GetClientSize();

        dc.SetBackground(*wxTRANSPARENT_BRUSH);
        dc.Clear();

        m_backgroundPtr->Draw(dc, wxRect(szClient));

        event.Skip(false);
    }

}