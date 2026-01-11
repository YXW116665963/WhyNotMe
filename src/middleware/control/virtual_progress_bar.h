#pragma once

#include "virtual_wnd.h"

namespace why
{
	class VirtualProgressBar : public VirtualWnd
	{
	public:
		VirtualProgressBar(VirtualWndOwner* parent, const wxString& label, const wxRect& rcRect, const wxString& name);
		~VirtualProgressBar() = default;

	public:
		void SetDirection(bool bHorizontal);
		void SetClickRange(bool bOnlyBackgroundClicked);
		void AddBackground(BackgroundPtr &normalPtr, BackgroundPtr &selectedPtr, int32_t nHeight);
		void AddSlider(PicturePtr& sliderPtr);
		void GetRange(int32_t &nMin, int32_t &nMax) const;
		void SetRange(int32_t nMin, int32_t nMax);
		int32_t GetPos() const;
		void SetPos(int32_t nPos);
		
	public:
		bool AcceptMouseEvent() override { return true; }
		void OnPaint(wxGraphicsContext* pGraphic) override;
		bool OnLButtonDown(wxMouseEvent& event) override;
		void OnLButtonUp(wxMouseEvent& event) override;
		void OnMouseMove(wxMouseEvent& event) override;
		void OnEnterWindow(wxMouseEvent& event) override;
		void OnLeaveWindow(wxMouseEvent& event) override;

	private:
		void OnPaintHorizontal(wxGraphicsContext* pGraphic, const wxRect &rcClient);
		void OnPaintVertical(wxGraphicsContext* pGraphic, const wxRect& rcClient);
		void GetProgressBarRect(wxRect& rcBar);
		int32_t CalculatePos(const wxPoint &ptPos);
		void SetPosition(int32_t nPos, bool bNotify);

	private:
		bool						m_bHorizontal{ true };
		BackgroundPtr				m_normalPtr;
		BackgroundPtr				m_selectedPtr;
		PicturePtr					m_sliderPtr;
		wxRect						m_rcClient;
		wxRect						m_rcSlider;
		int32_t						m_nHeight{ 6 };
		int32_t						m_nMin{ 0 };
		int32_t						m_nMax{ 100 };
		int32_t						m_nPos{ 0 };
		bool						m_bMouseDown{ false };
		bool						m_bOnlyBackgroundClicked{ true };
	};
}