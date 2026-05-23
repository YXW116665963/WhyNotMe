#pragma once
#include "child_panel.h"

namespace why
{
    class IFrameFactory 
    {
    public:
        virtual ~IFrameFactory() = default;
        virtual ChildPanel* CreatePanel(wxWindow* parent, const wxPoint& pos, const wxSize& size) const = 0;
    };

	template<typename T>
	class FrameFactory : public IFrameFactory
	{
	public:
		ChildPanel* CreatePanel(wxWindow* parent, const wxPoint& pos, const wxSize& size) const override
		{
			return new T(parent, pos, size);
		}
	};
}