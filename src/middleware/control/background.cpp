#include "background.h"
#include "util.h"
#include "logger.h"

namespace why
{
	SolidBackground::SolidBackground(const wxColor& crFill, const wxColor& crColor, uint32_t uBorderWidth)
	{
		m_fillBrushPtr.reset(new wxBrush(crFill));

		m_borderWidth = uBorderWidth;
		if (m_borderWidth > 0)
			m_borderPenPtr.reset(new wxPen(crColor, (int32_t)uBorderWidth, wxPENSTYLE_SOLID));//纯色风格
	}

	SolidBackground::~SolidBackground()
	{

	}

	bool SolidBackground::Draw(wxDC& dc, const wxRect& rcRect)
	{
		if (!rcRect.IsEmpty())
		{
			dc.SetBrush(*m_fillBrushPtr);
			if (0 == m_borderWidth)
				dc.SetPen(*wxTRANSPARENT_PEN);// 透明笔
			else
				dc.SetPen(*m_borderPenPtr.get());
			dc.DrawRectangle(rcRect);
			if (0 != wxTRANSPARENT_PEN)
				dc.SetPen(*wxTRANSPARENT_PEN);
			dc.SetBrush(*wxTRANSPARENT_BRUSH);
		}

		return true;
	}

	bool SolidBackground::Draw(wxGraphicsContext* pGraphic, const wxRect& rcRect)
	{
		if (!rcRect.IsEmpty())
		{
			pGraphic->SetBrush(*m_fillBrushPtr);
			if (0 == m_borderWidth)
				pGraphic->SetPen(*wxTRANSPARENT_PEN);
			else
				pGraphic->SetPen(*m_borderPenPtr.get());
			pGraphic->DrawRectangle(rcRect.x, rcRect.y, rcRect.width, rcRect.height);
			if (0 != wxTRANSPARENT_PEN)
				pGraphic->SetPen(*wxTRANSPARENT_PEN);
			pGraphic->SetBrush(*wxTRANSPARENT_BRUSH);
		}

		return true;
	}

	FadeBackground::FadeBackground(const wxColor& crStart, const wxColor& crEnd, bool bHorizontal, const wxColor& crBorder, uint32_t uBorderWidth)
		: m_startColor(crStart)
		, m_endColor(crEnd)
		, m_bHorizontal(bHorizontal)
	{
		if (uBorderWidth > 0)
			m_borderPenPtr.reset(new wxPen(crBorder, (int32_t)uBorderWidth, wxPENSTYLE_SOLID));
	}

	FadeBackground::~FadeBackground()
	{

	}

	bool FadeBackground::Draw(wxDC& dc, const wxRect& rcRect)
	{
		dc.GradientFillLinear(rcRect, m_startColor, m_endColor, m_bHorizontal ? wxEAST : wxBOTTOM);
		if (m_borderPenPtr)
		{
			dc.SetBrush(*wxTRANSPARENT_BRUSH);
			dc.SetPen(*m_borderPenPtr.get());
			dc.DrawRectangle(rcRect);
			dc.SetPen(*wxTRANSPARENT_PEN);
		}

		return true;
	}

	bool FadeBackground::Draw(wxGraphicsContext* pGraphic, const wxRect& rcRect)
	{
		wxGraphicsGradientStops stops(m_startColor, m_endColor);
		wxGraphicsBrush			brush; 
		
		if (m_bHorizontal)
			brush = pGraphic->CreateLinearGradientBrush(rcRect.x, rcRect.y, rcRect.x + rcRect.width, rcRect.y, stops);
		else
			brush = pGraphic->CreateLinearGradientBrush(rcRect.x, rcRect.y, rcRect.x, rcRect.y + rcRect.height, stops);
		pGraphic->SetBrush(brush);

		if (m_borderPenPtr)
			pGraphic->SetPen(*m_borderPenPtr.get());
		else
			pGraphic->SetPen(*wxTRANSPARENT_PEN);

		pGraphic->DrawRectangle(rcRect.x, rcRect.y, rcRect.width, rcRect.height);
		return true;
	}

