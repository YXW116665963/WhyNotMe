#pragma once

#include "gui_import.h"
#include "edit_ctrl.h"

namespace why
{
    class NumericTextCtrl : public EditCtrl
    {
    public:
        NumericTextCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, uint32_t uStyle, const wxString& name);

        wxDECLARE_CLASS(NumericTextCtrl);

    private:
        void OnChar(wxKeyEvent& event);
    };
}
