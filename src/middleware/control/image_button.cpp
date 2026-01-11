#include "image_button.h"
#include "singleton.h"
#include "resource_manager.h"
#include "util.h"

namespace why
{
	namespace
	{
		uint32_t g_uChoiceSize = 20;
	}

	wxIMPLEMENT_CLASS(ImageButton, wxWindow);

	ImageButton::ImageButton(wxWindow* parent, wxWindowID id, const wxString& label, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
		: wxWindow(parent, id, pos, size, style, name)
		, m_customWidgets(this)
	{
		if (m_uStyle & static_cast<uint32_t>(ButtonStyle::transparent))
		{
			SetBackgroundStyle(wxBG_STYLE_PAINT);
			SetTransparent(255);
		}

		SetLabel(label);
		Show(false);

		memset(&m_rcClient, 0, sizeof(m_rcClient));

		m_bMouseDown = false;

		m_eState = ControlStatus::e_state_normal;

		m_crBackground = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
		m_brBKBrush = wxBrush(m_crBackground);

		m_timerPtr.reset(new wxTimer(this));
	}

	ImageButton::~ImageButton()
	{

	}

	bool ImageButton::ProcessEvent(wxEvent& event)
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

			if (wxEVT_TIMER == eventType)
			{
				OnCommandRepeat(dynamic_cast<wxTimerEvent&>(event));
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

	void ImageButton::SetLabel(const wxString& label)
	{
		wxString		strName = GetName();

		if (!strName.IsEmpty())
			m_customWidgets.ChangeTextElement(strName.utf8_string(), label);

		if (m_strTitle != label)
		{
			m_strTitle = label;
			Refresh();
		}
	}

	bool ImageButton::Show(bool show)
	{
		bool bRet = wxWindow::Show(show);
		if (show)
		{
			Refresh();
			Update();
		}
		return bRet;
	}

	void ImageButton::SetButtonStyle(uint32_t uStyle)
	{
		if (m_uStyle != uStyle)
		{
			m_uStyle = uStyle;
			Refresh();
		}
	}

	void ImageButton::SetControlRes(const std::string& strControlRes)
	{
		IControlRes* pControlRes = Singleton<ResourceMgr>::Instance()->FindControlRes(strControlRes.c_str());

		if (m_pControlRes != pControlRes)
		{
			m_pControlRes = pControlRes;
			Refresh();
		}
		m_customWidgets.SetControlRes(strControlRes);
	}

	void ImageButton::SetSelectIconName(const std::string& strName)
	{
		m_strSelectIconName = strName;
	}

	bool ImageButton::IsSelect()
	{
		if (m_strSelectIconName.empty()) {
			return false;
		}

		return m_customWidgets.IsShowIconElement(m_strSelectIconName);
	}

	void ImageButton::ShowSelect(bool show)
	{
		if (m_strSelectIconName.empty()) {
			return;
		}

		m_customWidgets.ShowIconElement(m_strSelectIconName, show);
	}

	void ImageButton::OnLButtonDown(wxMouseEvent& event)
	{
		if (!ProcessMsgHandledFlag(event.GetPosition()))
		{
			SetFocus();
			m_bMouseDown = true;
			m_eState = ControlStatus::e_state_pushed;
			Refresh();

			if (m_uStyle & static_cast<uint32_t>(ButtonStyle::timer_repeat))
			{
				if (0 == m_uLastStamp)
				{
					m_uLastStamp = GetTimeStamp64();
					m_timerPtr->Start(100);
				}
			}
		}
	}

	void ImageButton::OnLButtonUp(wxMouseEvent& event)
	{
		if (!ProcessMsgHandledFlag(event.GetPosition()))
		{
			if (m_bMouseDown)
			{
				wxPoint		ptMouse = event.GetPosition();
				wxRect		rcClient = GetClientRect();

				if (!m_strSelectIconName.empty())
					m_customWidgets.ChangeIconElementShow(m_strSelectIconName);

				if (rcClient.Contains(ptMouse))
				{
					wxCommandEvent			buttonClickEvent(wxEVT_BUTTON, GetId());

					buttonClickEvent.SetEventObject(this);
					if (m_uStyle & static_cast<uint32_t>(ButtonStyle::timer_repeat))
					{
						buttonClickEvent.SetTimestamp(static_cast<long>(GetTimeStamp64() - m_uLastStamp));
						m_uLastStamp = 0;
						m_timerPtr->Stop();
					}

					wxPostEvent(this, buttonClickEvent);
				}

				m_bMouseDown = false;
			}
			m_eState = ControlStatus::e_state_hover;
			Refresh();
		}
	}

	void ImageButton::OnLeaveWindow(wxMouseEvent& event)
	{
		UnsetToolTip();

		if (0 != m_uLastStamp)
		{
			m_timerPtr->Stop();
			m_uLastStamp = 0;
		}

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

	void ImageButton::OnEnterWindow(wxMouseEvent& event)
	{
		if (!m_strTitle.empty())
		{
			SetToolTip(m_strTitle);
		}

		if (!ProcessMsgHandledFlag(event.GetPosition()))
		{
			m_eState = ControlStatus::e_state_hover;
			Refresh(false);
		}
	}

	void ImageButton::OnMouseMove(wxMouseEvent& event)
	{
		if (!ProcessMsgHandledFlag(event.GetPosition()))
		{
			m_eState = ControlStatus::e_state_hover;
			Refresh(false);
		}
	}

	void ImageButton::OnPaint(wxPaintEvent& event)
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
			DrawButton(memDC, rcClient);
			memDC.SelectObject(wxNullBitmap);
			dc.DrawBitmap(m_bufferBitmap, 0, 0, true);
		}
	}

	void ImageButton::OnEraseBackground(wxEraseEvent& event)
	{
		event.Skip(false);
	}

	void ImageButton::DrawButton(wxDC& dc, const wxRect& rcItem)
	{
		DrawBackground(dc, rcItem);
		if ((m_uStyle & static_cast<uint32_t>(ButtonStyle::only_text)) || m_customWidgets.IsEmpty())
			DrawText(dc);
		else
		{
			if (!m_bCleanStatus)
				m_customWidgets.OnDraw(dc);
		}
	}

	void ImageButton::DrawBackground(wxDC& dc, const wxRect& rcItem)
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

		if (m_pControlRes)
		{
			ControlStatus			eStatus = GetCurrentStatus();
			IBackground* pBackground = nullptr;

			m_pControlRes->SetStatus(eStatus);
			pBackground = m_pControlRes->Background();
			if (pBackground)
				pBackground->Draw(dc, rcItem);
		}
	}