	PictureBackground::PictureBackground(PicturePtr& imagePtr, const wxRect& rcRect, const wxSize& szLeftTop, const wxSize& szRightBottom)
		: m_imagePtr(imagePtr)
		, m_rcRect(rcRect)
		, m_szLeftTop(szLeftTop)
		, m_szRightBottom(szRightBottom)
	{
		if (m_rcRect.IsEmpty())
		{
			m_rcRect.x = 0;
			m_rcRect.y = 0;
			m_rcRect.width = m_imagePtr->Width();
			m_rcRect.height = m_imagePtr->Height();
		}
	}

	PictureBackground::~PictureBackground()
	{
		
	}

	wxColour PictureBackground::GetCenterPixel()
	{
		int32_t				x = m_rcRect.width / 2;
		int32_t				y = m_rcRect.height / 2;

		return m_imagePtr->GetPixel({ x, y });
	}

	bool PictureBackground::CreateCache(const wxColour &crBlend)
	{
		wxBitmap			cacheBitmap(m_imagePtr->Width(), m_imagePtr->Height());
		wxMemoryDC			memoryDC;
		wxBrush				brBlend(crBlend);

		memoryDC.SelectObject(cacheBitmap);
		memoryDC.SetBrush(brBlend);
		memoryDC.SetPen(*wxTRANSPARENT_PEN);
		memoryDC.DrawRectangle(0, 0, m_imagePtr->Width(), m_imagePtr->Height());
		m_imagePtr->DrawImage(memoryDC, { 0,0 });
		memoryDC.SetBrush(*wxTRANSPARENT_BRUSH);
		memoryDC.SelectObject(wxNullBitmap);
		
		m_imagePtr.reset(new Picture());
		m_bCanCache = true;
		if (!m_imagePtr->CreatePicture(cacheBitmap))
			return false;

		//LOG_INFO << "Create Cahce[" << this << "] color:" << crBlend.GetRGB();
		m_cacheBufs.insert(std::make_pair(std::make_pair(m_imagePtr->Width(), m_imagePtr->Height()), cacheBitmap));
		return true;
	}

	bool PictureBackground::GenerateCacheBuffer(uint32_t uWidth, uint32_t uHeight)
	{
		BufSize			szBuffer(uWidth, uHeight);
		auto			itFind = m_cacheBufs.find(szBuffer);

		if (m_cacheBufs.end() != itFind)
			return true;

		wxBitmap			cacheBitmap(uWidth, uHeight);
		wxMemoryDC			memoryDC;

		memoryDC.SelectObject(cacheBitmap);
		Draw(memoryDC, { 0, 0, (int32_t)uWidth, (int32_t)uHeight });
		memoryDC.SelectObject(wxNullBitmap);
		m_cacheBufs.insert(std::make_pair(szBuffer, cacheBitmap));

		//LOG_INFO << "Generate Cahce buffer[" << this << "] width:" << uWidth << ", height:" << uHeight;
		return true;
	}

	bool PictureBackground::Draw(wxDC& dc, const wxRect& rcRect)
	{
		if (m_imagePtr)
		{
			if (!m_cacheBufs.empty())
			{
				BufSize			szBuffer(rcRect.width, rcRect.height);
				auto			itFind = m_cacheBufs.find(szBuffer);

				if (itFind != m_cacheBufs.end())
				{
					dc.DrawBitmap(itFind->second, rcRect.GetPosition(), false);
					return true;
				}
			}

			if ((0 != m_szLeftTop.x) || (0 != m_szLeftTop.y) || (0 != m_szRightBottom.x) || (0 != m_szRightBottom.y))
			{	
				// 绘制左上角
				if ((0 != m_szLeftTop.x) && (0 != m_szLeftTop.y))
					DrawLeftTop(dc, rcRect);
				
				// 绘制上边
				if (0 != m_szLeftTop.y)
					DrawTop(dc, rcRect);

				// 绘制右上边
				if ((0 != m_szRightBottom.x) && (0 != m_szLeftTop.y))
					DrawRightTop(dc, rcRect);

				// 绘制左边
				if (0 != m_szLeftTop.x)
					DrawLeft(dc, rcRect);

				// 绘制中间
				DrawCenter(dc, rcRect);

				// 绘制左边
				if (0 != m_szRightBottom.x)
					DrawRight(dc, rcRect);

				// 绘制左下角
				if ((0 != m_szLeftTop.x) && (0 != m_szRightBottom.y))
					DrawLeftBottom(dc, rcRect);

				// 绘制下边
				if (0 != m_szRightBottom.y)
					DrawBottom(dc, rcRect);

				// 绘制右下边
				if ((0 != m_szRightBottom.x) && (0 != m_szRightBottom.y))
					DrawRightBottom(dc, rcRect);

				//LOG_INFO << "Picture[" << this << "] " << "width:" << rcRect.width << ", height:" << rcRect.height << ", time:" << GetTimeStamp64() - uStart;
			}
			else
			{
				m_imagePtr->DrawImage(dc, rcRect, m_rcRect);
			}
		}

		return TRUE;
	}

