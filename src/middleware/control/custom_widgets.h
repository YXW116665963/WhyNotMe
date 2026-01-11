#pragma once

#include "picture.h"
#include "control_res.h"
#include "custom_widgets_base.h"

#include <memory>
#include <map>

namespace why
{
	class CustomWidgets : public CustomWidgetsBase
	{
	public:
		CustomWidgets(wxWindow* pOwner);
		~CustomWidgets();

		void OnDraw(wxDC& dc);

		void Refresh() override;

	private:
		void DrawBackgroundElements(wxDC& dc, BaseInfo* pInfo);
		void DrawIconElements(wxDC& dc, BaseInfo* pInfo);
		void DrawTextElements(wxDC& dc, BaseInfo* pInfo);
		void DrawLineElements(wxDC& dc, BaseInfo* pInfo);
		void DrawLineArcElements(wxDC& dc, BaseInfo* pInfo);
		void DrawShapeElements(wxDC& dc, BaseInfo* pInfo);

		void Sort(std::vector<BaseInfo*>& list);

	private:
		wxWindow* m_pOwner;
	};
}