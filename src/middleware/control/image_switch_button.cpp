#include "image_switch_button.h"
#include "singleton.h"
#include "resource_manager.h"
#include "util.h"

namespace why
{
	wxIMPLEMENT_CLASS(ImageSwitchButton, wxWindow);

	ImageSwitchButton::ImageSwitchButton(wxWindow* parent, wxWindowID id, const wxString& label, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
		: wxWindow(parent, id, pos, size, 0, name)
	{
		SetLabel(label);
		Show(false);

		memset(&m_rcClient, 0, sizeof(m_rcClient));
		m_bMouseDown = false;

		m_eState = ControlStatus::e_state_normal;

		m_crBackground = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
		m_brBKBrush = wxBrush(m_crBackground);
	}
	
	ImageSwitchButton::~ImageSwitchButton()
	{
		
	}

	bool ImageSwitchButton::ProcessEvent(wxEvent& event)
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

			if (wxEVT_MOTION == eventType)
			{
				OnMouseMove(dynamic_cast<wxMouseEvent&>(event));
				break;
			}

			if (wxEVT_LEAVE_WINDOW == eventType)
			{
				OnLeaveWindow(dynamic_cast<wxMouseEvent&>(event));
				break;
			}

			if (wxEVT_LEFT_DCLICK == eventType)
			{
				OnLButtonDClick(dynamic_cast<wxMouseEvent&>(event));
				break;
			}
		} while (false);
		