	bool PictureBackground::Draw(wxGraphicsContext* pGraphic, const wxRect& rcRect)
	{
		if (m_imagePtr)
		{
			if ((0 != m_szLeftTop.x) || (0 != m_szLeftTop.y) || (0 != m_szRightBottom.x) || (0 != m_szRightBottom.y))
			{
				// 绘制左上角
				if ((0 != m_szLeftTop.x) && (0 != m_szLeftTop.y))
					DrawLeftTop(pGraphic, rcRect);

				// 绘制上边
				if (0 != m_szLeftTop.y)
					DrawTop(pGraphic, rcRect);

				// 绘制右上边
				if ((0 != m_szRightBottom.x) && (0 != m_szLeftTop.y))
					DrawRightTop(pGraphic, rcRect);

				// 绘制左边
				if (0 != m_szLeftTop.x)
					DrawLeft(pGraphic, rcRect);

				// 绘制中间
				DrawCenter(pGraphic, rcRect);

				// 绘制左边
				if (0 != m_szRightBottom.x)
					DrawRight(pGraphic, rcRect);

				// 绘制左下角
				if ((0 != m_szLeftTop.x) && (0 != m_szRightBottom.y))
					DrawLeftBottom(pGraphic, rcRect);

				// 绘制下边
				if (0 != m_szRightBottom.y)
					DrawBottom(pGraphic, rcRect);

				// 绘制右下边
				if ((0 != m_szRightBottom.x) && (0 != m_szRightBottom.y))
					DrawRightBottom(pGraphic, rcRect);

				// LOG_INFO << "Picture[" << this << "] time:" << GetTimeStamp64() - uStart;
			}
			else
			{
				m_imagePtr->DrawImage(pGraphic, rcRect, m_rcRect);
			}
		}

		return TRUE;
	}

	bool PictureBackground::GetDrawRectForLeftTop(const wxRect& rcRect, wxRect& rcDes, wxRect& rcSrc)
	{
		wxRect			rcDraw = { rcRect.x, rcRect.y, m_szLeftTop.x, m_szLeftTop.y };
		
		rcDes = rcDraw.Intersect(rcRect);
		if (rcDes.IsEmpty())
		{
			return false;
		}
		else
		{
			rcSrc = { m_rcRect.x, m_rcRect.y, m_szLeftTop.x, m_szLeftTop.y };
			if (rcSrc.width > rcDes.width)
				rcSrc.width = rcDes.width;

			if (rcSrc.height > rcDes.height)
				rcSrc.height = rcDes.height;

			return true;
		}
	}
	bool PictureBackground::GetDrawRectForTop(const wxRect& rcRect, wxRect& rcDes, wxRect& rcSrc)
	{
		wxRect			rcDraw;

		if (rcRect.width <= (m_szLeftTop.x + m_szRightBottom.x))
			return false;

		rcDraw.x = rcRect.x + m_szLeftTop.x;
		rcDraw.y = rcRect.y;
		rcDraw.width = rcRect.width - m_szLeftTop.x - m_szRightBottom.x;
		rcDraw.height = m_szLeftTop.y;
		rcDes = rcDraw.Intersect(rcRect);

		if (rcDes.IsEmpty())
		{
			return false;
		}
		else
		{
			rcSrc.x = m_rcRect.x + m_szLeftTop.x;
			rcSrc.y = m_rcRect.y;
			rcSrc.width = m_rcRect.width - m_szLeftTop.x - m_szRightBottom.x;
			rcSrc.height = m_szLeftTop.y;
			if (rcSrc.IsEmpty())
			{
				return false;
			}
			else
			{
				if (rcSrc.height > rcDes.height)
					rcSrc.height = rcDes.height;

				return true;
			}
		}
	}

