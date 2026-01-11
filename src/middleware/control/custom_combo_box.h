#pragma once

#include "picture.h"
#include "control_res.h"
#include "custom_widgets.h"
#include "popu_listbox.h"

#include <utility>
#include <vector>
#include "data_exchange.h"

namespace why
{
	class CustomComboBox : public wxWindow
	{
	public:
		CustomComboBox(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style = 0, const wxString& name = wxASCII_STR(wxControlNameStr));
		~CustomComboBox();

		wxDECLARE_CLASS(CustomComboBox);

	public:
		int32_t GetSelection() const { return m_nSelected; }
		void SetSelection(int32_t nSelected);
		void SetTypeDateTime() { m_bDateTime = true; };

		uint32_t GetCurrentData() const;
		wxString GetCurrentText() const;
		wxString GetTextByUserData(uint32_t uUserData) const;
		std::vector<wxString> GetAllText() const;
		void SetSelectionByUserData(uint32_t uUserData);

		int32_t Append(const wxString& item, uint32_t uUserData);
		void ClearOption(const wxString& item);
		void ClearOptions();
		int32_t GetOptionsSize();

		void SetBackground(BackgroundPtr& backgroundPtr);
		void SetButtonRes(const std::string& strControlRes, uint32_t uButtonSize);
		void SetButtonIcon(PicturePtr &iconPtr, const wxSize &szIcon);

		void SetDateTime(wxDateTime time);
		wxDateTime GetDateTime() { return m_dateTime; }

	private:
		bool ProcessEvent(wxEvent& event) override;
		void OnPaint(wxPaintEvent& event);
		void OnEraseBackground(wxEraseEvent& event);
		void OnSize(wxSizeEvent& event);
		void OnLButtonDown(wxMouseEvent& event);
		void OnLButtonUp(wxMouseEvent& event);
		void OnEnterWindow(wxMouseEvent& event);
		void OnLeaveWindow(wxMouseEvent& event);

	private:
		void GetButtonRect(wxRect &rcButton);
		void OnDraw(wxDC& dc);
		void DrawBackground(wxDC& dc);
		void DrawText(wxDC& dc, const wxRect& rcEdit);
		void DrawButton(wxDC& dc, const wxRect& rcButton);
		void OnButtonClick();
		
	private:
		void ShowListBox();
		void ShowDateBox();

		void OnMouseEnter(wxMouseEvent& event);
		void OnMouseLeave(wxMouseEvent& event);
		void OnMouseMove(wxMouseEvent& event);

	private:
		typedef std::vector<std::pair<wxString, uint32_t>> Options;

	private:
		wxBitmap						m_bufferBitmap;
		wxRect							m_rcClient;
		BackgroundPtr					m_backgroundPtr;
		PicturePtr						m_iconPtr;
		wxSize							m_szIcon;
		IControlRes*					m_pButtonRes{ nullptr };
		uint32_t						m_uButtonSize{ 0 };
		bool							m_bMouseDown{ false };
		ControlStatus					m_eState{ ControlStatus::e_state_normal };
		Options							m_options;
		wxColor							m_crBackground;
		wxBrush							m_brBKBrush;
		int32_t							m_nSelected{ -1 };
		bool                            m_bDateTime{ false };
		wxDateTime                      m_dateTime;

	private:
		typedef std::unique_ptr<DataExchange> DataExchangePtr;

	private:
		DataExchangePtr				m_dataExchangePtr;
	};
}
