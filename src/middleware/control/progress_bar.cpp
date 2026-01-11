#include "progress_bar.h"

namespace why
{
	ProgressBar::ProgressBar(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& strName)
		: wxWindow(parent, id, pos, size, style, strName),
		m_progress(0)
	{
		Bind(wxEVT_PAINT, &ProgressBar::OnPaint, this);
		Bind(wxEVT_ERASE_BACKGROUND, [](wxEraseEvent& event) {event.Skip(false); });
	}

	void ProgressBar::SetProgress(int progress)
	{
		if (0 <= progress && progress <= 100)
		{
			m_progress = progress;
			Refresh(); // 请求重绘
		}
	}

	void ProgressBar::SetBackgroundAndForeground(BackgroundPtr& pPngBack, BackgroundPtr& pPngFore)
	{
		pPngBackground_ = pPngBack;
		pPngForeground_ = pPngFore;
		Refresh();
	}

	void ProgressBar::OnPaint(wxPaintEvent& WXUNUSED(event))
	{
		wxBufferedPaintDC	dc(this);
		wxRect rcClient = GetClientRect();

		// 绘制背景
		if (pPngBackground_)
			pPngBackground_->Draw(dc, GetClientRect());

		// 绘制进度条
		if (pPngForeground_)
		{
			// 计算进度条尺寸
			int barWidth = GetClientSize().GetWidth() * m_progress / 100;
			int barHeight = GetClientSize().GetHeight();

			pPngForeground_->Draw(dc, wxRect(0, 0, barWidth, barHeight));
		}

		// 在进度条中间绘制文字
		wxString progressText = wxString::Format("%d%%", m_progress);
		wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
		wxSize textSize = dc.GetTextExtent(progressText);
		wxPoint textPos(
			(GetClientSize().GetWidth() - textSize.GetWidth()) / 2,
			(GetClientSize().GetHeight() - textSize.GetHeight()) / 2);
		dc.SetTextForeground(wxColour(*wxBLACK));
		dc.DrawText(progressText, textPos);
	}
}