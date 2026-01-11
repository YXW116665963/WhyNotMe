#pragma once

#include "picture.h"
#include "control_res.h"
#include "custom_widgets.h"
#include <memory>
#include <map>

namespace why
{
	class ImageLabel : public wxWindow
	{
	public:
		ImageLabel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style = 0, const wxString& name = wxASCII_STR(wxControlNameStr));
		~ImageLabel();

		wxDECLARE_CLASS(ImageLabel);

	public:
		//implement wxWindow
		void SetLabel(const wxString& label) override;
		wxString GetLabel() const override { return m_strTitle; }

		void SetBackground(BackgroundPtr& backgroundPtr);
		CustomWidgets& GetCustomWidgets() { return m_customWidgets; }

		int32_t GetTestLines(const wxString& strTitle, const FontPtr& ptr, int32_t width);

	private:
		void OnPaint(wxPaintEvent& event);
		void OnEraseBackground(wxEraseEvent& event);
		void OnMouseEnter(wxMouseEvent& event);
		void OnMouseLeave(wxMouseEvent& event);
		void OnMouseMove(wxMouseEvent& event);

	private:
		void DrawLabel(wxDC& dc, const wxRect& rcItem);
		void DrawBackground(wxDC& dc, const wxRect& rcItem);
		void DrawText(wxDC& dc);

	private:
		PicturePtr					m_iconPtr;
		wxSize						m_szIcon;
		BackgroundPtr				m_backgroundPtr;
		wxBitmap					m_bufferBitmap;
		CustomWidgets				m_customWidgets;
		wxColor						m_crBackground;
		wxBrush						m_brBKBrush;
		wxString					m_strTitle;
	};
}
