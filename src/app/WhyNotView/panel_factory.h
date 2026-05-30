#pragma once
#include "child_panel.h"

namespace why
{
    class IPanelFactory 
    {
    public:
        virtual ~IPanelFactory() = default;
        virtual ChildPanel* CreatePanel(wxWindow* parent, const wxPoint& pos, const wxSize& size) const = 0;
    };

	template<typename T>
	class PanelFactory : public IPanelFactory
	{
	public:
		ChildPanel* CreatePanel(wxWindow* parent, const wxPoint& pos, const wxSize& size) const override
		{
			return new T(parent, pos, size);
		}
	};
}