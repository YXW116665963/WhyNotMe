#pragma once

#include "gui_import.h"
#include <string>

#include "xml_util.h"
#include "resource_loader.h"
#include "data_exchange.h"
#include "image_button.h"
#include "image_label.h"
#include "panel_graphic.h"
#include "data_exchange.h"
#include "progress_bar.h"
#include "virtual_wnd.h"
#include "virtual_wnd_owner.h"
#include "ui_struct_def.h"

namespace why
{
	struct WndCoordinate
	{
		wxPoint		m_ptOrigin;
		wxSize		m_szOrigin;
		float64_t	m_fScaleX{ 1.0 };
		float64_t	m_fScaleY{ 1.0 };
	};
	
	void SetDefaultEditBackground(const std::string &strNormal, const std::string &strReadonly);
	bool LoadWindowRect(const WndCoordinate& wndCoordinate, CXmlNode* pNode, wxRect& rcRect);
	bool LoadWindowRect(const wxRect& rcRoot, CXmlNode* pNode, wxRect& rcRect);
	bool LoadWindowRect(wxWindow* pParent, CXmlNode* pNode, wxRect& rcWindow);
	bool LoadCellRect(CXmlNode* pNode, float64_t& fColumnPos, float64_t& fLinePos, float64_t& fWidth, float64_t& fHeight);
	bool LoadCellPos(CXmlNode* pNode, int32_t& nColumnPos, int32_t& nLinePos);
	void LoadExtraInfo(wxWindow* pControl, ControlType& eType, PanelGraphic* pPanelGraphic, CXmlNode* pNode);
	void BindData(wxWindow* pControl, DataExchange *pDataExchange, CXmlNode* pNode);
	void BindDynamicData(BaseBindDataInfo& dataInfo, DataExchange *pDataExchange, CXmlNode* pNode);
	void LoadVirtualWndInfo(VirtualWnd* pVirtualWnd, const WndCoordinate& wndCoordinate, VirtualWndType& eType, CXmlNode* pNode);
}
