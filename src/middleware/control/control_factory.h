#pragma once

#include "gui_import.h"
#include "virtual_wnd.h"
#include "virtual_wnd_owner.h"

namespace why
{
	ControlType GetControlType(const std::string& strControlType);
	VirtualWndType GetVirtualWndType(const std::string& strVirtualWndType);
	wxWindow* CreateControl(ControlType eControlType, wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle);
	VirtualWnd* CreateVirtualWnd(VirtualWndType eType, VirtualWndOwner* pOwner, const wxRect& rcItem, const wxString& strName, const std::string& strStyle);
}