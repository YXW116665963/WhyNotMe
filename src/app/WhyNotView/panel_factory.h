#pragma once
#include "child_panel.h"

namespace why
{
    class IPanelFactory 
    {
    public:
        virtual ~IPanelFactory() = default;
        virtual std::vector<ChildPanel*> CreatePanels(wxWindow* pParent) = 0;
    };

	template <typename... Args>
	class PanelFactory : public IPanelFactory
	{
	public:
        std::vector<ChildPanel*> CreatePanels(wxWindow* pParent) override
        {
            std::vector<ChildPanel*> m_vecPanels;
            (CreatePanel<Args>(pParent,m_vecPanels), ...);
            return m_vecPanels;
        }

    private:
        template <typename T>
		void CreatePanel(wxWindow* pParent, std::vector<ChildPanel*>& vecPanels)
		{
			vecPanels.push_back(new T(pParent));
		}
	};
}