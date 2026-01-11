#include "panel_graphic_element.h"

namespace why
{
	LineGraphic::LineGraphic(PenPtr& penPtr, const wxPoint& ptStart, const wxPoint& ptEnd, const std::string& strID)
		: m_penPtr(penPtr)
		, m_ptStart(ptStart)
		, m_ptEnd(ptEnd)
		, m_strID(strID)
	{

	}

	void LineGraphic::Draw(wxDC& dc)
	{
		dc.SetPen(*m_penPtr.get());
		dc.DrawLine(m_ptStart, m_ptEnd);
	}

	void LineGraphic::Expand(const wxRect& rcExpand, bool bExpandH, bool bExpandV)
	{
		if (bExpandH)
		{
			if (m_ptStart.x > rcExpand.x)
				m_ptStart.x += rcExpand.width;

			if (m_ptEnd.x > rcExpand.x)
				m_ptEnd.x += rcExpand.width;
		}

		if (bExpandV)
		{
			if (m_ptStart.y > rcExpand.x)
				m_ptStart.y += rcExpand.height;

			if (m_ptEnd.y > rcExpand.x)
				m_ptEnd.y += rcExpand.height;
		}
	}

	void LineGraphic::Contract(const wxRect& rcContract, bool bContractH, bool bContractV)
	{
		wxPoint			ptPos = rcContract.GetBottomRight();

		if (bContractH)
		{
			if (m_ptStart.x > ptPos.x)
				m_ptStart.x -= rcContract.width;

			if (m_ptEnd.x > ptPos.x)
				m_ptEnd.x -= rcContract.width;
		}

		if (bContractV)
		{
			if (m_ptStart.y > ptPos.x)
				m_ptStart.y -= rcContract.height;

			if (m_ptEnd.y > ptPos.x)
				m_ptEnd.y -= rcContract.height;
		}
	}

	PictureGraphic::PictureGraphic(PicturePtr& picturePtr, const wxRect& rcRect, const std::string& strID)
		: m_picturePtr(picturePtr)
		, m_rcRect(rcRect)
		, m_strID(strID)
	{

	}

	void PictureGraphic::Draw(wxDC& dc)
	{
		m_picturePtr->DrawImage(dc, m_rcRect.GetTopLeft(), m_rcRect.GetSize());
	}

	void PictureGraphic::Expand(const wxRect& rcExpand, bool bExpandH, bool bExpandV)
	{
		if (bExpandH)
		{
			if (m_rcRect.x > rcExpand.x)
			{
				m_rcRect.x += rcExpand.width;
			}
			else
			{
				if (m_rcRect.GetRight() >= rcExpand.x)
					m_rcRect.width += rcExpand.width;
			}
		}

		if (bExpandV)
		{
			if (m_rcRect.y > rcExpand.y)
			{
				m_rcRect.y += rcExpand.height;
			}
			else
			{
				if (m_rcRect.GetBottom() >= rcExpand.y)
					m_rcRect.height += rcExpand.height;
			}
		}
	}

	void PictureGraphic::Contract(const wxRect& rcContract, bool bContractH, bool bContractV)
	{
		wxPoint			ptPos = rcContract.GetBottomRight();

		if (bContractH)
		{
			if (m_rcRect.x > ptPos.x)
			{
				m_rcRect.x -= rcContract.width;
			}
			else
			{
				if (m_rcRect.GetRight() >= ptPos.x)
					m_rcRect.width -= rcContract.width;
			}
		}

		if (bContractV)
		{
			if (m_rcRect.y > ptPos.y)
			{
				m_rcRect.y -= rcContract.height;
			}
			else
			{
				if (m_rcRect.GetBottom() >= ptPos.y)
					m_rcRect.height -= rcContract.height;
			}
		}
	}

	BackgroudGraphic::BackgroudGraphic(BackgroundPtr& backgroundPtr, const wxRect& rcRect, const std::string& strID)
		: m_backgroundPtr(backgroundPtr)
		, m_rcRect(rcRect)
		, m_strID(strID)
	{

	}

	void BackgroudGraphic::Draw(wxDC& dc)
	{
		if (nullptr != m_backgroundPtr) {
			m_backgroundPtr->Draw(dc, m_rcRect);
		}
	}

