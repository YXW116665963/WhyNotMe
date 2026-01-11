#pragma once

#include "gui_import.h"
#include "background_interface.h"
#include "panel_graphic.h"

namespace why
{
    class EditCtrl : public wxTextCtrl
    {
    public:
        EditCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, uint32_t uStyle, const wxString& name);

        wxDECLARE_CLASS(EditCtrl);

    public:
        bool Enable(bool enable) override;
        void SetBackground(const wxColour& crEnableColor, const wxColour& crDisableColor);
        void SetBackgroundElement(PanelGraphic* pPanelGraphic, IPanelGraphicElement* pNormalBKElement, IPanelGraphicElement* pDisableBKElement);

    private:
        wxColour                m_crEnableBackground;
        wxColour                m_crDisableBackground;

        PanelGraphic* m_pPanelGraphic{ nullptr };
        IPanelGraphicElement* m_pNormalBKElement{ nullptr };
        IPanelGraphicElement* m_pDisableBKElement{ nullptr };
    };
}
