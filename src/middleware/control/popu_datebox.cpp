#include "popu_datebox.h"
#include "ui_loader.h"
#include "ui_util.h"
#include "custom_combo_box.h"

namespace why
{
	PopuDateBox::PopuDateBox(wxWindow *parent)
					: wxFrame(parent, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxFRAME_TOOL_WINDOW)
	{
		Show(false);

		m_pCalendarCtrl = new wxCalendarCtrl(this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxCAL_SHOW_HOLIDAYS|wxNO_BORDER);
		m_pCalendarCtrl->Bind(wxEVT_CALENDAR_SEL_CHANGED, &PopuDateBox::OnClick, this);

		SetSize(m_pCalendarCtrl->GetSize());
	}

	PopuDateBox::~PopuDateBox()
	{

	}

	void PopuDateBox::SetDateTime(wxDateTime time)
	{
		m_pCalendarCtrl->SetDate(time);
	}

	void PopuDateBox::ShowForTarget(wxWindow* pTarget)
	{
		wxRect          rcScreen;

		wxWindow* pTargetOld = m_pTarget;
		m_pTarget = pTarget;

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

			if (m_pTarget != pTargetOld) {
				Show(false);
			}

			Show(true);
			SetPosition(ptPopuList);

			m_pCalendarCtrl->SetFocus();
		}
	}

	void PopuDateBox::OnClick(wxCalendarEvent& event)
	{
		CustomComboBox		*pComboBox = dynamic_cast<CustomComboBox *>(m_pTarget);

		if (pComboBox) 
		{
			//<! 出发选择事件
			pComboBox->SetDateTime(event.GetDate());
			pComboBox->SetSelection(pComboBox->GetSelection() - 1);
		}
	}
}