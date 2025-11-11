#include "Picture.h"
#include "util.h"
#include "logger.h"

namespace why
{
	Picture::Picture()
	{

	}

	Picture::~Picture()
	{

	}

	uint32_t Picture::Width()
	{ 
		return m_image.GetWidth();
	}

	/*!
	* 获取图片高度
	* @return 图片高度
	*/
	uint32_t Picture::Height()
	{
		return m_image.GetHeight();
	}

	bool Picture::CreatePicture(const wxString& strFileName)
	{
		m_strFileName = strFileName;
		m_image.LoadFile(strFileName);
		return m_image.IsOk();
	}

	bool Picture::CreatePicture(const wxBitmap &bitmap)
	{
		m_image = bitmap.ConvertToImage();
		return m_image.IsOk();
	}

	bool Picture::CreateGrayPicture(uint32_t uWidth, uint32_t uHeight, const uint8_t* pData)
	{
		uint8_t			*pDataPixel = nullptr;
		uint32_t		uInputIndex = 0;
		uint32_t		uOutputIndex = 0;
		uint32_t		uDesFrameWidth = ((uWidth + 3) / 4) * 4;

		m_image = wxImage(uWidth, uHeight);
		pDataPixel = m_image.GetData();

		for (uint32_t y = 0; y < uHeight; y++)
		{
			for (uint32_t x = 0; x < uWidth; x++)
			{
				uInputIndex = y * uDesFrameWidth + x;
				uOutputIndex = (y * uWidth + x) * 3;
				pDataPixel[uOutputIndex] = pData[uInputIndex];
				pDataPixel[uOutputIndex + 1] = pData[uInputIndex];
				pDataPixel[uOutputIndex + 2] = pData[uInputIndex];
			}
		}

		return true;
	}
	
	bool Picture::CreateRGBPicture(uint32_t uWidth, uint32_t uHeight, const uint8_t* pData)
	{
		uint8_t			*pDataPixel = nullptr;
		uint32_t		uInputIndex = 0;
		uint32_t		uOutputIndex = 0;
		uint32_t		uDesFrameWidth = ((uWidth + 3) / 4) * 4;

		m_image = wxImage(uWidth, uHeight);
		pDataPixel = m_image.GetData();

		for (uint32_t y = 0; y < uHeight; y++)
		{
			for (uint32_t x = 0; x < uWidth; x++)
			{
				uInputIndex = (y * uDesFrameWidth + x) * 3;
				uOutputIndex = (y * uWidth + x) * 3;
				pDataPixel[uOutputIndex] = pData[uInputIndex];
				pDataPixel[uOutputIndex + 1] = pData[uInputIndex + 1];
				pDataPixel[uOutputIndex + 2] = pData[uInputIndex + 2];
			}
		}

		return true;
	}

	void Picture::DrawImage(wxDC& dc, const wxPoint& ptDst)
	{
		if (m_image.IsOk())
			DrawImage(dc, m_image, ptDst);
	}

	wxImage Picture::Scale(const wxRect &rt, int32_t width, int32_t height)
	{
		if (0 == width || 0 == height) return m_image.Copy();

		if (0 == rt.x && 0 == rt.y && rt.width == m_image.GetWidth() && rt.height == m_image.GetHeight()) 
			return Scale(width, height);

		wxImage			curImage = m_image.GetSubImage(rt);
		
		curImage.Rescale(width, height, wxIMAGE_QUALITY_HIGH);
		return curImage;
	}

	wxImage Picture::Scale(int32_t width, int32_t height)
	{
		if (0 == width || 0 == height) return m_image.Copy();

		if (width == m_image.GetWidth() && height == m_image.GetHeight())
			return m_image.Copy();

		auto iter = m_scaleChaches.find(std::pair(width, height));
		if (iter != m_scaleChaches.end())
			return iter->second;
		
		wxImage curImage = m_image.Copy();
		curImage.Rescale(width, height, wxIMAGE_QUALITY_HIGH);
		m_scaleChaches.emplace(std::pair(width, height), curImage);
		return curImage;
	}

	void Picture::DrawImage(wxDC& dc, const wxPoint& ptDst, const wxSize& szDst)
	{
		if (m_image.IsOk()) {
			DrawImage(dc, Scale(szDst.GetWidth(), szDst.GetHeight()), ptDst);
		}
	}

