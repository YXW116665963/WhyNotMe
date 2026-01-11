#include "virtual_button.h"
#include "singleton.h"
#include "resource_manager.h"
#include "virtual_wnd_owner.h"
#include "logger.h"
#include "ui_util.h"

namespace why
{
	VirtualButton::VirtualButton(VirtualWndOwner* parent, const wxString& label, const wxRect& rcRect, const wxString& name)
			: VirtualWnd(parent, label, rcRect, name)
	{
		
	}

	void VirtualButton::OnPaint(wxGraphicsContext* pGraphic)
	{
		VirtualWnd::OnPaint(pGraphic);
		
		if (m_textElements.empty() && m_iconElements.empty())
		{
			DrawText(pGraphic);
		}
		else
		{
			DrawIconElements(pGraphic);
			DrawTextElements(pGraphic);
		}
	}

	bool VirtualButton::OnLButtonDown(wxMouseEvent& event) 
	{
		m_bMouseDown = true;
		SetControlStatus(ControlStatus::e_state_pushed);

		wxMouseEvent			mouseEvent(wxEVT_LEFT_DOWN);

		mouseEvent.SetEventObject(this);
		AddPendingEvent(mouseEvent);
		return true;
	}

	void VirtualButton::OnLButtonUp(wxMouseEvent& event) 
	{
		if (m_bMouseDown)
		{
			wxMouseEvent			mouseEvent(wxEVT_LEFT_UP);
			wxCommandEvent			buttonClickEvent(wxEVT_BUTTON);
			wxRect					rcRect = GetRect();

			mouseEvent.SetEventObject(this);
			AddPendingEvent(mouseEvent);

			buttonClickEvent.SetEventObject(this);
			buttonClickEvent.SetString(Name());
			AddPendingEvent(buttonClickEvent);
			m_bMouseDown = false;

			if (PtInRect(event.GetPosition(), rcRect))
				SetControlStatus(ControlStatus::e_state_hover);
			else
				SetControlStatus(ControlStatus::e_state_normal);
		}
	}

	void VirtualButton::OnEnterWindow(wxMouseEvent& event) 
	{
		SetControlStatus(ControlStatus::e_state_hover);
	}

	void VirtualButton::OnLeaveWindow(wxMouseEvent& event) 
	{
		SetControlStatus(ControlStatus::e_state_normal);
	}

	bool VirtualButton::OnLButtonDClick(wxMouseEvent& event)
	{
		wxCommandEvent			buttonClickEvent(wxEVT_BUTTON);
		
		buttonClickEvent.SetEventObject(this);
		buttonClickEvent.SetString(Name());
		AddPendingEvent(buttonClickEvent);
		return true;
	}

	void VirtualButton::AddTextElement(const std::string& strName, FontPtr& fontPtr, wxColour& crText, const wxString& strText, const wxRect& rcText)
	{
		bool			bFind = false;
		wxRect			rcClient = GetRect();
		wxRect			rcElement = rcText;

		rcElement.x += rcClient.x;
		rcElement.y += rcClient.y;

		if (!strName.empty())
		{
			for (auto it = m_textElements.begin(); it != m_textElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_fontPtr = fontPtr;
					(*it)->m_crText = crText;
					(*it)->m_strText = strText;
					(*it)->m_rcText = rcElement;
					bFind = true;
				}
			}
		}

		if (!bFind)
		{
			CustomTextInfoPtr		customTextPtr(new CustomTextInfo);

			customTextPtr->m_strName = strName;
			customTextPtr->m_fontPtr = fontPtr;
			customTextPtr->m_crText = crText;
			customTextPtr->m_strText = strText;
			customTextPtr->m_rcText = rcElement;
			m_textElements.push_back(std::move(customTextPtr));
		}

