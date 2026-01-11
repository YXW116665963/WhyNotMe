#pragma once

#include "gui_import.h"
#include "xml_util.h"
#include "resource_loader.h"
#include "data_exchange.h"
#include "panel_graphic.h"
#include "localization_manager.h"
#include "ui_def.h"
#include <string>

#include "virtual_wnd_owner.h"
#include "common_control_loader.h"
namespace why
{
	void SetLocalizationManager(LocalizationManager* pLocalizationMgr);
	void SetDataExceedLimitNotify(uint32_t uLowLimitCode, uint32_t uHighLimitCode, const OnDataExceedLimit& fnDataExceedLimitNotify);
	void SetUIResourcePath(const char* pConfigPath, const char* pPicturePath, const char* pResourceTargetPath);
	void LoadCommonControl(wxWindow* pParent, const WndCoordinate& wndCoordinate, DataExchange* pDataExchange, PanelGraphic* pPanelGraphic, CXmlNode* pNode, ControlType eType);
	void LoadWindow(wxWindow* pParent, const WndCoordinate& wndCoordinate, DataExchange* pDataExchange, PanelGraphic* pPanelGraphic, CXmlNode* pNode);
	bool LoadPanel(wxWindow* pParent, DataExchange *pDataExchange, const std::string& strFileName, PanelGraphic* pPanelGraphic);
	bool LoadVirtualWnd(VirtualWndOwner* pOwner, const std::string& strFileName);
}

