#include "image_label.h"
#include "singleton.h"
#include "resource_manager.h"
#define PDDING_LEFT_10        10

namespace why
{
	wxIMPLEMENT_CLASS(ImageLabel, wxWindow);

	ImageLabel::ImageLabel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
		: wxWindow(parent, id, pos, size, style, name)
		, m_customWidgets(this)
	{
		m_crBackground = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
		m_brBKBrush = wxBrush(m_crBackground);

		Bind(wxEVT_PAINT, &ImageLabel::OnPaint, this);
		Bind(wxEVT_ERASE_BACKGROUND, &ImageLabel::OnEraseBackground, this);

		Bind(wxEVT_ENTER_WINDOW, &ImageLabel::OnMouseEnter, this);
		Bind(wxEVT_LEAVE_WINDOW, &ImageLabel::OnMouseLeave, this);
		Bind(wxEVT_MOTION, &ImageLabel::OnMouseMove, this);
	}

	ImageLabel::~ImageLabel()
	{

	}

	void ImageLabel::OnMouseEnter(wxMouseEvent& event)
	{
		if (!m_strTitle.empty())
		{
			SetToolTip(m_strTitle);
		}
		event.Skip();
	}

	void ImageLabel::OnMouseLeave(wxMouseEvent& event)
	{
		UnsetToolTip();
		event.Skip();
	}

	void ImageLabel::OnMouseMove(wxMouseEvent& event)
	{
		event.Skip();
	}

	void ImageLabel::SetLabel(const wxString& label)
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

	void ImageLabel::SetBackground(BackgroundPtr& backgroundPtr)
	{
		m_backgroundPtr = backgroundPtr;
		Refresh();
	}

	void ImageLabel::OnPaint(wxPaintEvent& event)
	{
		if (!m_bufferBitmap.IsOk() || m_bufferBitmap.GetSize() != GetClientSize())
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
			memDC.SetBackground(*wxTRANSPARENT_BRUSH);
			memDC.Clear();
			DrawLabel(memDC, rcClient);
			memDC.SelectObject(wxNullBitmap);
			dc.DrawBitmap(m_bufferBitmap, 0, 0, true);
		}
	}

	void ImageLabel::OnEraseBackground(wxEraseEvent& event)
	{
		event.Skip(false);
	}

	void ImageLabel::DrawLabel(wxDC& dc, const wxRect& rcItem)
	{
		DrawBackground(dc, rcItem);
		if (m_customWidgets.IsEmpty())
			DrawText(dc);
		else
			m_customWidgets.OnDraw(dc);
	}

	int32_t ImageLabel::GetTestLines(const wxString& strTitle, const FontPtr& ptr, int32_t width)
	{
		int32_t lines = 0;
		wxSize szSize;
		wxMemoryDC		memDC;

		memDC.SetFont(*ptr);

		wxString tempTitle, tempTest;
		for (auto iter = strTitle.begin(); iter != strTitle.end(); iter++)
		{
			tempTest = tempTitle + *iter;
			szSize = memDC.GetTextExtent(tempTest);
			if (szSize.x > width && !tempTitle.empty())
			{
				tempTitle.clear();
				lines++;
			}
			tempTitle += *iter;
		}

		if (!tempTitle.empty() || 0 == lines) {
			lines++;
		}

		return lines;
	}

	void ImageLabel::DrawBackground(wxDC& dc, const wxRect& rcItem)
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

		if (nullptr != m_backgroundPtr.get())
			m_backgroundPtr->Draw(dc, rcItem);
	}

	void ImageLabel::DrawText(wxDC& dc)
	{
		wxFont					ftDefault = GetFont();
		wxFont* pFont = &ftDefault;
		wxColour				crColor = GetForegroundColour();
		wxRect					rcText(wxPoint(0, 0), GetSize());
		wxSize					szText;
		wxPoint					ptPos;
		auto					lStyle = GetWindowStyleFlag();

		dc.SetFont(*pFont);
		dc.SetTextForeground(crColor);
		szText = dc.GetTextExtent(m_strTitle);

		if ((lStyle & wxALIGN_LEFT) == wxALIGN_LEFT)
		{
			ptPos.x = rcText.x + PDDING_LEFT_10;

		}
		else if ((lStyle & wxALIGN_RIGHT) == wxALIGN_RIGHT)
		{
			ptPos.x = rcText.x + (rcText.width - szText.GetWidth());
		}
		else
		{
			ptPos.x = rcText.x + (rcText.width - szText.x) / 2 + PDDING_LEFT_10;
		}
		ptPos.y = rcText.y + (rcText.height - szText.y) / 2;
		dc.DrawText(m_strTitle, ptPos);
	}
}