	bool PictureBackground::GetDrawRectForRightTop(const wxRect& rcRect, wxRect& rcDes, wxRect& rcSrc)
	{
		wxRect			rcDraw;

		if (rcRect.width <= m_szLeftTop.x)
			return false;

		rcDraw.x = rcRect.x + rcRect.width - m_szRightBottom.x;
		rcDraw.y = rcRect.y;
		rcDraw.width = m_szRightBottom.x;
		rcDraw.height = m_szLeftTop.y;
		rcDes = rcDraw.Intersect(rcRect);

		if (rcDes.IsEmpty())
		{
			return false;
		}
		else
		{
			rcSrc.x = m_rcRect.x + m_rcRect.width - m_szRightBottom.x;
			rcSrc.y = m_rcRect.y;
			rcSrc.width = m_szRightBottom.x;
			rcSrc.height = m_szLeftTop.y;

			if (rcSrc.width > rcDes.width)
			{
				rcSrc.x = m_rcRect.x + m_rcRect.width - rcDes.width;
				rcSrc.width = rcDes.width;
			}

			if (rcSrc.height > rcDes.height)
				rcSrc.height = rcDes.height;

			return true;
		}
	}
	
	bool PictureBackground::GetDrawRectForLeft(const wxRect& rcRect, wxRect& rcDes, wxRect& rcSrc)
	{
		wxRect			rcDraw;

		if (rcRect.height <= (m_szLeftTop.y + m_szRightBottom.y))
			return false;

		rcDraw.x = rcRect.x;
		rcDraw.y = rcRect.y + m_szLeftTop.y;
		rcDraw.width = m_szLeftTop.x;
		rcDraw.height = rcRect.height - m_szLeftTop.y - m_szRightBottom.y;
		rcDes = rcDraw.Intersect(rcRect);

		if (rcDes.IsEmpty())
		{
			return false;
		}
		else
		{
			rcSrc.x = m_rcRect.x;
			rcSrc.y = m_rcRect.y + m_szLeftTop.y;
			rcSrc.width = m_szLeftTop.x;
			rcSrc.height = m_rcRect.height - m_szLeftTop.y - m_szRightBottom.y;
			if (rcSrc.IsEmpty())
			{
				return false;
			}
			else
			{
				if (rcSrc.width > rcDes.width)
					rcSrc.width = rcDes.width;

				return true;
			}
		}
	}
	
	bool PictureBackground::GetDrawRectForCenter(const wxRect& rcRect, wxRect& rcDes, wxRect& rcSrc)
	{
		if (rcRect.width <= (m_szLeftTop.x + m_szRightBottom.x))
			return false;

		if (rcRect.height <= (m_szLeftTop.y + m_szRightBottom.y))
			return false;

		rcDes.x = rcRect.x + m_szLeftTop.x;
		rcDes.y = rcRect.y + m_szLeftTop.y;
		rcDes.width = rcRect.width - m_szLeftTop.x - m_szRightBottom.x;
		rcDes.height = rcRect.height - m_szLeftTop.y - m_szRightBottom.y;

		rcSrc.x = m_rcRect.x + m_szLeftTop.x;
		rcSrc.y = m_rcRect.y + m_szLeftTop.y;
		rcSrc.width = m_rcRect.width - m_szLeftTop.x - m_szRightBottom.x;
		rcSrc.height = m_rcRect.height - m_szLeftTop.y - m_szRightBottom.y;
		return true;
	}

