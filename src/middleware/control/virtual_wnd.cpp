#include "virtual_wnd.h"
#include "singleton.h"
#include "resource_manager.h"
#include "virtual_wnd_owner.h"
#include "background.h"
#include "logger.h"

namespace why
{
	VirtualWnd::VirtualWnd(VirtualWndOwner* pOwner, const wxString& label, const wxRect& rcRect, const wxString& name)
			: m_pOwner(pOwner)
			, m_label(label)
			, m_pos(rcRect.GetLeftTop())
			, m_size(rcRect.GetSize())
			, m_name(name)
	{
		m_dirtyRegion.SetRegion(m_pos, m_size);
	}

	VirtualWndOwner* VirtualWnd::Owner()
	{
		return m_pOwner;
	}

	wxString& VirtualWnd::Name()
	{
		return m_name;
	}

	void VirtualWnd::SetLabel(const wxString& label)
	{
		if (m_label != label)
		{
			m_label = label;
			Refresh();
		}
	}

	wxString VirtualWnd::GetLabel() const
	{
		return m_label;
	}

	bool VirtualWnd::Show(bool bShow)
	{
		if (m_show != bShow)
		{
			m_show = bShow;
			Refresh();

			return !bShow;
		}

		return bShow;
	}

	bool VirtualWnd::IsShow() const
	{
		return m_show;
	}
	wxPoint VirtualWnd::GetPosition()
	{
		return m_pos;
	}

	wxSize VirtualWnd::GetSize()
	{
		return m_size;
	}

	wxRect VirtualWnd::GetRect()
	{
		return wxRect(m_pos, m_size);
	}

	void VirtualWnd::SetSize(int x, int y, int width, int height)
	{
		bool		bRefresh = false;

		if ((m_pos.x != x) || (m_pos.y != y))
		{
			m_pos.x = x;
			m_pos.y = y;
			bRefresh = true;
		}

		if ((m_size.x != width) || (m_size.y != height))
		{
			m_size.x = width;
			m_size.y = height;
			bRefresh = true;
		}

		if (bRefresh)
			Refresh();
	}

	void VirtualWnd::SetSize(const wxRect& rect)
	{
		bool		bRefresh = false;

		if ((m_pos.x != rect.x) || (m_pos.y != rect.y))
		{
			m_pos.x = rect.x;
			m_pos.y = rect.y;
			bRefresh = true;
		}

		if ((m_size.x != rect.width) || (m_size.y != rect.height))
		{
			m_size.x = rect.width;
			m_size.y = rect.height;
			bRefresh = true;
		}

		if (bRefresh)
			Refresh();
	}

	ControlStatus VirtualWnd::GetControlStatus() const
	{
		return m_eStatus;
	}

	void VirtualWnd::SetControlStatus(ControlStatus eStatus)
	{
		if (m_eStatus != eStatus)
		{
			m_eStatus = eStatus;
			Refresh();
		}
	}

	void VirtualWnd::SetControlRes(const std::string& strControlRes)
	{
		IControlRes* pControlRes = Singleton<ResourceMgr>::Instance()->FindControlRes(strControlRes.c_str());

		if (m_pControlRes != pControlRes)
		{
			m_pControlRes = pControlRes;
			Refresh();
		}
	}

	const wxColour& VirtualWnd::GetBackgroundColour() const
	{
		return m_crBackground;
	}

	const wxColour& VirtualWnd::GetForegroundColour() const
	{
		wxFont* pFont = m_pControlRes ? m_pControlRes->Font() : nullptr;

		if (nullptr != pFont)
			return m_pControlRes->FontColor();
		else
			return m_crForeground;
	}

	const wxFont& VirtualWnd::GetFont() const
	{
		wxFont* pFont = m_pControlRes ? m_pControlRes->Font() : nullptr;

		if (nullptr != pFont)
			return *pFont;
		else
			return m_ftText;
	}

	void VirtualWnd::SetBackgroundColour(const wxColour& colour)
	{
		if (m_crBackground != colour)
		{
			m_brBackground = wxBrush(colour, wxBRUSHSTYLE_SOLID);
			m_crBackground = colour;
			Refresh();
		}
	}

	void VirtualWnd::SetForegroundColour(const wxColour& colour)
	{
		if (m_crForeground != colour)
		{
			m_crForeground = colour;
			Refresh();
		}
	}

	void VirtualWnd::SetFont(const wxFont& ftText)
	{
		if (m_ftText != ftText)
		{
			m_ftText = ftText;
			Refresh();
		}
	}

	void VirtualWnd::Refresh()
	{
		if (m_pOwner)
		{
			m_dirtyRegion.pos.x = min(m_pos.x, m_dirtyRegion.pos.x);
			m_dirtyRegion.pos.y = min(m_pos.y, m_dirtyRegion.pos.y);

			int32_t right = max(m_pos.x + m_size.x, m_dirtyRegion.pos.x + m_dirtyRegion.size.x);
			int32_t bottom = max(m_pos.y + m_size.y, m_dirtyRegion.pos.y + m_dirtyRegion.size.y);

			m_dirtyRegion.size.x = right - m_dirtyRegion.pos.x + 1;
			m_dirtyRegion.size.y = bottom - m_dirtyRegion.pos.y + 1;

			m_pOwner->RefreshRect(wxRect(m_dirtyRegion.pos, m_dirtyRegion.size));
			m_dirtyRegion.SetRegion(m_pos, wxSize(m_size.x + 1, m_size.y + 1));
		}
	}

	void VirtualWnd::OnPaint(wxGraphicsContext* pGraphic)
	{
		if (0 != m_crBackground.GetAlpha())
		{
			pGraphic->SetBrush(m_brBackground);
			pGraphic->SetPen(*wxTRANSPARENT_PEN);
			pGraphic->DrawRectangle(m_pos.x, m_pos.y, m_size.x, m_size.y);
		}

		if (m_pControlRes)
		{
			IBackground*		pBackground = nullptr;

			m_pControlRes->SetStatus(m_eStatus);
			pBackground = m_pControlRes->Background();
			if (pBackground)
				pBackground->Draw(pGraphic, wxRect(m_pos, m_size));
		}
	}
}