		return wxEvtHandler::ProcessEvent(event);
	}

	void ImageSwitchButton::SetLabel(const wxString& label)
	{ 
		if (m_strTitle != label)
		{
			m_strTitle = label;
			Refresh();
		}
	}

	void ImageSwitchButton::SetControlRes(const std::string& strUncheckRes, const std::string& strCheckRes)
	{
		IControlRes			*pUncheckRes = Singleton<ResourceMgr>::Instance()->FindControlRes(strUncheckRes.c_str());
		IControlRes			*pCheckRes = Singleton<ResourceMgr>::Instance()->FindControlRes(strCheckRes.c_str());

		if ((m_pUnCheckRes != pUncheckRes) || (m_pCheckRes != pCheckRes))
		{
			m_pUnCheckRes = pUncheckRes;
			m_pCheckRes = pCheckRes;
			Refresh();
		}
	}

	void ImageSwitchButton::SetCheck(bool bCheck)
	{
		if (m_bChecked != bCheck)
		{
			m_bChecked = bCheck;
			Refresh();
		}
	}

	bool ImageSwitchButton::IsChecked() 
	{
		return m_bChecked;
	}

	uint32_t ImageSwitchButton::GetUserData(bool bCheck)
	{
		return (bCheck ? m_userData[1] : m_userData[0]);
	}

	void ImageSwitchButton::SetUserData(bool bCheck, uint32_t uUserData)
	{
		if (bCheck)
			m_userData[1] = uUserData;
		else
			m_userData[0] = uUserData;
	}

	bool ImageSwitchButton::MatchUserData(uint32_t uUserData)
	{
		return (uUserData == m_userData[1]);
	}

	void ImageSwitchButton::OnLButtonDown(wxMouseEvent& event)
	{
		if (!ProcessMsgHandledFlag(event.GetPosition()))
		{
			SetFocus();
			m_bMouseDown = true;
			m_eState = ControlStatus::e_state_pushed;
			Refresh();
		}
	}

	void ImageSwitchButton::OnLButtonUp(wxMouseEvent& event)
	{
		if (!ProcessMsgHandledFlag(event.GetPosition()))
		{
			if (m_bMouseDown)
			{
				wxPoint		ptMouse = event.GetPosition();
				wxRect		rcClient = GetClientRect();


				if (rcClient.Contains(ptMouse))
				{
					wxCommandEvent			buttonClickEvent(wxEVT_BUTTON, GetId());

					m_bChecked = !m_bChecked;
					buttonClickEvent.SetEventObject(this);
					wxPostEvent(this, buttonClickEvent);
				}
				
				m_bMouseDown = false;
			}
			m_eState = ControlStatus::e_state_hover;
			Refresh();
		}
	}

	void ImageSwitchButton::OnLeaveWindow(wxMouseEvent& event)
	{
		if (m_bMouseDown)
		{
			wxMouseEvent			mouseEvent(wxEVT_LEFT_UP);

			mouseEvent.SetEventObject(this);
			mouseEvent.SetPosition(event.GetPosition());
			mouseEvent.SetId(this->GetId());
		
			wxPostEvent(this, mouseEvent);
			m_bMouseDown = false;
		}

		m_eState = ControlStatus::e_state_normal;
		Refresh();
	}

	void ImageSwitchButton::OnEnterWindow(wxMouseEvent& event)
	{
		if (!ProcessMsgHandledFlag(event.GetPosition()))
		{
			m_eState = ControlStatus::e_state_hover;
			Refresh(false);
		}
	}

	void ImageSwitchButton::OnMouseMove(wxMouseEvent& event)
	{
		if (!ProcessMsgHandledFlag(event.GetPosition()))
		{
			m_eState = ControlStatus::e_state_hover;
			Refresh(false);
		}
	}

	void ImageSwitchButton::OnPaint(wxPaintEvent& event)
	{
		if (!m_bufferBitmap.IsOk())
		{
			wxSize			szButton = GetClientSize();

			m_bufferBitmap = wxBitmap(szButton.x, szButton.y);
		}

		if (m_bufferBitmap.IsOk())
		{
			wxPaintDC		dc(this);
			wxMemoryDC		memDC;
			wxRect			rcClient;

			rcClient = GetClientRect();
			memDC.SelectObject(m_bufferBitmap);
			memDC.Clear();
			DrawCheckbox(memDC, rcClient);
			memDC.SelectObject(wxNullBitmap);
			dc.DrawBitmap(m_bufferBitmap, 0, 0, true);
		}
	}

	void ImageSwitchButton::OnEraseBackground(wxEraseEvent& event)
	{
		event.Skip(false);
	}

	void ImageSwitchButton::DrawCheckbox(wxDC& dc, const wxRect& rcItem)
	{
		DrawBackground(dc, rcItem);
	}

	void ImageSwitchButton::DrawBackground(wxDC& dc, const wxRect& rcItem)
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
		dc.DrawRectangle(rcItem);

		IControlRes* pControlRes = m_bChecked ? m_pCheckRes : m_pUnCheckRes;
		if (pControlRes)
		{
			ControlStatus			eStatus = GetCurrentStatus();
			IBackground*			pBackground = nullptr;

			pControlRes->SetStatus(eStatus);
			pBackground = pControlRes->Background();
			if (pBackground)
				pBackground->Draw(dc, rcItem);
		}
	}

	ControlStatus ImageSwitchButton::GetCurrentStatus()
	{
		ControlStatus			eStatus;

		if (!IsEnabled())
			eStatus = ControlStatus::e_state_disable;
		else
			eStatus = m_eState;
	
		return eStatus;
	}

	bool ImageSwitchButton::ProcessMsgHandledFlag(const wxPoint &ptPos)
	{
		if (!m_bufferBitmap.IsOk())
			return false;

		wxNativePixelData		data(m_bufferBitmap);

		if (data)
		{
			wxNativePixelData::Iterator		pixelIt(data);
			auto							szBitmap = m_bufferBitmap.GetSize();
			wxColour						crPixel;

			// 确保坐标在位图范围内
			if (ptPos.x >= 0 && ptPos.x < szBitmap.x && ptPos.y >= 0 && ptPos.y < szBitmap.y)
			{
				uint8_t		uGreen;
				uint8_t		uRed;
				uint8_t		uBlue;
				uint8_t		uAlpha = wxALPHA_OPAQUE;

				pixelIt.MoveTo(data, ptPos.x, ptPos.y);

				uRed = pixelIt.Red();
				uGreen = pixelIt.Green();
				uBlue = pixelIt.Blue();
				if (m_bufferBitmap.HasAlpha())
					uAlpha = pixelIt.Alpha();

				crPixel.Set(uRed, uGreen, uBlue, uAlpha);
				if (m_crBackground == crPixel)
					return true;
			}
		}
		return false;
	}

	void ImageSwitchButton::OnSize(wxSizeEvent& event)
	{
		wxSize			szButton = GetClientSize();

		if (szButton.x > 0 && szButton.y > 0)
			m_bufferBitmap = wxBitmap(szButton.x, szButton.y);
	}

	void ImageSwitchButton::OnLButtonDClick(wxMouseEvent& event)
	{
		wxCommandEvent			buttonClickEvent(wxEVT_BUTTON, GetId());

		m_bChecked = !m_bChecked;
		buttonClickEvent.SetEventObject(this);
		wxPostEvent(this, buttonClickEvent);
		Refresh();
	}
}