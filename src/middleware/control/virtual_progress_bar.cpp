#include "virtual_progress_bar.h"
#include "singleton.h"
#include "resource_manager.h"
#include "virtual_wnd_owner.h"
#include "ui_util.h"
#include "logger.h"

namespace why
{
	VirtualProgressBar::VirtualProgressBar(VirtualWndOwner* parent, const wxString& label, const wxRect& rcRect, const wxString& name)
		: VirtualWnd(parent, label, rcRect, name)
	{

	}

	void VirtualProgressBar::OnPaint(wxGraphicsContext* pGraphic)
	{
		VirtualWnd::OnPaint(pGraphic);

		m_rcClient.SetPosition(GetPosition());
		m_rcClient.SetSize(GetSize());
		if (m_bHorizontal)
			OnPaintHorizontal(pGraphic, m_rcClient);
		else
			OnPaintVertical(pGraphic, m_rcClient);
	}

	void VirtualProgressBar::OnPaintHorizontal(wxGraphicsContext* pGraphic, const wxRect& rcClient)
	{
		wxRect			rcDraw;

		GetProgressBarRect(rcDraw);
		if (nullptr != m_normalPtr.get())
			m_normalPtr->Draw(pGraphic, rcDraw);
		
		if (nullptr != m_selectedPtr.get())
		{
			int32_t			nWidth;

			nWidth = rcClient.width - m_sliderPtr->Width();
			rcDraw.width = nWidth * (m_nPos - m_nMin) / (m_nMax - m_nMin);
			m_selectedPtr->Draw(pGraphic, rcDraw);
		}

		if (nullptr != m_sliderPtr.get())
		{
			m_rcSlider.x = rcDraw.x + rcDraw.width;
			m_rcSlider.y = rcClient.y + (rcClient.height - m_sliderPtr->Height()) / 2;
			m_rcSlider.width = m_sliderPtr->Width();
			m_rcSlider.height = m_sliderPtr->Height();
			if (m_rcSlider.y < rcClient.y)
			{
				m_rcSlider.y = rcClient.y + 1;
				m_rcSlider.height = rcClient.height - 2;
			}
			m_sliderPtr->DrawImage(pGraphic, m_rcSlider);
		}
	}

	void VirtualProgressBar::OnPaintVertical(wxGraphicsContext* pGraphic, const wxRect& rcClient)
	{
		wxRect			rcDraw;

		GetProgressBarRect(rcDraw);
		if (nullptr != m_normalPtr.get())
			m_normalPtr->Draw(pGraphic, rcDraw);

		if (nullptr != m_selectedPtr.get())
		{
			int32_t			nHeight;

			nHeight = rcClient.height - m_sliderPtr->Height();
			rcDraw.height = nHeight * (m_nPos - m_nMin) / (m_nMax - m_nMin);
			m_selectedPtr->Draw(pGraphic, rcDraw);
		}

		if (nullptr != m_sliderPtr.get())
		{
			m_rcSlider.x = rcClient.x + (rcClient.width - m_sliderPtr->Width()) / 2;
			m_rcSlider.y = rcDraw.y + rcDraw.height;
			m_rcSlider.width = m_sliderPtr->Width();
			m_rcSlider.height = m_sliderPtr->Height();
			if (m_rcSlider.x < rcClient.x)
			{
				m_rcSlider.x = rcClient.x + 1;
				m_rcSlider.width = rcClient.width - 2;
			}
			m_sliderPtr->DrawImage(pGraphic, m_rcSlider);
		}
	}

	bool VirtualProgressBar::OnLButtonDown(wxMouseEvent& event)
	{
		wxPoint			ptPos = event.GetPosition();

		if (PtInRect(ptPos, m_rcSlider))
		{
			m_bMouseDown = true;
			return true;
		}
		else if (m_bOnlyBackgroundClicked)
		{
			wxRect			rcBar;

			GetProgressBarRect(rcBar);
			if (PtInRect(ptPos, rcBar))
			{
				auto nPos = CalculatePos(ptPos);
				SetPosition(nPos, true);
				return true;
			}
		}
		else if (PtInRect(ptPos, m_rcClient))
		{
			auto nPos = CalculatePos(ptPos);
			SetPosition(nPos, true);
			return true;
		}

		return false;
	}

	void VirtualProgressBar::OnLButtonUp(wxMouseEvent& event)
	{
		m_bMouseDown = false;
	}

	void VirtualProgressBar::OnMouseMove(wxMouseEvent& event)
	{
		if (m_bMouseDown)
		{
			wxPoint			ptPos = event.GetPosition();
			int32_t			nPos = 0;
			
			nPos = CalculatePos(ptPos);
			SetPosition(nPos, true);
		}
	}

	void VirtualProgressBar::OnEnterWindow(wxMouseEvent& event)
	{

	}