	bool PictureBackground::GetDrawRectForRight(const wxRect& rcRect, wxRect& rcDes, wxRect& rcSrc)
	{
		wxRect			rcDraw;

		if (rcRect.height <= (m_szLeftTop.y + m_szRightBottom.y))
			return false;

		rcDraw.x = rcRect.x + rcRect.width - m_szRightBottom.x;
		rcDraw.y = rcRect.y + m_szLeftTop.y;
		rcDraw.width = m_szRightBottom.x;
		rcDraw.height = rcRect.height - m_szLeftTop.y - m_szRightBottom.y;
		rcDes = rcDraw.Intersect(rcRect);

		if (rcDes.IsEmpty())
		{
			return false;
		}
		else
		{
			rcSrc.x = m_rcRect.x + m_rcRect.width - m_szRightBottom.x;
			rcSrc.y = m_rcRect.y + m_szLeftTop.y;
			rcSrc.width = m_szRightBottom.x;
			rcSrc.height = m_rcRect.height - m_szLeftTop.y - m_szRightBottom.y;
			if (rcSrc.IsEmpty())
			{
				return false;
			}
			else
			{
				if (rcSrc.width > rcDes.width)
				{
					rcSrc.x = m_rcRect.x + m_rcRect.width - rcDes.width;
					rcSrc.width = rcDes.width;
				}

				return true;
			}
		}
	}

	bool PictureBackground::GetDrawRectForLeftBottom(const wxRect& rcRect, wxRect& rcDes, wxRect& rcSrc)
	{
		wxRect			rcDraw;

		if (rcRect.height <= m_szLeftTop.y)
			return false;

		rcDraw.x = rcRect.x;
		rcDraw.y = rcRect.y + rcRect.height - m_szRightBottom.y;
		rcDraw.width = m_szLeftTop.x;
		rcDraw.height = m_szRightBottom.y;
		rcDes = rcDraw.Intersect(rcRect);

		if (rcDes.IsEmpty())
		{
			return false;
		}
		else
		{
			rcSrc.x = m_rcRect.x;
			rcSrc.y = m_rcRect.y + m_rcRect.height - m_szRightBottom.y;
			rcSrc.width = m_szLeftTop.x;
			rcSrc.height = m_szRightBottom.y;
			if (rcSrc.IsEmpty())
			{
				return false;
			}
			else
			{
				if (rcSrc.width > rcDes.width)
					rcSrc.width = rcDes.width;

				if (rcSrc.height > rcDes.height)
				{
					rcSrc.y = m_rcRect.y + m_rcRect.height - rcDes.height;
					rcSrc.height = rcDes.height;
				}

				return true;
			}
		}
	}
		
	bool PictureBackground::GetDrawRectForBottom(const wxRect& rcRect, wxRect& rcDes, wxRect& rcSrc)
	{
		wxRect			rcDraw;

		if (rcRect.height <= m_szLeftTop.y)
			return false;

		if (rcRect.width <= (m_szLeftTop.x + m_szRightBottom.x))
			return false;

		rcDraw.x = rcRect.x + m_szLeftTop.x;
		rcDraw.y = rcRect.y + rcRect.height - m_szRightBottom.y;
		rcDraw.width = rcRect.width - m_szLeftTop.x - m_szRightBottom.x;
		rcDraw.height = m_szRightBottom.y;
		rcDes = rcDraw.Intersect(rcRect);

		if (rcDes.IsEmpty())
		{
			return false;
		}
		else
		{
			rcSrc.x = m_rcRect.x + m_szLeftTop.x;
			rcSrc.y = m_rcRect.y + m_rcRect.height - m_szRightBottom.y;
			rcSrc.width = m_rcRect.width - m_szLeftTop.x - m_szRightBottom.x;
			rcSrc.height = m_szRightBottom.y;
			if (rcSrc.IsEmpty())
			{
				return false;
			}
			else
			{
				if (rcSrc.height > rcDes.height)
					rcSrc.height = rcDes.height;

				return true;
			}
		}
	}

