#pragma once

#include "virtual_wnd.h"

namespace why
{
	class VirtualButton : public VirtualWnd
	{
	public:
		VirtualButton(VirtualWndOwner* parent, const wxString& label, const wxRect& rcRect, const wxString& name);
		~VirtualButton() = default;

	public:
		//IVirtualWnd interface
		bool AcceptMouseEvent() override { return true; }
		void OnPaint(wxGraphicsContext* pGraphic) override;
		bool OnLButtonDown(wxMouseEvent& event) override;
		void OnLButtonUp(wxMouseEvent& event) override;
		void OnEnterWindow(wxMouseEvent& event) override;
		void OnLeaveWindow(wxMouseEvent& event) override;
		bool OnLButtonDClick(wxMouseEvent& event) override;
	public:
		void AddTextElement(const std::string& strName, FontPtr& fontPtr, wxColour& crText, const wxString& strText, const wxRect& rcText);
		void ChangeTextElement(const std::string& strName, const wxString& strText);
		void RemoveTextElement(const std::string& strName);
		void AddIconElement(const std::string& strName, PicturePtr& normalPtr, PicturePtr& hoverPtr, PicturePtr& pushedPtr, const wxRect& rcIcon);
		void ChangeIconElement(const std::string& strName, PicturePtr& normalPtr, PicturePtr& hoverPtr, PicturePtr& pushedPtr);
		void RemoveIconElement(const std::string& strName);

	private:
		struct CustomTextInfo
		{
			std::string		m_strName;
			FontPtr			m_fontPtr;
			wxColour		m_crText;
			wxString		m_strText;
			wxRect			m_rcText;
		};
		typedef std::unique_ptr<CustomTextInfo> CustomTextInfoPtr;
		typedef std::list<CustomTextInfoPtr> CustomTextList;
		typedef CustomTextList::iterator CustomTextIt;

		struct CustomIconInfo
		{
			std::string		m_strName;
			PicturePtr		m_normalPtr;
			PicturePtr		m_hoverPtr;
			PicturePtr		m_pushedPtr;
			wxRect			m_rcIcon;
		};
		typedef std::unique_ptr<CustomIconInfo> CustomIconInfoPtr;
		typedef std::list<CustomIconInfoPtr> CustomIconList;
		typedef CustomIconList::iterator CustomIconIt;

	private:
		void DrawIconElements(wxGraphicsContext* pGraphic);
		void DrawTextElements(wxGraphicsContext* pGraphic);
		void DrawText(wxGraphicsContext* pGraphic);
		Picture* GetPicture(CustomIconInfoPtr& iconInfoPtr, ControlStatus eStatus);

	private:
		CustomTextList					m_textElements;
		CustomIconList					m_iconElements;
		bool							m_bMouseDown{ false };
	};
}