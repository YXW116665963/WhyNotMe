#include "edit_ctrl.h"
#include "singleton.h"
#include "resource_manager.h"

namespace why
{
    wxIMPLEMENT_CLASS(EditCtrl, wxTextCtrl);

    EditCtrl::EditCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, uint32_t uStyle, const wxString& name)
        : wxTextCtrl(parent, id, wxEmptyString, pos, size, uStyle | wxTE_LEFT | wxNO_BORDER | wxTE_PROCESS_ENTER, wxDefaultValidator, name)
    {
    }

    bool EditCtrl::Enable(bool enable)
    {
        bool            bRet = wxTextCtrl::Enable(enable);

        if (enable)
        {
            if (m_pPanelGraphic)
            {
                if (m_pDisableBKElement)
                    m_pDisableBKElement->Show(false);

                if (m_pNormalBKElement)
                    m_pNormalBKElement->Show(true);

                m_pPanelGraphic->Refresh();
            }
            SetBackgroundColour(m_crEnableBackground);
        }
        else
        {
            if (m_pPanelGraphic)
            {
                if (m_pDisableBKElement)
                    m_pDisableBKElement->Show(true);

                if (m_pNormalBKElement)
                    m_pNormalBKElement->Show(false);

                m_pPanelGraphic->Refresh();
            }
            SetBackgroundColour(m_crDisableBackground);
        }
        Refresh();
        if (m_pPanelGraphic)
            GetParent()->Refresh();
        return bRet;
    }

    void EditCtrl::SetBackground(const wxColour& crEnableColor, const wxColour& crDisableColor)
    {
        m_crEnableBackground = crEnableColor;
        m_crDisableBackground = crDisableColor;
        if (IsEnabled())
            SetBackgroundColour(m_crEnableBackground);
        else
            SetBackgroundColour(m_crDisableBackground);
        Refresh();
    }

    void EditCtrl::SetBackgroundElement(PanelGraphic* pPanelGraphic, IPanelGraphicElement* pNormalBKElement, IPanelGraphicElement* pDisableBKElement)
    {
        m_pPanelGraphic = pPanelGraphic;
        m_pNormalBKElement = pNormalBKElement;
        m_pDisableBKElement = pDisableBKElement;
    }
}