	bool PictureBackground::GetDrawRectForRightBottom(const wxRect& rcRect, wxRect& rcDes, wxRect& rcSrc)
	{
		wxRect			rcDraw;

		if (rcRect.width <= m_szLeftTop.x)
			return false;

		if (rcRect.height <= m_szLeftTop.y)
			return false;

		rcDraw.x = rcRect.x + rcRect.width - m_szRightBottom.x;
		rcDraw.y = rcRect.y + rcRect.height - m_szRightBottom.y;
		rcDraw.width = m_szRightBottom.x;
		rcDraw.height = m_szRightBottom.y;
		rcDes = rcDraw.Intersect(rcRect);

		if (rcDes.IsEmpty())
		{
			return false;
		}
		else
		{
			rcSrc.x = m_rcRect.x + m_rcRect.width - m_szRightBottom.x;
			rcSrc.y = m_rcRect.y + m_rcRect.height - m_szRightBottom.y;
			rcSrc.width = m_szRightBottom.x;
			rcSrc.height = m_szRightBottom.y;

			if (rcSrc.width > rcDes.width)
			{
				rcSrc.x = m_rcRect.x + m_rcRect.width - rcDes.width;
				rcSrc.width = rcDes.width;
			}

			if (rcSrc.height > rcDes.height)
			{
				rcSrc.y = m_rcRect.y + m_rcRect.height - rcDes.height;
				rcSrc.height = rcDes.height;
			}

			return true;
		}
	}

	void PictureBackground::DrawLeftTop(wxDC& dc, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForLeftTop(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(dc, rcDes, rcSrc);
	}

	void PictureBackground::DrawTop(wxDC& dc, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForTop(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(dc, rcDes, rcSrc);
	}

	void PictureBackground::DrawRightTop(wxDC& dc, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForRightTop(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(dc, rcDes, rcSrc);
	}

	void PictureBackground::DrawLeft(wxDC& dc, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForLeft(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(dc, rcDes, rcSrc);
	}

	void PictureBackground::DrawCenter(wxDC& dc, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForCenter(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(dc, rcDes, rcSrc);
	}

	void PictureBackground::DrawRight(wxDC& dc, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForRight(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(dc, rcDes, rcSrc);
	}

	void PictureBackground::DrawLeftBottom(wxDC& dc, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForLeftBottom(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(dc, rcDes, rcSrc);
	}

	void PictureBackground::DrawBottom(wxDC& dc, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForBottom(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(dc, rcDes, rcSrc);
	}

	void PictureBackground::DrawRightBottom(wxDC& dc, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForRightBottom(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(dc, rcDes, rcSrc);
	}

	void PictureBackground::DrawLeftTop(wxGraphicsContext* pGraphic, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForLeftTop(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(pGraphic, rcDes, rcSrc);
	}

	void PictureBackground::DrawTop(wxGraphicsContext* pGraphic, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForTop(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(pGraphic, rcDes, rcSrc);
	}

	void PictureBackground::DrawRightTop(wxGraphicsContext* pGraphic, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForRightTop(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(pGraphic, rcDes, rcSrc);
	}

	void PictureBackground::DrawLeft(wxGraphicsContext* pGraphic, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForLeft(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(pGraphic, rcDes, rcSrc);
	}

	void PictureBackground::DrawCenter(wxGraphicsContext* pGraphic, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForCenter(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(pGraphic, rcDes, rcSrc);
	}

	void PictureBackground::DrawRight(wxGraphicsContext* pGraphic, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForRight(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(pGraphic, rcDes, rcSrc);
	}

	void PictureBackground::DrawLeftBottom(wxGraphicsContext* pGraphic, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForLeftBottom(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(pGraphic, rcDes, rcSrc);
	}

	void PictureBackground::DrawBottom(wxGraphicsContext* pGraphic, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForBottom(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(pGraphic, rcDes, rcSrc);
	}

	void PictureBackground::DrawRightBottom(wxGraphicsContext* pGraphic, const wxRect& rcRect)
	{
		wxRect			rcDes;
		wxRect			rcSrc;

		if (GetDrawRectForRightBottom(rcRect, rcDes, rcSrc))
			m_imagePtr->DrawImage(pGraphic, rcDes, rcSrc);
	}

}
