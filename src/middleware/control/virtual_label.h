#pragma once

#include "virtual_wnd.h"

namespace why
{
	class VirtualLabel : public VirtualWnd
	{
	public:
		VirtualLabel(VirtualWndOwner* parent, const wxString& label, const wxRect& rcRect, const wxString& name);
		~VirtualLabel() = default;

	public:
		//IVirtualWnd interface
		void OnPaint(wxGraphicsContext* pGraphic) override;
	};
}