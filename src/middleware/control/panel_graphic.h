#pragma once

#include "standard.h"
#include "gui_import.h"

#include "panel_graphic_element_interface.h"
#include "picture.h"

#include <list>
#include <set>

namespace why
{

	/**
	* @author	jca
	* @brief	PanelGraphic ->
	*			IPanelGraphicElement ->
	*			IBackGroundGraphic ->
	*			IBackGround、IPicture、ILine、IText
	*/
	class PanelGraphic
	{
	public:
		PanelGraphic();
		~PanelGraphic();

	public:
		IPanelGraphicElement* AddLine(const std::string& strResName, const wxPoint& ptStart, const wxPoint& ptEnd, std::string id = "");
		void ChangeLineRect(const std::string& id, const wxPoint& ptStart, const wxPoint& ptEnd);
		void OffsetLineRect(const std::vector<std::string>& id, const wxPoint& ptStart, const wxPoint& ptEnd);

		IPanelGraphicElement* AddPicture(const std::string& strResName, const wxRect& rcRect, std::string id = "");
		void ChangePictureRect(const std::string& id, const wxRect& rcRect);
		void OffsetPictureRect(const std::vector<std::string>& id, const wxRect& rcRect);

		IPanelGraphicElement* AddText(const std::string& strResName, const wxColor& crColor, const wxString& strText, const wxRect& rcRect, std::string id = "", uint32_t style = wxTE_LEFT);
		void ChangeTextRect(const std::string& id, const wxRect& rcRect);
		void OffsetTextRect(const std::vector<std::string>& id, const wxRect& rcRect);

		IPanelGraphicElement* AddBackground(const std::string& strResName, const wxRect& rcRect, std::string id = "");
		void ChangeBackgroundRect(const std::string& id, const wxRect& rcRect);
		void OffsetBackgroundRect(const std::vector<std::string>& id, const wxRect& rcRect);
		void ChangeBackground(const std::string& id, const std::string& strResName);

		uint32_t ElementCount();
		void ShowElement(const std::string& elementID, bool bShow);
		void ShowElements(const std::vector<std::string>& elementIDs, bool bShow);

		void Refresh() { m_bNeedDraw = true; }
		bool IsNeedDraw() { return m_bNeedDraw; }
		void DrawGraphic(wxDC& dc);

		void Expand(const wxRect& rcExpand, bool bExpandH, bool bExpandV);
		void Contract(const wxRect& rcContract, bool bContracH, bool bContracV);

		void HideAndOffsetElements(const std::string& strIncludeElement, bool bIsVertical);
		void ShowIncludeAreaElements(const std::string& strIncludeName, bool bShow);
		void OffsetIncludeAreaElement(const std::string& strIncludeName, wxRect rect);

	private:
		typedef std::unique_ptr<IPanelGraphicElement> GraphicElementPtr;
		typedef std::list<GraphicElementPtr> GraphicElementList;

	private:
		GraphicElementList			m_elementList;
		bool						m_bNeedDraw{ true };
	};
}