	void VirtualProgressBar::OnLeaveWindow(wxMouseEvent& event)
	{

	}

	void VirtualProgressBar::SetDirection(bool bHorizontal)
	{
		if (m_bHorizontal != bHorizontal)
		{
			m_bHorizontal = bHorizontal;
			Refresh();
		}
	}

	void VirtualProgressBar::SetClickRange(bool bOnlyBackgroundClicked)
	{
		m_bOnlyBackgroundClicked = bOnlyBackgroundClicked;
	}

	void VirtualProgressBar::AddBackground(BackgroundPtr& normalPtr, BackgroundPtr& selectedPtr, int32_t nHeight)
	{
		bool			bRefresh = false;
		
		if (m_normalPtr.get() != normalPtr.get())
		{
			m_normalPtr = normalPtr;
			bRefresh = true;
		}

		if (m_selectedPtr.get() != selectedPtr.get())
		{
			m_selectedPtr = selectedPtr;
			bRefresh = true;
		}

		if (m_nHeight != nHeight)
		{
			m_nHeight = nHeight;
			bRefresh = true;
		}

		if (bRefresh)
			Refresh();
	}

	void VirtualProgressBar::AddSlider(PicturePtr& sliderPtr)
	{
		if (m_sliderPtr.get() != sliderPtr.get())
		{
			m_sliderPtr = sliderPtr;
			Refresh();
		}
	}

	void VirtualProgressBar::GetRange(int32_t& nMin, int32_t& nMax) const
	{
		nMin = m_nMin;
		nMax = m_nMax;
	}

	void VirtualProgressBar::SetRange(int32_t nMin, int32_t nMax)
	{
		if (nMin < nMax)
		{
			if ((m_nMin != nMin) || (m_nMax != nMax))
			{
				m_nMin = nMin;
				m_nMax = nMax;
				Refresh();
			}
		}
		else
		{
			if ((m_nMin != nMax) || (m_nMax != nMin))
			{
				m_nMin = nMax;
				m_nMax = nMin;
				Refresh();
			}
		}
	}

	void VirtualProgressBar::SetPos(int32_t nPos)
	{
		SetPosition(nPos, false);
	}

	int32_t VirtualProgressBar::GetPos() const
	{
		return m_nPos;
	}

	void VirtualProgressBar::GetProgressBarRect(wxRect& rcBar)
	{
		if (m_bHorizontal)
		{
			rcBar.x = m_rcClient.x;
			rcBar.y = m_rcClient.y + (m_rcClient.height - m_nHeight) / 2;
			rcBar.width = m_rcClient.width;
			rcBar.height = m_nHeight;
		}
		else
		{
			rcBar.x = m_rcClient.x + (m_rcClient.width - m_nHeight) / 2;
			rcBar.y = m_rcClient.y;
			rcBar.width = m_nHeight;
			rcBar.height = m_rcClient.height;
		}
	}

	int32_t VirtualProgressBar::CalculatePos(const wxPoint &ptPos)
	{
		wxSize				szSize;
		int32_t				nPos = 0;
		int32_t				x = ptPos.x;
		int32_t				y = ptPos.y;

		if (nullptr != m_sliderPtr.get())
		{
			szSize.x = m_sliderPtr->Width();
			szSize.y = m_sliderPtr->Height();
		}

		if (x < m_rcClient.x)
			x = m_rcClient.x;

		if (y < m_rcClient.y)
			y = m_rcClient.y;

		if (m_bHorizontal)
		{
			int32_t				nRight = m_rcClient.x + m_rcClient.width - szSize.x;
			
			if (x > nRight)
				x = nRight;

			if (y < (m_rcClient.y + m_rcClient.height))
				y = m_rcClient.y + m_rcClient.height;

			nPos = m_nMin + (x - m_rcClient.x) * (m_nMax - m_nMin) / (nRight - m_rcClient.x);
		}
		else
		{
			int32_t				nBottom = m_rcClient.y + m_rcClient.height - szSize.y;
			
			if (x > (m_rcClient.x + m_rcClient.width))
				x = m_rcClient.x + m_rcClient.width;

			if (y < nBottom)
				y = nBottom;

			nPos = m_nMin + (y - m_rcClient.y) * (m_nMax - m_nMin) / (nBottom - m_rcClient.y);
		}

		if (nPos < m_nMin)
			nPos = m_nMin;

		if (nPos >= m_nMax)
			nPos = m_nMax;

		return nPos;
	}

	void VirtualProgressBar::SetPosition(int32_t nPos, bool bNotify)
	{
		if (m_nPos != nPos)
		{
			m_nPos = nPos;
			Refresh();

			if (bNotify)
			{
				wxCommandEvent			commandEvent(wxEVT_SLIDER);

				commandEvent.SetEventObject(this);
				commandEvent.SetInt(m_nPos);
				AddPendingEvent(commandEvent);
			}
		}
	}
}