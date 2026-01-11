#pragma once

#include "picture.h"
#include "control_res.h"
#include "custom_widgets.h"
#include <memory>
#include <map>

namespace why
{
	enum class ButtonStyle
	{
		transparent = 0x00010000L,			//!< 透明窗口，当窗口背景位指定背景时不响应鼠标
		only_text = 0x00020000L,			//!< 仅显示文本
		timer_repeat = 0x00040000L,			//!< 定时按钮
	};

	class ImageButton : public wxWindow
	{
	public:
		ImageButton(wxWindow* parent, wxWindowID id, const wxString& label,
			const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
			long style = 0, const wxString& name = wxASCII_STR(wxButtonNameStr));
		~ImageButton();

		wxDECLARE_CLASS(ImageButton);

	public:
		//implement wxWindow
		void SetLabel(const wxString& label) override;
		wxString GetLabel() const override { return m_strTitle; }
		bool Show(bool show = true) override;

		uint32_t GetButtonSytle() { return m_uStyle; }
		void SetButtonStyle(uint32_t dwStyle);
		void SetControlRes(const std::string& strControlRes);
		void SetSelectIconName(const std::string& strName);

		void ShowSelect(bool show);
		bool IsSelect();

		CustomWidgets& GetCustomWidgets() { return m_customWidgets; }
		void SetCleanStatus(bool bCleanStatus);

	private:
		bool ProcessEvent(wxEvent& event) override;

	private:
		void OnPaint(wxPaintEvent& event);
		void OnEraseBackground(wxEraseEvent& event);
		void OnSize(wxSizeEvent& event);
		void OnLButtonDown(wxMouseEvent& event);
		void OnLButtonUp(wxMouseEvent& event);
		void OnEnterWindow(wxMouseEvent& event);
		void OnMouseMove(wxMouseEvent& event);
		void OnLeaveWindow(wxMouseEvent& event);
		void OnCommandRepeat(wxTimerEvent& event);
		void OnLButtonDClick(wxMouseEvent& event);

	private:
		void DrawButton(wxDC& dc, const wxRect& rcItem);

		ControlStatus GetCurrentStatus();
		void DrawBackground(wxDC& dc, const wxRect& rcItem);

		void DrawText(wxDC& dc);
		bool ProcessMsgHandledFlag(const wxPoint& ptPos);

	private:
		wxRect							m_rcClient;
		uint32_t						m_uStyle{ 0 };
		IControlRes* m_pControlRes{ nullptr };
		ControlStatus					m_eState;
		bool							m_bMouseDown;
		wxBitmap						m_bufferBitmap;
		CustomWidgets					m_customWidgets;
		wxColor							m_crBackground;
		wxBrush							m_brBKBrush;
		wxString						m_strTitle;
		std::unique_ptr<wxTimer>		m_timerPtr;
		uint64_t						m_uRepeatTimer{ 0 };
		uint64_t						m_uLastStamp{ 0 };
		std::string                     m_strSelectIconName;
		bool							m_bCleanStatus{ false };

	};

	typedef std::unique_ptr<ImageButton> ImageButtonPtr;
}
