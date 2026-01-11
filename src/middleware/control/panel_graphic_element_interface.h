#pragma once

#include "standard.h"
#include "gui_import.h"

namespace why
{
	class IPanelGraphicElement
	{
	public:
		virtual ~IPanelGraphicElement() = default;

	public:
		virtual const std::string& ID() const = 0;
		virtual bool IsShow() const = 0;
		virtual void Show(bool bShow) = 0;
		virtual void Draw(wxDC& dc) = 0;
		virtual wxRect GetRect() = 0;
		virtual void UpdateRect(const wxRect& rcRect) = 0;
		virtual void OffsetRect(const wxRect& rcRect) = 0;
		virtual void Expand(const wxRect& rcExpand, bool bExpandH, bool bExpandV) = 0;
		virtual void Contract(const wxRect& rcExpand, bool bExpandH, bool bExpandV) = 0;
	};
}
