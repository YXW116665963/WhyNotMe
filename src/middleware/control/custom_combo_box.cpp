#include "custom_combo_box.h"
#include "singleton.h"
#include "resource_manager.h"
#include "popu_datebox.h"

namespace why
{
	namespace
	{
		PopuListBox		*g_pListBoxPart = nullptr;
		PopuDateBox     *g_pDateBoxPart = nullptr;

		enum class DeviceModel
		{
			e9000 = 0,		//!< 9000
			e6100,			//!< 6100	
			e8000,			//!< 8000	
			e7000,			//!< 7000	
			e6000,			//!< 6000 使用3000的UI
			e3000,			//!< 3000
			e9000W,			//!< 9000W 使用9000的UI
		};
	}

	wxIMPLEMENT_CLASS(CustomComboBox, wxWindow);

	extern IDataCenter* g_pMiddlewareDataCenter;

	CustomComboBox::CustomComboBox(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
			: wxWindow(parent, id, pos, size, style, name)
	{
		m_crBackground = GetBackgroundColour();
		m_brBKBrush = wxBrush(m_crBackground);
		m_dateTime = wxDateTime::Now();

		Bind(wxEVT_ENTER_WINDOW, &CustomComboBox::OnMouseEnter, this);
		Bind(wxEVT_LEAVE_WINDOW, &CustomComboBox::OnMouseLeave, this);
		Bind(wxEVT_MOTION, &CustomComboBox::OnMouseMove, this);

		m_dataExchangePtr.reset(new DataExchange(g_pMiddlewareDataCenter));
	}

	CustomComboBox::~CustomComboBox()
	{

	}

	void CustomComboBox::OnMouseEnter(wxMouseEvent& event)
	{
		wxClientDC		dc(this);
		if (m_nSelected >= 0 && GetOptionsSize() != 0 && m_nSelected < GetOptionsSize()&& dc.GetTextExtent(m_options[m_nSelected].first).GetWidth() > m_rcClient.width)
		{
			SetToolTip(m_options[m_nSelected].first);
		}
		event.Skip();
	}

	void CustomComboBox::OnMouseLeave(wxMouseEvent& event)
	{
		UnsetToolTip();
		event.Skip();
	}

	void CustomComboBox::OnMouseMove(wxMouseEvent& event)
	{
		event.Skip();
	}

	bool CustomComboBox::ProcessEvent(wxEvent& event)
	{
		wxEventType		eventType = event.GetEventType();

		do
		{
			if (wxEVT_PAINT == eventType)
			{
				OnPaint(dynamic_cast<wxPaintEvent&>(event));
				break;
			}

			if (wxEVT_ERASE_BACKGROUND == eventType)
			{
				OnEraseBackground(dynamic_cast<wxEraseEvent&>(event));
				break;
			}

			if (wxEVT_SIZE == eventType)
			{
				OnSize(dynamic_cast<wxSizeEvent&>(event));
				break;
			}

			if (wxEVT_LEFT_UP == eventType)
			{
				OnLButtonUp(dynamic_cast<wxMouseEvent&>(event));
				break;
			}

			if (wxEVT_LEFT_DOWN == eventType)
			{
				OnLButtonDown(dynamic_cast<wxMouseEvent&>(event));
				break;
			}

			if (wxEVT_ENTER_WINDOW == eventType)
			{
				OnEnterWindow(dynamic_cast<wxMouseEvent&>(event));
				break;
			}

			if (wxEVT_LEAVE_WINDOW == eventType)
			{
				OnLeaveWindow(dynamic_cast<wxMouseEvent&>(event));
				break;
			}
		} while (false);

		return wxEvtHandler::ProcessEvent(event);
	}

	void CustomComboBox::SetSelection(int32_t nSelected)
	{
		if (m_nSelected != nSelected)
		{
			m_nSelected = nSelected;
			Refresh(false);

			wxCommandEvent		selectChangedEvent(wxEVT_COMBOBOX, GetId());

			selectChangedEvent.SetEventObject(this);
			wxPostEvent(this, selectChangedEvent);
		}
	}

	void CustomComboBox::SetDateTime(wxDateTime time)
	{
		m_dateTime = time;
		Refresh(false);
	};

	uint32_t CustomComboBox::GetCurrentData() const
	{
		if ((m_nSelected >= 0) && (m_nSelected < (int32_t)m_options.size()))
			return m_options[m_nSelected].second;
		else
			return 0;
	}

	wxString CustomComboBox::GetCurrentText() const
	{
		if ((m_nSelected >= 0) && (m_nSelected < (int32_t)m_options.size()))
		{
			return m_options[m_nSelected].first;
		}
		return "";
	}

	wxString CustomComboBox::GetTextByUserData(uint32_t uUserData) const
	{
		int32_t nSize = (int32_t)m_options.size();

		for (int32_t i = 0; i < nSize; i++)
		{
			if (uUserData == m_options[i].second)
			{
				return m_options[i].first;
			}
		}
		return "";
	}

	std::vector<wxString> CustomComboBox::GetAllText() const
	{
		std::vector<wxString> optionsText;
		int32_t nSize = (int32_t)m_options.size();

		for (int32_t i = 0; i < nSize; i++)
		{
			optionsText.push_back(m_options[i].first);
		}
		return optionsText;
	}

	void CustomComboBox::SetSelectionByUserData(uint32_t uUserData)
	{
		int32_t			nSize = (int32_t)m_options.size();

		for (int32_t i = 0; i < nSize; i++)
		{
			if (uUserData == m_options[i].second)
			{
				if (m_nSelected != i)
				{
					m_nSelected = i;
					Refresh();
				}
				break;
			}
		}
	}

	int32_t CustomComboBox::Append(const wxString& item, uint32_t uUserData)
	{
		int32_t				nSize = (int32_t)m_options.size();

		m_options.push_back(std::make_pair(item, uUserData));
		return nSize;
	}

	void CustomComboBox::ClearOption(const wxString& item)
	{
		for (auto it = m_options.begin(); it != m_options.end();)
		{
			if (it->first == item)
			{
				it = m_options.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void CustomComboBox::ClearOptions()
	{
		m_options.clear();
	}

	int32_t CustomComboBox::GetOptionsSize()
	{
		return  m_options.size();
	}

	void CustomComboBox::SetBackground(BackgroundPtr& backgroundPtr)
	{
		m_backgroundPtr = backgroundPtr;
		Refresh();
	}

	void CustomComboBox::SetButtonRes(const std::string &strControlRes, uint32_t uButtonSize)
	{
		IControlRes			*pControlRes = Singleton<ResourceMgr>::Instance()->FindControlRes(strControlRes.c_str());

		m_uButtonSize = uButtonSize;
		if (m_pButtonRes != pControlRes)
		{
			m_pButtonRes = pControlRes;
			Refresh();
		}
	}

	void CustomComboBox::SetButtonIcon(PicturePtr &iconPtr, const wxSize &szIcon)
	{
		m_iconPtr = iconPtr;
		m_szIcon = szIcon;
		Refresh();
	}

	void CustomComboBox::OnPaint(wxPaintEvent& event)
	{
		if (!m_bufferBitmap.IsOk())
		{
			wxSize			szButton = GetClientSize();

			m_bufferBitmap = wxBitmap(szButton.x, szButton.y);
		}

		if (m_bufferBitmap.IsOk())
		{
			wxPaintDC		dc(this);
			wxMemoryDC		memDC(m_bufferBitmap);
		
			m_rcClient = GetClientRect();
			OnDraw(memDC);
			memDC.SelectObject(wxNullBitmap);
			dc.DrawBitmap(m_bufferBitmap, 0, 0, false);
		}
	}

	void CustomComboBox::OnEraseBackground(wxEraseEvent& event)
	{

	}

	void CustomComboBox::OnSize(wxSizeEvent& event)
	{
		wxSize			szButton = GetClientSize();

		if (szButton.x > 0 && szButton.y > 0)
			m_bufferBitmap = wxBitmap(szButton.x, szButton.y);
	}

	void CustomComboBox::GetButtonRect(wxRect &rcButton)
	{
		rcButton.x = m_rcClient.x + m_rcClient.width - m_uButtonSize;
		rcButton.y = m_rcClient.y;
		rcButton.width = m_uButtonSize;
		rcButton.height = m_rcClient.height;
	}

	void CustomComboBox::OnLButtonDown(wxMouseEvent& event)
	{
		SetFocus();		
		if (m_rcClient.Contains(event.GetPosition()))
		{
			m_bMouseDown = true;
			m_eState = ControlStatus::e_state_pushed;
			Refresh();
		}
		
		event.Skip(false);
	}

	void CustomComboBox::OnLButtonUp(wxMouseEvent& event)
	{
		if (m_bMouseDown)
		{
			m_bMouseDown = false;
			if (m_rcClient.Contains(event.GetPosition()))
				OnButtonClick();
		}
		
		m_eState = ControlStatus::e_state_hover;
		Refresh();
		
		event.Skip(false);
	}

	void CustomComboBox::OnLeaveWindow(wxMouseEvent& event)
	{
		m_eState = ControlStatus::e_state_normal;
		Refresh();
		event.Skip(false);
	}

	void CustomComboBox::OnEnterWindow(wxMouseEvent& event)
	{
		wxRect			rcButton;

		GetButtonRect(rcButton);
		if (rcButton.Contains(event.GetPosition()))
		{
			m_eState = ControlStatus::e_state_hover;
			Refresh(false);
		}

		event.Skip();
	}

	void CustomComboBox::OnDraw(wxDC& dc)
	{
		wxRect					rcEdit;
		wxRect					rcButton;

		DrawBackground(dc);

		rcEdit = m_rcClient;
		rcEdit.width -= m_uButtonSize;
		DrawText(dc, rcEdit);

		GetButtonRect(rcButton);
		DrawButton(dc, rcButton);
	}

	void CustomComboBox::DrawBackground(wxDC& dc)
	{
		wxColor			crBackground;

		crBackground = GetBackgroundColour();
		if (crBackground != m_crBackground)
		{
			m_crBackground = crBackground;
			m_brBKBrush = wxBrush(m_crBackground);
		}

		dc.SetBrush(m_brBKBrush);
		dc.SetPen(*wxTRANSPARENT_PEN);
		dc.DrawRectangle(m_rcClient);

		if (nullptr != m_backgroundPtr.get())
			m_backgroundPtr->Draw(dc, m_rcClient);
	}

	void CustomComboBox::DrawText(wxDC& dc, const wxRect& rcEdit)
	{
		wxString				strTitle;

		if (m_bDateTime)
		{
			wxString strMonth = m_dateTime.GetMonth() + 1 > 9 ? "" : "0";
			strMonth += std::to_string(m_dateTime.GetMonth() + 1);

			wxString strDay = m_dateTime.GetDay() > 9 ? "" : "0";
			strDay += std::to_string(m_dateTime.GetDay());

			strTitle = std::to_string(m_dateTime.GetYear()) + "/" + strMonth + "/" + strDay;
		}
		else if ((m_nSelected >= 0) && (m_nSelected < (int32_t)m_options.size()))
		{
			strTitle = m_options[m_nSelected].first;
		}

		if (!strTitle.empty())
		{
			wxFont					ftDefault = GetFont();
			wxFont*					pFont = &ftDefault;
			wxColour				crColor = GetForegroundColour();
			wxRect					rcText(wxPoint(0, 0), GetSize());
			wxSize					szText;
			wxPoint					ptPos;

			dc.SetFont(*pFont);
			dc.SetTextForeground(crColor);
			szText = dc.GetTextExtent(strTitle);

			ptPos.x = rcText.x + 10;
			ptPos.y = rcText.y + (rcText.height - szText.y) / 2;
			dc.DrawText(strTitle, ptPos);
		}
	}

	void CustomComboBox::DrawButton(wxDC& dc, const wxRect& rcButton)
	{
		if (m_pButtonRes)
		{
			IBackground*			pBackground = nullptr;

			m_pButtonRes->SetStatus(m_eState);
			pBackground = m_pButtonRes->Background();
			if (pBackground)
				pBackground->Draw(dc, rcButton);

		}

		if (m_iconPtr.get())
		{
			wxPoint					ptIcon;
			
			ptIcon.x = rcButton.x + (rcButton.width - m_szIcon.x) / 2;
			ptIcon.y = rcButton.y + (rcButton.height - m_szIcon.y) / 2;
			m_iconPtr->DrawImage(dc, ptIcon, m_szIcon);
		}
	}

	void CustomComboBox::OnButtonClick()
	{
		if (m_bDateTime) {
			ShowDateBox();
		}
		else {
			ShowListBox();
		}
	}

	void CustomComboBox::ShowListBox()
	{
		if (nullptr == g_pListBoxPart)
			g_pListBoxPart = new PopuListBox(nullptr);

		wxClientDC		dc(this);
		uint32_t		uCount = (uint32_t)m_options.size();
		wxSize			szListBox{ 0, 2 };
		int32_t			nTextWidth;
		int32_t			nTextHeight;
		auto			ftFont = GetFont();

		dc.SetFont(ftFont);
		g_pListBoxPart->Clear();
		g_pListBoxPart->SetFont(ftFont);
		for (uint32_t i = 0; i < uCount; i++)
		{
			wxString ellipsisedText = m_options[i].first;

			if (m_options[i].first.empty())
				dc.GetTextExtent("Hg", &nTextWidth, &nTextHeight);
			else
				dc.GetTextExtent(m_options[i].first, &nTextWidth, &nTextHeight);

			if (nTextWidth > m_rcClient.width)
			{
				while (dc.GetTextExtent(ellipsisedText + "...").GetWidth() > m_rcClient.width)
				{
					ellipsisedText = ellipsisedText.Left(ellipsisedText.Length() - 1);
				}
				ellipsisedText += "...";
			}

			auto uDeviceMode = static_cast<DeviceModel>(g_pMiddlewareDataCenter->GetDataValue("operation_maintenance", "device_model")->GetUIntValue());
			if ((uDeviceMode == DeviceModel::e6100 || uDeviceMode == DeviceModel::e7000 || uDeviceMode == DeviceModel::e6000 || uDeviceMode == DeviceModel::e3000)
				&& (m_options[i].first == "Y2" || m_options[i].first == "Y1&Y2"|| m_options[i].first == "Z2"|| m_options[i].first == "Z1&Z2"))
			{}
			else
			{
				if (i < 8)
					szListBox.y += nTextHeight;

				g_pListBoxPart->Append(ellipsisedText);
			}
		}

		szListBox.y += 2;
		szListBox.x = m_rcClient.width;

		g_pListBoxPart->ShowForTarget(this, szListBox);
	}

	void CustomComboBox::ShowDateBox()
	{
		if (nullptr == g_pDateBoxPart)
			g_pDateBoxPart = new PopuDateBox(nullptr);

		g_pDateBoxPart->SetDateTime(m_dateTime);
		g_pDateBoxPart->ShowForTarget(this);
	}
}