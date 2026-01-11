#pragma once

#include "standard.h"
#include "resource_manager.h"
#include "panel_graphic_element_interface.h"

namespace why
{
	class LineGraphic : public IPanelGraphicElement
	{
	public:
		LineGraphic(PenPtr& penPtr, const wxPoint& ptStart, const wxPoint& ptEnd, const std::string& strID);
		~LineGraphic() = default;

	public:
		const std::string& ID() const override { return m_strID; }
		bool IsShow() const override { return m_bShow; }
		void Show(bool bShow) override { m_bShow = bShow; }
		void Draw(wxDC& dc) override;
		void UpdateRect(const wxRect& rcRect) override {
			m_ptStart = wxPoint(rcRect.GetLeft(), rcRect.GetTop());
			m_ptEnd = wxPoint(rcRect.GetRight(), rcRect.GetBottom());
		}
		void OffsetRect(const wxRect& rcRect) override {
			m_ptStart.x += rcRect.GetLeft();
			m_ptStart.y += rcRect.GetTop();
			m_ptEnd.x += rcRect.GetRight();
			m_ptEnd.y += rcRect.GetBottom();
		}
		wxRect GetRect() override { return wxRect(m_ptStart, m_ptEnd); }
		void Expand(const wxRect& rcExpand, bool bExpandH, bool bExpandV) override;
		void Contract(const wxRect& rcContract, bool bContractH, bool bContractV) override;

	private:
		PenPtr					m_penPtr;
		wxPoint					m_ptStart;
		wxPoint					m_ptEnd;
		std::string				m_strID;
		bool					m_bShow{ true };
	};

	class PictureGraphic : public IPanelGraphicElement
	{
	public:
		PictureGraphic(PicturePtr& picturePtr, const wxRect& rcRect, const std::string& strID);
		~PictureGraphic() = default;

	public:
		const std::string& ID() const override { return m_strID; }
		bool IsShow() const override { return m_bShow; }
		void Show(bool bShow) override { m_bShow = bShow; }
		void Draw(wxDC& dc) override;
		void UpdateRect(const wxRect& rcRect) override { m_rcRect = rcRect; }
		void OffsetRect(const wxRect& rcRect) {
			m_rcRect.SetX(m_rcRect.GetX() + rcRect.GetX());
			m_rcRect.SetY(m_rcRect.GetY() + rcRect.GetY());
			m_rcRect.SetWidth(m_rcRect.GetWidth() + rcRect.GetWidth());
			m_rcRect.SetHeight(m_rcRect.GetHeight() + rcRect.GetHeight());
		}
		wxRect GetRect() override { return m_rcRect; }
		void Expand(const wxRect& rcExpand, bool bExpandH, bool bExpandV) override;
		void Contract(const wxRect& rcContract, bool bContractH, bool bContractV) override;

	private:
		PicturePtr				m_picturePtr;
		wxRect					m_rcRect;
		std::string				m_strID;
		bool					m_bShow{ true };
	};

	class BackgroudGraphic : public IPanelGraphicElement
	{
	public:
		BackgroudGraphic(BackgroundPtr& backgroundPtr, const wxRect& rcRect, const std::string& strID);
		~BackgroudGraphic() = default;

	public:
		const std::string& ID() const override { return m_strID; }
		bool IsShow() const override { return m_bShow; }
		void Show(bool bShow) override { m_bShow = bShow; }
		void Draw(wxDC& dc) override;
		void UpdateRect(const wxRect& rcRect) { m_rcRect = rcRect; }
		void OffsetRect(const wxRect& rcRect) {
			m_rcRect.SetX(m_rcRect.GetX() + rcRect.GetX());
			m_rcRect.SetY(m_rcRect.GetY() + rcRect.GetY());
			m_rcRect.SetWidth(m_rcRect.GetWidth() + rcRect.GetWidth());
			m_rcRect.SetHeight(m_rcRect.GetHeight() + rcRect.GetHeight());
		}
		void ChangeBackground(BackgroundPtr& backgroundPtr) { m_backgroundPtr = backgroundPtr; }
		wxRect GetRect() override { return m_rcRect; }
		void Expand(const wxRect& rcExpand, bool bExpandH, bool bExpandV) override;
		void Contract(const wxRect& rcContract, bool bContractH, bool bContractV) override;

	private:
		BackgroundPtr			m_backgroundPtr;
		wxRect					m_rcRect;
		std::string				m_strID;
		bool					m_bShow{ true };
	};

	class TextGraphic : public IPanelGraphicElement
	{
	public:
		TextGraphic(FontPtr& fontPtr, const wxColor& rcColor, const wxString& strText, const wxRect& rcRect, const std::string& strID, uint32_t& uStyle);
		~TextGraphic() = default;

	public:
		const std::string& ID() const override { return m_strID; }
		bool IsShow() const override { return m_bShow; }
		void Show(bool bShow) override { m_bShow = bShow; }
		void Draw(wxDC& dc) override;
		void UpdateRect(const wxRect& rcRect) { m_rcRect = rcRect; }
		void OffsetRect(const wxRect& rcRect) {
			m_rcRect.SetX(m_rcRect.GetX() + rcRect.GetX());
			m_rcRect.SetY(m_rcRect.GetY() + rcRect.GetY());
			m_rcRect.SetWidth(m_rcRect.GetWidth() + rcRect.GetWidth());
			m_rcRect.SetHeight(m_rcRect.GetHeight() + rcRect.GetHeight());
		}
		wxRect GetRect() override { return m_rcRect; }
		void Expand(const wxRect& rcExpand, bool bExpandH, bool bExpandV) override;
		void Contract(const wxRect& rcContract, bool bContractH, bool bContractV) override;

	private:
		FontPtr					m_fontPtr;
		wxString				m_strText;
		wxColor					m_rcColor;
		wxRect					m_rcRect;
		std::string				m_strID;
		uint32_t				m_uStyle;
		bool					m_bShow{ true };
	};
}
