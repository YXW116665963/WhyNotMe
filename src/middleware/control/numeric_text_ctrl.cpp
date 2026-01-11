#include "numeric_text_ctrl.h"
#include "singleton.h"
#include "resource_manager.h"

namespace why
{
    wxIMPLEMENT_CLASS(NumericTextCtrl, EditCtrl);

    NumericTextCtrl::NumericTextCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, uint32_t uStyle, const wxString& name)
        : EditCtrl(parent, id, pos, size, uStyle | wxTE_LEFT | wxNO_BORDER, name)
    {
        Bind(wxEVT_CHAR, &NumericTextCtrl::OnChar, this);
    }

    void NumericTextCtrl::OnChar(wxKeyEvent& event)
    {
        if (event.GetKeyCode() == WXK_BACK || event.GetKeyCode() == WXK_DELETE ||
            event.GetKeyCode() == WXK_LEFT || event.GetKeyCode() == WXK_RIGHT ||
            event.GetKeyCode() == WXK_TAB || event.GetKeyCode() == WXK_RETURN)
        {
            event.Skip();
            return;
        }

        wxChar uchar = static_cast<wxChar>(event.GetUnicodeKey());
        if (uchar == 0)
            return;

        if (wxIsdigit(uchar))
        {
            event.Skip();
            return;
        }

        if (('.' == uchar) || ('-' == uchar))
        {
            wxString        currentValue = GetValue();

            if (currentValue.empty())
            {
                event.Skip();
                return;
            }

            if (wxNOT_FOUND == currentValue.Find(uchar))
            {
                if (('.' == uchar) || (0 == GetInsertionPoint()))
                {
                    event.Skip();
                    return;
                }
            }
        }
    }
}