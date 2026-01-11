#pragma once

#include "picture.h"
#include "control_res.h"
#include "custom_widgets.h"
#include <memory>
#include <map>

namespace why
{
	class ImageSwitchButton : public wxWindow
	{
	public:
		ImageSwitchButton(wxWindow* parent, wxWindowID id, const wxString& label,
			const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
			long style = 0, const wxString& name = wxASCII_STR(wxButtonNameStr));
		~ImageSwitchButton();

		wxDECLARE_CLASS(ImageSwitchButton);

	public:
		//implement wxWindow
		void SetLabel(const wxString& label) override;
		wxString GetLabel() const override { return m_strTitle; }

		void SetControlRes(const std::string& strUncheckRes, const std::string& strCheckRes);
		void SetCheck(bool bCheck);
		bool IsChecked();
		
		uint32_t GetUserData(bool bCheck);
		void SetUserData(bool bCheck, uint32_t uUserData);
		bool MatchUserData(uint32_t uUserData);

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
		void OnLButtonDClick(wxMouseEvent& event);

	private:
		void DrawCheckbox(wxDC &dc, const wxRect& rcItem);
		
		ControlStatus GetCurrentStatus();
		void DrawBackground(wxDC& dc, const wxRect& rcItem);

		bool ProcessMsgHandledFlag(const wxPoint& ptPos);

	private:
		wxRect							m_rcClient;
		IControlRes*					m_pUnCheckRes{ nullptr };
		IControlRes*					m_pCheckRes{ nullptr };
		ControlStatus					m_eState;
		bool							m_bMouseDown;
		wxBitmap						m_bufferBitmap;
		wxColor							m_crBackground;
		wxBrush							m_brBKBrush;
		wxString						m_strTitle;
		bool							m_bChecked{ false };
		uint32_t						m_userData[2]{ 0, 1 };
	};

	typedef std::unique_ptr<ImageSwitchButton> ImageSwitchButtonPtr;
}
