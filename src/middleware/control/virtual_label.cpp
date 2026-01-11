#include "virtual_label.h"
#include "singleton.h"
#include "resource_manager.h"
#include "virtual_wnd_owner.h"

namespace why
{
	VirtualLabel::VirtualLabel(VirtualWndOwner* parent, const wxString& label, const wxRect& rcRect, const wxString& name)
		: VirtualWnd(parent, label, rcRect, name)
	{

	}

	void VirtualLabel::OnPaint(wxGraphicsContext* pGraphic)
	{
		wxString				strTitle = GetLabel();

		VirtualWnd::OnPaint(pGraphic);

		if (!strTitle.IsEmpty())
		{
			wxRect					rcClient = GetRect();
			wxFont					ftDefault = GetFont();
			wxColour				crColor = GetForegroundColour();

			pGraphic->SetFont(ftDefault, crColor);
			pGraphic->DrawText(strTitle, rcClient.x, rcClient.y);
		}
	}
}