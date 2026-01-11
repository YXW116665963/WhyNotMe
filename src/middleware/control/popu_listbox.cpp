#include "popu_listbox.h"
#include "ui_loader.h"
#include "ui_util.h"
#include "custom_combo_box.h"

namespace why
{
	PopuListBox::PopuListBox(wxWindow *parent)
					: wxFrame(parent, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxFRAME_TOOL_WINDOW)
	{
		Show(false);
		m_pListBox = new wxListBox(this, wxID_ANY, wxPoint(0, 0), wxDefaultSize, 0, nullptr, 0, wxDefaultValidator);
		m_pListBox->Bind(wxEVT_LISTBOX, &PopuListBox::OnSelectionChanged, this);
		Bind(wxEVT_SIZE, &PopuListBox::OnSize, this);
		//Bind(wxEVT_TIMER, &PopuListBox::OnTimer, this);

		//m_pTimer = new wxTimer(this, wxID_ANY);
		//m_pTimer->Start(100, wxTIMER_CONTINUOUS);
	}

	PopuListBox::~PopuListBox()
	{

	}

	void PopuListBox::ShowForTarget(wxWindow* pTarget, const wxSize &szSize)
	{
		wxSize			szPopulist;
		wxFont			ftText = GetFont();
		wxColour		crText = GetForegroundColour();
		
		m_pTarget = pTarget;
		
		m_pListBox->SetFont(ftText);
		m_pListBox->SetForegroundColour(crText);

		SetSize(szSize);
		ShowForTarget();
		m_pListBox->SetFocus();
	}

	void PopuListBox::OnSize(wxSizeEvent& event)
	{
		m_pListBox->SetSize(event.GetSize());
		event.Skip(true);
	}

	//void PopuListBox::OnTimer(wxTimerEvent& event)
	//{
	//	if (IsShown())
	//	{
	//		wxWindow*			pFocus = wxWindow::FindFocus();
	//
	//		if ((pFocus != this) || (pFocus != m_pListBox))
	//			Show(false);
	//	}
	//}
	//
	void PopuListBox::ShowForTarget()
	{
		wxRect          rcScreen;

		GetCurrentScreenInfo(rcScreen);
		if ((0 != rcScreen.width) && (0 != rcScreen.height))
		{
			wxPoint         ptTarget = m_pTarget->GetPosition();
			wxSize          szTarget = m_pTarget->GetSize();
			wxSize          szPopuList = GetSize();
			wxPoint         ptPopuList;
			wxWindow*       pParent = m_pTarget->GetParent();

			if (pParent)
				ptTarget = pParent->ClientToScreen(ptTarget);

			ptPopuList.x = ptTarget.x;
			ptPopuList.y = ptTarget.y + szTarget.y + 2;

			if ((ptPopuList.x + szPopuList.x) > (rcScreen.x + rcScreen.width))
				ptPopuList.x = rcScreen.x + rcScreen.width - szPopuList.x - 2;

			if ((ptPopuList.y + szPopuList.y) > (rcScreen.y + rcScreen.height))
				ptPopuList.y = ptTarget.y - szPopuList.y - 2;

			Show(false);
			Show(true);
			SetPosition(ptPopuList);
		}
	}

	void PopuListBox::OnSelectionChanged(wxCommandEvent& event)
	{
		CustomComboBox		*pComboBox = dynamic_cast<CustomComboBox *>(m_pTarget);

		if (nullptr != pComboBox)
		{
			pComboBox->SetSelection(m_pListBox->GetSelection());
			Show(false);
		}
	}
}