	ControlStatus ImageButton::GetCurrentStatus()
	{
		ControlStatus			eStatus;

		if (!IsEnabled())
			eStatus = ControlStatus::e_state_disable;
		else
			eStatus = m_eState;

		return eStatus;
	}

	void ImageButton::DrawText(wxDC& dc)
	{
		wxFont					ftDefault = GetFont();
		wxFont* pFont = &ftDefault;
		wxColour				crColor = GetForegroundColour();
		wxRect					rcText(wxPoint(0, 0), GetSize());
		wxSize					szText;
		wxPoint					ptPos;

		dc.SetFont(*pFont);
		dc.SetTextForeground(crColor);
		szText = dc.GetTextExtent(m_strTitle);
		ptPos.x = rcText.x + (rcText.width - szText.x) / 2;
		ptPos.y = rcText.y + (rcText.height - szText.y) / 2;
		dc.DrawText(m_strTitle, ptPos);
	}

	bool ImageButton::ProcessMsgHandledFlag(const wxPoint& ptPos)
	{
		if (m_uStyle & static_cast<uint32_t>(ButtonStyle::transparent))
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
		}

		return false;
	}

	void ImageButton::OnSize(wxSizeEvent& event)
	{
		wxSize			szButton = GetClientSize();

		if (szButton.x > 0 && szButton.y > 0)
			m_bufferBitmap = wxBitmap(szButton.x, szButton.y);
	}

	void ImageButton::OnCommandRepeat(wxTimerEvent& event)
	{
		uint64_t				uStamp = GetTimeStamp64();
		wxCommandEvent			buttonClickEvent(wxEVT_BUTTON, GetId());

		buttonClickEvent.SetEventObject(this);
		buttonClickEvent.SetTimestamp((long)(uStamp - m_uLastStamp));
		wxPostEvent(this, buttonClickEvent);
		m_uLastStamp = uStamp;
	}

	void ImageButton::OnLButtonDClick(wxMouseEvent& event)
	{
		wxCommandEvent			buttonClickEvent(wxEVT_BUTTON, GetId());

		buttonClickEvent.SetEventObject(this);
		wxPostEvent(this, buttonClickEvent);
	}
	void ImageButton::SetCleanStatus(bool bCleanStatus)
	{
		if (bCleanStatus)
		{
			Disable();
		}
		m_bCleanStatus = bCleanStatus;
	}
}