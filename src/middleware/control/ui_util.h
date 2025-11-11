#pragma once

#include "standard.h"
#include "gui_import.h"

namespace why
{
	enum class TextAlignFormat
	{
		left = 0x00000,
		center = 0x0001,
		right = 0x0002,
		top = 0x0000,
		v_center = 0x0010,
		bottom = 0x0020
	};

	bool PtInRect(const wxPoint& ptPoint, const wxRect& rcRect);
	void DrawText(wxDC &dc, const wxString &strText, const wxRect &rcText, wxFont &ftFont, uint32_t uColor, uint32_t uAlignFormat);
	void DrawRectBorder(wxDC &dc, wxPen &pnBorder, const wxRect &rcRect);
	void GetCurrentScreenInfo(wxRect& rcScreen);
}