	void BackgroudGraphic::Expand(const wxRect& rcExpand, bool bExpandH, bool bExpandV)
	{
		if (bExpandH)
		{
			if (m_rcRect.x > rcExpand.x)
			{
				m_rcRect.x += rcExpand.width;
			}
			else
			{
				if (m_rcRect.GetRight() >= rcExpand.x)
					m_rcRect.width += rcExpand.width;
			}
		}

		if (bExpandV)
		{
			if (m_rcRect.y > rcExpand.y)
			{
				m_rcRect.y += rcExpand.height;
			}
			else
			{
				if (m_rcRect.GetBottom() >= rcExpand.y)
					m_rcRect.height += rcExpand.height;
			}
		}
	}

	void BackgroudGraphic::Contract(const wxRect& rcContract, bool bContractH, bool bContractV)
	{
		wxPoint			ptPos = rcContract.GetBottomRight();

		if (bContractH)
		{
			if (m_rcRect.x > ptPos.x)
			{
				m_rcRect.x -= rcContract.width;
			}
			else
			{
				if (m_rcRect.GetRight() >= ptPos.x)
					m_rcRect.width -= rcContract.width;
			}
		}

		if (bContractV)
		{
			if (m_rcRect.y > ptPos.y)
			{
				m_rcRect.y -= rcContract.height;
			}
			else
			{
				if (m_rcRect.GetBottom() >= ptPos.y)
					m_rcRect.height -= rcContract.height;
			}
		}
	}

	TextGraphic::TextGraphic(FontPtr& fontPtr, const wxColor& rcColor, const wxString& strText, const wxRect& rcRect, const std::string& strID, uint32_t& uStyle)
		: m_fontPtr(fontPtr)
		, m_rcColor(rcColor)
		, m_strText(strText)
		, m_rcRect(rcRect)
		, m_strID(strID)
		, m_uStyle(uStyle)
	{

	}

	void TextGraphic::Draw(wxDC& dc)
	{
		wxSize			szSize;
		wxPoint			ptPos;

		dc.SetFont(*m_fontPtr.get());
		dc.SetTextForeground(m_rcColor);

		szSize = dc.GetTextExtent(m_strText);

		if (m_uStyle & wxTE_CENTER)
		{
			ptPos.x = m_rcRect.x + (m_rcRect.width - szSize.x) / 2;
		}
		else if (m_uStyle & wxTE_RIGHT)
		{
			ptPos.x = m_rcRect.x + m_rcRect.width - szSize.x;
		}
		else
		{
			ptPos.x = m_rcRect.x;
		}
		ptPos.y = m_rcRect.y + (m_rcRect.height - szSize.y) / 2;

		dc.DrawText(m_strText, ptPos);
	}

	void TextGraphic::Expand(const wxRect& rcExpand, bool bExpandH, bool bExpandV)
	{
		if (bExpandH)
		{
			if (m_rcRect.x > rcExpand.x)
			{
				m_rcRect.x += rcExpand.width;
			}
			else
			{
				if (m_rcRect.GetRight() >= rcExpand.x)
					m_rcRect.width += rcExpand.width;
			}
		}

		if (bExpandV)
		{
			if (m_rcRect.y > rcExpand.y)
			{
				m_rcRect.y += rcExpand.height;
			}
			else
			{
				if (m_rcRect.GetBottom() >= rcExpand.y)
					m_rcRect.height += rcExpand.height;
			}
		}
	}

	void TextGraphic::Contract(const wxRect& rcContract, bool bContractH, bool bContractV)
	{
		wxPoint			ptPos = rcContract.GetBottomRight();

		if (bContractH)
		{
			if (m_rcRect.x > ptPos.x)
			{
				m_rcRect.x -= rcContract.width;
			}
			else
			{
				if (m_rcRect.GetRight() >= ptPos.x)
					m_rcRect.width -= rcContract.width;
			}
		}

		if (bContractV)
		{
			if (m_rcRect.y > ptPos.y)
			{
				m_rcRect.y -= rcContract.height;
			}
			else
			{
				if (m_rcRect.GetBottom() >= ptPos.y)
					m_rcRect.height -= rcContract.height;
			}
		}
	}

}
