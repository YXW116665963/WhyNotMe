#include "table_container.h"
#include <map>
#include "image_button.h"
#include "image_label.h"
#include "progress_bar.h"
#include "edit_ctrl.h"
#include "numeric_text_ctrl.h"
#include "logger.h"
#include "text_util.h"
#include "ui_def.h"

#include "virtual_wnd_owner.h"
#include "virtual_label.h"
#include "virtual_button.h"
#include "virtual_progress_bar.h"
#include "virtual_figure.h"

#include "custom_combo_box.h"
#include "image_switch_button.h"
#include "static_text_ctrl.h"

namespace why
{
	namespace
	{
		std::map<std::string, ControlType> g_controlTypes =
		{
			{"Label", ControlType::e_static_ctrl},
			{"Edit", ControlType::e_edit_ctrl},
			{"Combobox", ControlType::e_combobox_ctrl},
			{"Button", ControlType::e_button_ctrl},
			{"Groupbox", ControlType::e_groupbox_ctrl},
			{"CheckBox", ControlType::e_checkbox_ctrl},
			{"RaidoBtn", ControlType::e_radiobtn_ctrl},
			{"ListBox", ControlType::e_listbox_ctrl},
			{"TreeCtrl", ControlType::e_tree_ctrl},
			{"Progress", ControlType::e_progress_bar_ctrl},
			{"Table", ControlType::e_table_ctrl},
			{"ImageButton", ControlType::e_image_button_ctrl},
			{"ImageLabel", ControlType::e_image_label_ctrl},
			{"CustomComboBox", ControlType::e_custom_combobox},
			{"SwitchButton", ControlType::e_switch_button},
		};
		
		typedef std::map<std::string, uint32_t> StyleConvertMap;
		const StyleConvertMap g_buttonStyleMap =
		{
			{"transparent", static_cast<uint32_t>(ButtonStyle::transparent)},
			{"only_text", static_cast<uint32_t>(ButtonStyle::only_text)},
			{"timer_repeat", static_cast<uint32_t>(ButtonStyle::timer_repeat)}
		};

		const StyleConvertMap g_labelStyleMap =
		{
			{"left", wxALIGN_LEFT },
			{"right", wxALIGN_RIGHT },
			{"center", wxALIGN_CENTER_HORIZONTAL },
			{"no_auto_resize", wxST_NO_AUTORESIZE},
			{"ellip_end",wxST_ELLIPSIZE_END}
		};

		const StyleConvertMap g_editStyleMap =
		{ 
			{"left", wxTE_LEFT },
			{"right", wxTE_RIGHT },
			{"center", wxTE_CENTER },
			{"readonly", wxTE_READONLY },
			{"multline", wxTE_MULTILINE | wxTE_NO_VSCROLL },
			{"password", wxTE_PASSWORD},
			{"numeric", edit_style::g_uNumeric },
			{"none_background", edit_style::g_uNoneBackgroud },
		};

		const StyleConvertMap g_normalStyleMap =
		{
			{"none_border", wxBORDER_NONE}
		};

		std::map<std::string, VirtualWndType> g_virtualWndTypes =
		{
			{"VirtualLabel", VirtualWndType::e_label_ctrl},
			{"VirtualButton", VirtualWndType::e_button_ctrl},
			{"VirtualFigure", VirtualWndType::e_figure_ctrl},
			{"VirtualProgressBar", VirtualWndType::e_progress_bar_ctrl}
		};

		uint32_t GetControlStyle(const std::string &strStyle, const StyleConvertMap &styleCovertMap)
		{
			uint32_t			uStyle = 0;

			if (!strStyle.empty())
			{
				std::vector<std::string>		styleVector;

				StringSplitRegular(strStyle, ", ", styleVector);
				for (auto strStyleName : styleVector)
				{
					auto		itFind = styleCovertMap.find(strStyleName);

					if (styleCovertMap.end() != itFind)
						uStyle |= itFind->second;
				}
			}

			return uStyle;
		}