	void Picture::DrawImage(wxDC& dc, const wxPoint& ptDst, const wxSize& szDst, const double & rotate, const wxPoint& rotatePt)
	{
		if (m_image.IsOk())
		{
			wxImage curImage = Scale(szDst.GetWidth(), szDst.GetHeight());

			wxPoint offset;
			wxImage curImageRotate = curImage.Rotate(rotate, rotatePt, true, &offset);

			wxPoint ptDstRotate(ptDst.x + offset.x, ptDst.y + offset.y);

			DrawImage(dc, curImageRotate, ptDstRotate);
		}
	}

	void Picture::DrawImage(wxDC& dc, const wxPoint& ptDst, const wxPoint& ptSrc, const wxSize& szSrc)
	{
		if (m_image.IsOk())
		{
			wxRect rt(ptSrc.x, ptSrc.y, szSrc.GetWidth(), szSrc.GetHeight());
			DrawImage(dc, Scale(rt, szSrc.GetWidth(), szSrc.GetHeight()), ptDst);
		}
	}

	void Picture::DrawImage(wxDC& dc, const wxPoint& ptDst, const wxSize& szDst, const wxPoint& ptSrc, const wxSize& szSrc)
	{
		if (m_image.IsOk())
		{
			wxRect rt(ptSrc.x, ptSrc.y, szSrc.GetWidth(), szSrc.GetHeight());
			DrawImage(dc, Scale(rt, szDst.GetWidth(), szDst.GetHeight()), ptDst);
		}
	}

	void Picture::DrawImage(wxDC& dc, const wxImage& image, const wxPoint& ptPos)
	{
		wxBitmap			curBitmap(image);

		dc.DrawBitmap(curBitmap, ptPos, true);
	}

	void Picture::DrawImage(wxGraphicsContext* pGraphic, const wxImage& image, const wxPoint& ptPos)
	{
		wxBitmap			curBitmap(image);

		pGraphic->DrawBitmap(curBitmap, ptPos.x, ptPos.y, image.GetWidth(), image.GetHeight());
	}

	void Picture::DrawImage(wxDC& dc, const wxRect& rcDst, const wxRect& rcSrc)
	{
		if (m_image.IsOk())
		{
			wxRect rt(rcSrc.x, rcSrc.y, rcSrc.width, rcSrc.height);
			DrawImage(dc, Scale(rt, rcDst.GetWidth(), rcDst.GetHeight()), rcDst.GetPosition());
		}
	}

	void Picture::DrawImage(wxGraphicsContext* pGraphic, const wxRect& rcDst)
	{
		if (m_image.IsOk()) {
			DrawImage(pGraphic, Scale(rcDst.width, rcDst.height), rcDst.GetPosition());
		}
	}

	void Picture::DrawImage(wxGraphicsContext* pGraphic, const wxRect& rcDst, const wxRect& rcSrc)
	{
		if (m_image.IsOk())
		{
			wxRect rt(rcSrc.x, rcSrc.y, rcSrc.width, rcSrc.height);
			DrawImage(pGraphic, Scale(rt, rcDst.GetWidth(), rcDst.GetHeight()), rcDst.GetPosition());
		}
	}

	void Picture::DrawImage(wxGraphicsContext *pGraphics, const wxPoint& ptDst, const wxSize& szDst,
		const double & rotate, const wxPoint& rotatePt)
	{
		if (m_image.IsOk())
		{
			wxImage curImage = Scale(szDst.GetWidth(), szDst.GetHeight());

			wxPoint offset;
			wxImage curImageRotate = curImage.Rotate(rotate, rotatePt, true, &offset);

			wxPoint ptDstRotate(ptDst.x + offset.x, ptDst.y + offset.y);

			DrawImage(pGraphics, curImageRotate, ptDstRotate);
		}
	}

	wxColour Picture::GetPixel(const wxPoint& ptPos)
	{
		wxColour			curColour;
		auto				uRed = m_image.GetRed(ptPos.x, ptPos.y);
		auto				uGreen = m_image.GetGreen(ptPos.x, ptPos.y);
		auto				uBlue = m_image.GetBlue(ptPos.x, ptPos.y);

		curColour.Set(uRed, uGreen, uBlue);
		return curColour;
	}

	bool Picture::SaveToFile(const wxString &strFileName)
	{
		if (!m_image.IsOk())
			return false;

		return m_image.SaveFile(strFileName);
	}

}
