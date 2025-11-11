#include "ui_util.h"

namespace why
{
	bool PtInRect(const wxPoint& ptPoint, const wxRect& rcRect)
	{
		if ((ptPoint.x < rcRect.x) || (ptPoint.x > (rcRect.x + rcRect.width)))
			return false;

		if ((ptPoint.y < rcRect.y) || (ptPoint.y > (rcRect.y + rcRect.height)))
			return false;

		return true;
	}

	void DrawText(wxDC &dc, const wxString &strText, const wxRect &rcText, wxFont &ftFont, uint32_t uColor, uint32_t uAlignFormat)
	{
		wxSize					szText;
		wxPoint					ptPos;

		dc.SetFont(ftFont);
		dc.SetTextForeground(uColor);
		szText = dc.GetTextExtent(strText);
		ptPos.x = rcText.x;
		
		if (uAlignFormat & static_cast<uint32_t>(TextAlignFormat::center))
			ptPos.x = rcText.x + (rcText.width - szText.x) / 2;

		if (uAlignFormat & static_cast<uint32_t>(TextAlignFormat::right))
			ptPos.x = rcText.x + (rcText.width - szText.x);

		ptPos.y = rcText.y;
		if (uAlignFormat & static_cast<uint32_t>(TextAlignFormat::v_center))
			ptPos.y = rcText.y + (rcText.height - szText.y) / 2;

		if (uAlignFormat & static_cast<uint32_t>(TextAlignFormat::bottom))
			ptPos.y = rcText.y + (rcText.height - szText.y);
		
		dc.DrawText(strText, ptPos);
	}

	void DrawRectBorder(wxDC &dc, wxPen &pnBorder, const wxRect &rcRect)
	{
		dc.SetPen(pnBorder);
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.DrawRectangle(rcRect);
	}

	void GetCurrentScreenInfo(wxRect& rcScreen)
	{
		wxDisplay display = wxDisplay(wxDisplay::GetFromWindow(wxTheApp->GetTopWindow()));
		if (display.IsOk())
		{
			rcScreen = display.GetGeometry();
		}
		else
		{
			rcScreen.x = 0;
			rcScreen.y = 0;
			rcScreen.width = 0;
			rcScreen.height = 0;
		}
	}
}