		bool FindStyle(const std::vector<std::string>& styleVecotr, const std::string& strStyle)
		{
			for (auto curStyle : styleVecotr)
			{
				if (curStyle == strStyle)
					return true;
			}

			return false;
		}

		wxWindow* StaticCreator(wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
		{
			uint32_t						uStyle = GetControlStyle(strStyle, g_labelStyleMap);
			wxPoint							ptPos(rcItem.x, rcItem.y);
			wxSize							szControl(rcItem.width, rcItem.height);
			//wxStaticText*					pStaticText = new wxStaticText(pParent, id, wxT(""), ptPos, szControl, uStyle| wxALIGN_CENTER_VERTICAL, strName);
			wxStaticText*					pStaticText = new StaticTextCtrl(pParent, id, ptPos, szControl, uStyle, strName);

			pStaticText->Wrap(-1);
			return pStaticText;
		}

		wxWindow* EditCreator(wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
		{
			uint32_t						uStyle = GetControlStyle(strStyle, g_editStyleMap);
			wxPoint							ptPos(rcItem.x, rcItem.y);
			wxSize							szControl(rcItem.width, rcItem.height);
			wxTextCtrl*						pTextCtrl = nullptr;

			if (uStyle & edit_style::g_uNumeric)
			{
				uStyle = uStyle & (~edit_style::g_uNumeric);
				pTextCtrl = new NumericTextCtrl(pParent, id, ptPos, szControl, uStyle, strName);
			}
			else
			{
				pTextCtrl = new EditCtrl(pParent, id, ptPos, szControl, uStyle | wxTE_LEFT | wxNO_BORDER, strName);
			}

			return pTextCtrl;
		}

		wxWindow* ComboBoxCreator(wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
		{
			wxPoint				ptPos(rcItem.x, rcItem.y);
			wxSize				szControl(rcItem.width, rcItem.height);
			wxComboBox*			pComboBox = new wxComboBox(pParent, id, wxEmptyString, ptPos, szControl, 0, nullptr, wxCB_READONLY | wxCB_DROPDOWN, wxDefaultValidator, strName);

			return pComboBox;
		}

		wxWindow* ButtonCreator(wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
		{
			wxPoint				ptPos(rcItem.x, rcItem.y);
			wxSize				szControl(rcItem.width, rcItem.height);
			wxButton*			pButton = new wxButton(pParent, id, wxEmptyString, ptPos, szControl, 0, wxDefaultValidator, strName);

			return pButton;
		}

		wxWindow* GroupBoxCreator(wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
		{
			wxPoint				ptPos(rcItem.x, rcItem.y);
			wxSize				szControl(rcItem.width, rcItem.height);
			wxStaticBox*		pGroupBox = new wxStaticBox(pParent, id, wxEmptyString, ptPos, szControl, 0, strName);

			return pGroupBox;
		}

		wxWindow* CheckBoxCreator(wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
		{
			wxPoint				ptPos(rcItem.x, rcItem.y);
			wxSize				szControl(rcItem.width, rcItem.height);
			wxCheckBox*			pCheckBox = new wxCheckBox(pParent, id, wxEmptyString, ptPos, szControl, 0, wxDefaultValidator, strName);

			return pCheckBox;
		}

		wxWindow* ListBoxCreator(wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
		{
			wxPoint				ptPos(rcItem.x, rcItem.y);
			wxSize				szControl(rcItem.width, rcItem.height);
			wxListBox*			pListBox = new wxListBox(pParent, id, ptPos, szControl, 0, nullptr, 0, wxDefaultValidator, strName);

			return pListBox;
		}

		wxWindow* TreeCtrlCreator(wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
		{
			wxPoint				ptPos(rcItem.x, rcItem.y);
			wxSize				szControl(rcItem.width, rcItem.height);
			uint32_t			uStyle = GetControlStyle(strStyle, g_normalStyleMap);
			wxTreeCtrl*			pTreeCtrl = new wxTreeCtrl(pParent, id, ptPos, szControl, uStyle |wxTR_DEFAULT_STYLE | wxVSCROLL | wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT, wxDefaultValidator, strName);

			return pTreeCtrl;
		}

		wxWindow* RadioButtonCreator(wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
		{
			wxPoint				ptPos(rcItem.x, rcItem.y);
			wxSize				szControl(rcItem.width, rcItem.height);
			wxRadioButton*		pRadioBtn = new wxRadioButton(pParent, id, wxEmptyString, ptPos, szControl, 0, wxDefaultValidator, strName);

			return pRadioBtn;
		}

		wxWindow* ProgressBarCreator(wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
		{
			wxPoint				ptPos(rcItem.x, rcItem.y);
			wxSize				szControl(rcItem.width, rcItem.height);
			ProgressBar*		pProgressBar = new ProgressBar(pParent, id, ptPos, szControl, 0, strName);

			return pProgressBar;
		}

		wxWindow* TableCtrlCreator(wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
		{
			wxPoint					ptPos(rcItem.x, rcItem.y);
			wxSize					szControl(rcItem.width, rcItem.height);
			uint32_t				uStyle = GetControlStyle(strStyle, g_normalStyleMap);
			wxDataViewListCtrl*		pTableCtrl = new wxDataViewListCtrl(pParent, id, ptPos, szControl, uStyle | wxDV_ROW_LINES| wxDV_VARIABLE_LINE_HEIGHT, wxDefaultValidator);
			
			pTableCtrl->SetName(strName);
			//pTableCtrl->Bind(wxEVT_ERASE_BACKGROUND, [](wxEraseEvent& event) {event.Skip(false); });
			return pTableCtrl;
		}
		
		wxWindow* ImageButtonCreator(wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
		{
			uint32_t				uStyle = GetControlStyle(strStyle, g_buttonStyleMap);
			wxPoint					ptPos(rcItem.x, rcItem.y);
			wxSize					szControl(rcItem.width, rcItem.height);
			ImageButton*			pImageButton = nullptr;

			pImageButton = new ImageButton(pParent, id, wxEmptyString, ptPos, szControl, uStyle, strName);
			pImageButton->SetButtonStyle(uStyle);
			return pImageButton;
		}

		wxWindow* ImageLabelCreator(wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
		{
			wxPoint					ptPos(rcItem.x, rcItem.y);
			wxSize					szControl(rcItem.width, rcItem.height);
			ImageLabel*				pImageLabel = new ImageLabel(pParent, id, ptPos, szControl, 0, strName);

			return pImageLabel;
		}

		wxWindow* CustomComboBoxCreator(wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
		{
			wxPoint					ptPos(rcItem.x, rcItem.y);
			wxSize					szControl(rcItem.width, rcItem.height);
			CustomComboBox*			pCustomComboBox = new CustomComboBox(pParent, id, ptPos, szControl, 0, strName);

			return pCustomComboBox;
		}

		wxWindow* ImageSwitchButtonCreator(wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
		{
			wxPoint					ptPos(rcItem.x, rcItem.y);
			wxSize					szControl(rcItem.width, rcItem.height);
			ImageSwitchButton*		pImageSwitchButton = new ImageSwitchButton(pParent, id, wxEmptyString, ptPos, szControl, 0, strName);

			return pImageSwitchButton;
		}
	}

	ControlType GetControlType(const std::string& strControlType)
	{
		ControlType				eType = ControlType::e_empty_ctrl;
		auto					itFind = g_controlTypes.find(strControlType);

		if (g_controlTypes.end() != itFind)
		{
			eType = itFind->second;
		}
		else
		{
			if (strControlType != "Graphic")
				LOG_ERROR << "Don't support control type:" << strControlType;
		}
			
		return eType;
	}

	VirtualWndType GetVirtualWndType(const std::string& strVirtualWndType)
	{
		VirtualWndType				eType = VirtualWndType::e_empty_ctrl;
		auto						itFind = g_virtualWndTypes.find(strVirtualWndType);

		if (g_virtualWndTypes.end() != itFind)
			eType = itFind->second;
		else
			LOG_ERROR << "Don't support virtual wnd type:" << strVirtualWndType;
		return eType;
	}

	wxWindow* CreateControl(ControlType eControlType, wxWindow* pParent, wxWindowID id, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
	{
		wxWindow*		pControl = nullptr;

		switch (eControlType)
		{
			case why::ControlType::e_empty_ctrl:
				break;
			case why::ControlType::e_static_ctrl:
				pControl = StaticCreator(pParent, id, rcItem, strName, strStyle);
				break;
			case why::ControlType::e_edit_ctrl:
				pControl = EditCreator(pParent, id, rcItem, strName, strStyle);
				break;
			case why::ControlType::e_combobox_ctrl:
				pControl = ComboBoxCreator(pParent, id, rcItem, strName, strStyle);
				break;
			case why::ControlType::e_button_ctrl:
				pControl = ButtonCreator(pParent, id, rcItem, strName, strStyle);
				break;
			case why::ControlType::e_groupbox_ctrl:
				pControl = GroupBoxCreator(pParent, id, rcItem, strName, strStyle);
				break;
			case why::ControlType::e_checkbox_ctrl:
				pControl = CheckBoxCreator(pParent, id, rcItem, strName, strStyle);
				break;
			case why::ControlType::e_radiobtn_ctrl:
				pControl = RadioButtonCreator(pParent, id, rcItem, strName, strStyle);
				break;
			case why::ControlType::e_listbox_ctrl:
				pControl = ListBoxCreator(pParent, id, rcItem, strName, strStyle);
				break;
			case why::ControlType::e_tree_ctrl:
				pControl = TreeCtrlCreator(pParent, id, rcItem, strName, strStyle);
				break;
			case why::ControlType::e_progress_bar_ctrl:
				pControl = ProgressBarCreator(pParent, id, rcItem, strName, strStyle);
				break;
			case why::ControlType::e_table_ctrl:
				pControl = TableCtrlCreator(pParent, id, rcItem, strName, strStyle);
				break;
			case why::ControlType::e_image_button_ctrl:
				pControl = ImageButtonCreator(pParent, id, rcItem, strName, strStyle);
				break;
			case why::ControlType::e_image_label_ctrl:
				pControl = ImageLabelCreator(pParent, id, rcItem, strName, strStyle);
				break;
			case why::ControlType::e_custom_combobox:
				pControl = CustomComboBoxCreator(pParent, id, rcItem, strName, strStyle);
				break;
			case why::ControlType::e_switch_button:
				pControl = ImageSwitchButtonCreator(pParent, id, rcItem, strName, strStyle);
				break;
			default:
				break;
		}

		return pControl;
	}

	VirtualWnd* CreateVirtualWnd(VirtualWndType eType, VirtualWndOwner* pOwner, const wxRect& rcItem, const wxString& strName, const std::string& strStyle)
	{
		VirtualWnd* pVirtualWnd = nullptr;

		switch (eType)
		{
			case VirtualWndType::e_label_ctrl:
				pVirtualWnd = new VirtualLabel(pOwner, wxEmptyString, rcItem, strName);
				break;
			case VirtualWndType::e_button_ctrl:
				pVirtualWnd = new VirtualButton(pOwner, wxEmptyString, rcItem, strName);
				break;
			case VirtualWndType::e_progress_bar_ctrl:
				pVirtualWnd = new VirtualProgressBar(pOwner, wxEmptyString, rcItem, strName);
				break;
			case VirtualWndType::e_figure_ctrl:
				pVirtualWnd = new VirtualFigure(pOwner, wxEmptyString, rcItem, strName);
				break;
			default:
				break;
		}

		return pVirtualWnd;
	}
}