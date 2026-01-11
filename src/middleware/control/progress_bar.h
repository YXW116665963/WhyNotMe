#pragma once

#include <wx/wx.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include "background.h"

namespace why
{
	class ProgressBar : public wxWindow
	{
	public:
		ProgressBar(
			wxWindow* parent,
			wxWindowID id = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = 0,
			const wxString& strName = "");

		void SetProgress(int progress);
		void SetBackgroundAndForeground(BackgroundPtr& pPngBack, BackgroundPtr& pPngFore);

	protected:
		void OnPaint(wxPaintEvent& event);

	private:
		int m_progress;
		BackgroundPtr		pPngBackground_;
		BackgroundPtr		pPngForeground_;
	};
}