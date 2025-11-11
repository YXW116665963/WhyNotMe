#pragma once

#include "background_interface.h"
#include "picture.h"

namespace why
{
	//单色背景
	class SolidBackground : public IBackground
	{
	public:
		SolidBackground(const wxColor& crFill, const wxColor& crColor, uint32_t uBorderWidth);
		~SolidBackground();

		bool Draw(wxDC &dc, const wxRect &rcRect) override;
		bool Draw(wxGraphicsContext* pGraphic, const wxRect& rcRect) override;
		void Destroy() override { delete this; }

	private:
		typedef std::unique_ptr<wxBrush> BrushPtr;
		typedef std::unique_ptr<wxPen> PenPtr;
		
	private:
		BrushPtr		m_fillBrushPtr;		//!< 背景的颜色
		PenPtr			m_borderPenPtr;		//!< 绘制边框的画笔
		uint32_t		m_borderWidth;		//!< 边框画笔的宽带
	};

	//渐变色背景
	class FadeBackground : public IBackground
	{
	public:
		FadeBackground(const wxColor &crStart, const wxColor& crEnd, bool bHorizontal, const wxColor& crBorder, uint32_t uBorderWidth);
		~FadeBackground();
		
		bool Draw(wxDC& dc, const wxRect& rcRect) override;
		bool Draw(wxGraphicsContext* pGraphic, const wxRect& rcRect) override;
		void Destroy() override { delete this; }

	private:
		typedef std::unique_ptr<wxPen> PenPtr;

	private:
		wxColour		m_startColor;					//!< 渐变色的起始颜色
		wxColour		m_endColor;						//!< 渐变色的结束颜色
		bool			m_bHorizontal;					//!< 颜色渐变的方向
		PenPtr			m_borderPenPtr;					//!< 绘制边框的画笔
	};

	//图片背景
	class PictureBackground : public IBackground
	{
	public:
		PictureBackground(PicturePtr &imagePtr, const wxRect &rcRect, const wxSize &szLeftTop, const wxSize &szRightBottom);
		~PictureBackground();

		bool Draw(wxDC& dc, const wxRect& rcRect) override;
		bool Draw(wxGraphicsContext* pGraphic, const wxRect& rcRect) override;
		void Destroy() override { delete this; }

	public:
		wxColour GetCenterPixel();
		bool CreateCache(const wxColour &crBlend);
		bool GenerateCacheBuffer(uint32_t uWidth, uint32_t uHeight);

	private:
		void DrawLeftTop(wxDC& dc, const wxRect& rcRect);
		void DrawTop(wxDC& dc, const wxRect& rcRect);
		void DrawRightTop(wxDC& dc, const wxRect& rcRect);
		void DrawLeft(wxDC& dc, const wxRect& rcRect);
		void DrawCenter(wxDC& dc, const wxRect& rcRect);
		void DrawRight(wxDC& dc, const wxRect& rcRect);
		void DrawLeftBottom(wxDC& dc, const wxRect& rcRect);
		void DrawBottom(wxDC& dc, const wxRect& rcRect);
		void DrawRightBottom(wxDC& dc, const wxRect& rcRect);

		void DrawLeftTop(wxGraphicsContext* pGraphic, const wxRect& rcRect);
		void DrawTop(wxGraphicsContext* pGraphic, const wxRect& rcRect);
		void DrawRightTop(wxGraphicsContext* pGraphic, const wxRect& rcRect);
		void DrawLeft(wxGraphicsContext* pGraphic, const wxRect& rcRect);
		void DrawCenter(wxGraphicsContext* pGraphic, const wxRect& rcRect);
		void DrawRight(wxGraphicsContext* pGraphic, const wxRect& rcRect);
		void DrawLeftBottom(wxGraphicsContext* pGraphic, const wxRect& rcRect);
		void DrawBottom(wxGraphicsContext* pGraphic, const wxRect& rcRect);
		void DrawRightBottom(wxGraphicsContext* pGraphic, const wxRect& rcRect);

		bool GetDrawRectForLeftTop(const wxRect& rcRect, wxRect& rcDest, wxRect& rcSrc);
		bool GetDrawRectForTop(const wxRect& rcRect, wxRect& rcDest, wxRect& rcSrc);
		bool GetDrawRectForRightTop(const wxRect& rcRect, wxRect& rcDest, wxRect& rcSrc);
		bool GetDrawRectForLeft(const wxRect& rcRect, wxRect& rcDest, wxRect& rcSrc);
		bool GetDrawRectForCenter(const wxRect& rcRect, wxRect& rcDest, wxRect& rcSrc);
		bool GetDrawRectForRight(const wxRect& rcRect, wxRect& rcDest, wxRect& rcSrc);
		bool GetDrawRectForLeftBottom(const wxRect& rcRect, wxRect& rcDest, wxRect& rcSrc);
		bool GetDrawRectForBottom(const wxRect& rcRect, wxRect& rcDest, wxRect& rcSrc);
		bool GetDrawRectForRightBottom(const wxRect& rcRect, wxRect& rcDest, wxRect& rcSrc);

	private:
		typedef std::pair<uint32_t, uint32_t> BufSize;
		typedef std::map<BufSize, wxBitmap> CacheBufMap;

	private:
		PicturePtr				m_imagePtr;						//!< 背景使用的图片
		wxRect					m_rcRect;						//!< 图片m_pPicture被使用的区域
		wxSize					m_szLeftTop;					//!< 指示九宫格画法的左边界和上边界的尺寸
		wxSize					m_szRightBottom;				//!< 指示九宫格画法的右边界和下边界的尺寸
		bool					m_bCanCache{ false };
		CacheBufMap				m_cacheBufs;
	};
}