		Refresh();
	}

	void VirtualButton::ChangeTextElement(const std::string& strName, const wxString& strText)
	{
		if (!strName.empty())
		{
			for (auto it = m_textElements.begin(); it != m_textElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_strText = strText;
					Refresh();
					break;
				}
			}
		}
	}

	void VirtualButton::RemoveTextElement(const std::string& strName)
	{
		if (!strName.empty())
		{
			for (auto it = m_textElements.begin(); it != m_textElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					m_textElements.erase(it);
					Refresh();
					break;
				}
			}
		}
	}

	void VirtualButton::AddIconElement(const std::string& strName, PicturePtr& normalPtr, PicturePtr& hoverPtr, PicturePtr& pushedPtr, const wxRect& rcIcon)
	{
		bool			bFind = false;
		wxRect			rcClient = GetRect();
		wxRect			rcElement = rcIcon;

		rcElement.x += rcClient.x;
		rcElement.y += rcClient.y;

		if (!strName.empty())
		{
			for (auto it = m_iconElements.begin(); it != m_iconElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_normalPtr = normalPtr;
					(*it)->m_hoverPtr = hoverPtr;
					(*it)->m_pushedPtr = pushedPtr;
					(*it)->m_rcIcon = rcElement;
					bFind = true;
				}
			}
		}

		if (!bFind)
		{
			CustomIconInfoPtr		customIconPtr(new CustomIconInfo);

			customIconPtr->m_strName = strName;
			customIconPtr->m_normalPtr = normalPtr;
			customIconPtr->m_hoverPtr = hoverPtr;
			customIconPtr->m_pushedPtr = pushedPtr;
			customIconPtr->m_rcIcon = rcElement;
			m_iconElements.push_back(std::move(customIconPtr));
		}

		Refresh();
	}

	void VirtualButton::ChangeIconElement(const std::string& strName, PicturePtr& normalPtr, PicturePtr& hoverPtr, PicturePtr& pushedPtr)
	{
		if (!strName.empty())
		{
			for (CustomIconIt it = m_iconElements.begin(); it != m_iconElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_normalPtr = normalPtr;
					(*it)->m_hoverPtr = hoverPtr;
					(*it)->m_pushedPtr = pushedPtr;

					Refresh();
					break;
				}
			}
		}
	}

	void VirtualButton::RemoveIconElement(const std::string& strName)
	{
		if (!strName.empty())
		{
			for (auto it = m_iconElements.begin(); it != m_iconElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					m_iconElements.erase(it);
					Refresh();
					break;
				}
			}
		}
	}

	void VirtualButton::DrawIconElements(wxGraphicsContext* pGraphic)
	{
		for (auto it = m_iconElements.begin(); it != m_iconElements.end(); it++)
		{
			CustomIconInfoPtr&		iconInfoPtr = (*it);
			ControlStatus			eStatus = GetControlStatus();
			Picture*				pPicture = GetPicture(iconInfoPtr, eStatus);

			if (nullptr != pPicture)
				pPicture->DrawImage(pGraphic, iconInfoPtr->m_rcIcon);
		}
	}

	void VirtualButton::DrawTextElements(wxGraphicsContext* pGraphic)
	{
		wxFont					ftDefault = GetFont();
		wxFont*					pFont = &ftDefault;
		wxColour				crColor = GetForegroundColour();

		for (CustomTextIt it = m_textElements.begin(); it != m_textElements.end(); it++)
		{
			wxString& strTitle = (*it)->m_strText;

			if (!strTitle.empty())
			{
				wxRect					rcText = (*it)->m_rcText;
				float64_t				fWidth = 0.0;
				float64_t				fHeight = 0.0;
				float64_t				fPosX = 0.0;
				float64_t				fPosY = 0.0;

				if (nullptr != (*it)->m_fontPtr.get())
				{
					pFont = (*it)->m_fontPtr.get();
					crColor = (*it)->m_crText;
				}
				else
				{
					pFont = &ftDefault;
					crColor = GetForegroundColour();
				}

				pGraphic->SetFont(*pFont, crColor);
				pGraphic->GetTextExtent(strTitle, &fWidth, &fHeight);
				fPosX = rcText.x + (rcText.width - fWidth) / 2;
				fPosY = rcText.y + (rcText.height - fHeight) / 2;
				pGraphic->DrawText(strTitle, fPosX, fPosY);
			}
		}
	}

	Picture* VirtualButton::GetPicture(CustomIconInfoPtr& iconInfoPtr, ControlStatus eStatus)
	{
		Picture* pPicture = iconInfoPtr->m_normalPtr.get();

		switch (eStatus)
		{
			case ControlStatus::e_state_normal:
				break;
			case ControlStatus::e_state_hover:
				if (nullptr != iconInfoPtr->m_hoverPtr.get())
				{
					pPicture = iconInfoPtr->m_hoverPtr.get();
				}
				break;
			case ControlStatus::e_state_pushed:
				if (nullptr != iconInfoPtr->m_pushedPtr.get())
				{
					pPicture = iconInfoPtr->m_pushedPtr.get();
				}
				break;
			case ControlStatus::e_state_disable:
				break;
			default:
				break;
		}

		return pPicture;
	}

	void VirtualButton::DrawText(wxGraphicsContext* pGraphic)
	{
		wxString				strTitle = GetLabel();

		if (!strTitle.IsEmpty())
		{
			wxRect					rcClient = GetRect();
			wxFont					ftDefault = GetFont();
			wxColour				crColor = GetForegroundColour();
			float64_t				fWidth = 0.0;
			float64_t				fHeight = 0.0;
			float64_t				fPosX = 0.0;
			float64_t				fPosY = 0.0;

			pGraphic->SetFont(ftDefault, crColor);
			pGraphic->GetTextExtent(strTitle, &fWidth, &fHeight);
			fPosX = rcClient.x + (rcClient.width - fWidth) / 2;
			fPosY = rcClient.y + (rcClient.height - fHeight) / 2;
			pGraphic->DrawText(strTitle, fPosX, fPosY);
		}
	}
}
