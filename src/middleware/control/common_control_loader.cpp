#include "common_control_loader.h"
#include "resource_manager.h"
#include "singleton.h"
#include "text_util.h"
#include "xml_util.h"
#include "logger.h"
#include "edit_ctrl.h"
#include "numeric_text_ctrl.h"
#include "background.h"
#include "ui_def.h"

#include "virtual_label.h"
#include "virtual_button.h"
#include "virtual_progress_bar.h"
#include "virtual_figure.h"

#include "custom_combo_box.h"
#include "image_switch_button.h"

namespace why
{
	namespace
	{
		std::string			g_strNormalBKRes;
		std::string			g_strReadonlyBKRes;
		wxColor				g_crEditNormalBK;
		wxColor				g_crEditReadonlyBK;

		std::map<std::string, VirtualFigure::FigureType> g_figureTypes =
		{
			{"ChipRect", VirtualFigure::FigureType::chip_rect},
			{"AimWnd", VirtualFigure::FigureType::aim_wnd},
			{"CutPath", VirtualFigure::FigureType::cut_path},
		};

		void ScaleRect(wxRect& rcRect, const WndCoordinate& wndCoordinate)
		{
			rcRect.x = (int32_t)(rcRect.x * wndCoordinate.m_fScaleX);
			rcRect.y = (int32_t)(rcRect.y * wndCoordinate.m_fScaleY);
			rcRect.width = (int32_t)(rcRect.width * wndCoordinate.m_fScaleX);
			rcRect.height = (int32_t)(rcRect.height * wndCoordinate.m_fScaleY);
		}

		int32_t GetSizeValue(int32_t nStandSize, const std::string strText)
		{
			auto			nFind = strText.find(':');

			if (std::string::npos == nFind)
			{
				int32_t			nValue = std::stoi(strText);

				if (nValue >= 0)
				{
					return nValue;
				}
				else
				{
					nValue = nStandSize + nValue;
					return (nValue > 0) ? nValue : 0;
				}
			}
			else
			{
				std::vector<std::string>		strList;
				float64_t						fNumerator;
				float64_t						fDenominator;

				StringSplitRegular(strText, ": ", strList);
				fNumerator = std::stod(strList[0]);
				fDenominator = std::stod(strList[1]);
				return (int32_t)(nStandSize * fNumerator / fDenominator);
			}
		}

		void SetControlFont(wxWindow* pControl, CXmlNode* pNode)
		{
			CXmlNode*		pFontInfo = pNode->first_node("Font", 0, false);
			bool			bUsedParentFont = true;

			if (nullptr != pFontInfo)
			{
				std::string			strFont;
				wxColour			textColor;
				
				if (GetAttributeText(pFontInfo, "name", strFont))
				{
					FontPtr			fontPtr = Singleton<ResourceMgr>::Instance()->GetFont(strFont.c_str());

					if (nullptr != fontPtr.get())
					{
						pControl->SetFont(*fontPtr);
						bUsedParentFont = false;
					}
				}

				if (GetAttributeColor(pFontInfo, "color", textColor))
					pControl->SetForegroundColour(textColor);
			}
			
			if (bUsedParentFont)
			{
				wxWindow *pParent = pControl->GetParent();
				pControl->SetFont(pParent->GetFont());
			}
		}

		void LoadStaticExtraInfo(wxStaticText* pStatic, CXmlNode* pNode)
		{
			wxColour		crBackground;

			if (GetAttributeColor(pNode, "background", crBackground))
				pStatic->SetBackgroundColour(crBackground);
		}

		int32_t GetSingleEditHeight(wxTextCtrl* pEdit)
		{
			wxClientDC		dc(pEdit);
			int32_t			nTextWidth;
			int32_t			nTextHeight;

			dc.SetFont(pEdit->GetFont());
			// "Hg" 是一个常用的测试字符串，具有上下延伸的字符
			dc.GetTextExtent("Hg", &nTextWidth, &nTextHeight); 
			return nTextHeight + 4;
		}

		void SetEditBackground(EditCtrl* pEdit, CXmlNode* pNode)
		{
			wxColor		crEnable;
			wxColor		crDisable;

			if (!GetAttributeColor(pNode, "background", crEnable))
				crEnable = g_crEditNormalBK;
				
			if (!GetAttributeColor(pNode, "disable_background", crDisable))
				crDisable = g_crEditReadonlyBK;
			
			pEdit->SetBackground(crEnable, crDisable);
		}

		void AppendEditBackground(EditCtrl* pEdit, PanelGraphic* pPanelGraphic, CXmlNode* pNode, uint32_t uStyle)
		{
			CXmlNode*				pMargin = pNode->first_node("Margin", 0, false);
			wxPoint					ptPoint = pEdit->GetPosition();
			wxSize					szSize = pEdit->GetSize();
			wxRect					rcRect(ptPoint, szSize);
			wxSize					szLeftTop;
			wxSize					szRightBottom;
			IPanelGraphicElement*	pNormalBKElement = nullptr;
			IPanelGraphicElement*	pDisableBKElement = nullptr;

			GetAttributeMargin(pMargin, szLeftTop, szRightBottom);
			if (uStyle & wxTE_MULTILINE)
			{
				ptPoint.x += szLeftTop.x;
				ptPoint.y += szLeftTop.y;
				szSize.x -= (szLeftTop.x + szRightBottom.x);
				szSize.y -= (szLeftTop.y + szRightBottom.y);
			}
			else
			{
				wxSize		szShowSize = { szSize.x, GetSingleEditHeight(pEdit) };

				ptPoint.x += szLeftTop.x;
				ptPoint.y += (szSize.y - szShowSize.y) / 2;
				szSize.x -= (szLeftTop.x + szRightBottom.x);
				szSize.y = szShowSize.y;
			}
			pEdit->SetSize(ptPoint.x, ptPoint.y, szSize.x, szSize.y);
			pNormalBKElement = pPanelGraphic->AddBackground(g_strNormalBKRes, rcRect);
			pDisableBKElement = pPanelGraphic->AddBackground(g_strReadonlyBKRes, rcRect);
			if (pEdit->IsEnabled())
				pDisableBKElement->Show(false);
			else
				pNormalBKElement->Show(false);
			pEdit->SetBackground(g_crEditNormalBK, g_crEditReadonlyBK);
			pEdit->SetBackgroundElement(pPanelGraphic, pNormalBKElement, pDisableBKElement);
		}

		void LoadEditExtraInfo(EditCtrl* pEdit, PanelGraphic* pPanelGraphic, CXmlNode* pNode)
		{
			uint32_t				uStyle = pEdit->GetWindowStyleFlag();
			std::string				strEnable;
			std::string				strDisable;
			bool					bEnable = false;

			bEnable = AttributeAsBool(pNode, "enable", true);
			pEdit->Enable(bEnable);

			if (uStyle & edit_style::g_uNoneBackgroud)
			{
				SetEditBackground(pEdit, pNode);
			}
			else
			{
				if (nullptr != pPanelGraphic)
					AppendEditBackground(pEdit, pPanelGraphic, pNode, uStyle);
			}
		}

		void LoadComboBoxExtraInfo(wxComboBox* pComboBox, CXmlNode* pNode)
		{
			CXmlNode* pOptions = pNode->first_node("Options", 0, false);
			
			if (pOptions)
			{
				std::string				strText;
				int32_t					nSel = AttributeAsInt(pOptions, "selected", 0);
				int32_t					nCount = 0;
			
				for (auto pCur = pOptions->first_node(); nullptr != pCur; pCur = pCur->next_sibling())
				{
					const char* lpNodeName = pCur->name();
					if (0 == _stricmp(lpNodeName, "Option"))
					{
						if (GetAttributeText(pCur, "text", strText))
						{
							assert(nullptr != g_pLocalizationMgr);
							strText = g_pLocalizationMgr->ToLocalString(strText);
							pComboBox->Append(wxString::FromUTF8(strText.c_str()));
							nCount++;
						}
					}
				}
			
				if (nSel < nCount)
					pComboBox->Select(nSel);
			}
		}

		void LoadButtonExtraInfo(wxButton* pButton, CXmlNode* pNode)
		{

		}

		void LoadCheckBoxExtraInfo(wxCheckBox* pCheckBox, CXmlNode* pNode)
		{
			bool		bChecked = AttributeAsBool(pNode, "checked", false);

			pCheckBox->SetValue(bChecked);
		}

		void LoadRaidoButtonExtraInfo(wxRadioButton* pRadioButton, CXmlNode* pNode)
		{
			bool				bChecked = AttributeAsBool(pNode, "checked", false);
			
			pRadioButton->SetValue(bChecked);
		}
		
		void LoadListBoxExtraInfo(wxListBox* pListBox, CXmlNode* pNode)
		{
			CXmlNode* pOptions = pNode->first_node("Options", 0, false);

			if (pOptions)
			{
				std::string				strText;
				int32_t					nSel = AttributeAsInt(pOptions, "selected", 0);
				int32_t					nCount = 0;

				for (auto pCur = pOptions->first_node(); nullptr != pCur; pCur = pCur->next_sibling())
				{
					const char* lpNodeName = pCur->name();
					if (0 == _stricmp(lpNodeName, "Option"))
					{
						if (GetAttributeText(pCur, "text", strText))
						{
							assert(nullptr != g_pLocalizationMgr);
							strText = g_pLocalizationMgr->ToLocalString(strText);
							pListBox->Append(wxString::FromUTF8(strText.c_str()));
							nCount++;
						}
					}
				}

				if (nSel < nCount)
					pListBox->Select(nSel);
			}
		}

		void LoadTreeCtrlExtraInfo(wxTreeCtrl* pTreeCtrl, CXmlNode* pNode)
		{
			wxColour		crBackground;

			if (GetAttributeColor(pNode, "background", crBackground))
				pTreeCtrl->SetBackgroundColour(crBackground);
		}

		void LoadProgressBarExtraInfo(ProgressBar* pProgressBar, CXmlNode* pNode)
		{
			std::string background = "";
			std::string foreground = "";
			GetAttributeText(pNode, "backpng", background);
			GetAttributeText(pNode, "forepng", foreground);
			BackgroundPtr	pBackground = Singleton<ResourceMgr>::Instance()->GetBackground(background.c_str());
			BackgroundPtr	pForeground = Singleton<ResourceMgr>::Instance()->GetBackground(foreground.c_str());

			pProgressBar->SetBackgroundAndForeground(pBackground, pForeground);
		}

		void LoadTableCtrlExtraInfo(wxDataViewListCtrl* pTableCtrl, CXmlNode* pNode)
		{
			CXmlNode*		pOptions = pNode->first_node("Columns", 0, false);
			wxColour		crBackground;

			if (pOptions)
			{
				std::string				strText;
				uint32_t				uCount = 0;
				uint32_t				uWide = 0;

				for (auto pCur = pOptions->first_node(); nullptr != pCur; pCur = pCur->next_sibling())
				{
					const char* lpNodeName = pCur->name();
					if (0 == _stricmp(lpNodeName, "Column"))
					{
						if (GetAttributeText(pCur, "text", strText))
						{
							assert(nullptr != g_pLocalizationMgr);
							strText = g_pLocalizationMgr->ToLocalString(strText);

							uWide = AttributeAsUInt(pCur, "wide", 80);
							wxDataViewColumn* column = new wxDataViewColumn(wxString::FromUTF8(strText), new wxDataViewTextRenderer(""), uCount, uWide);
							pTableCtrl->AppendColumn(column);
							uCount++;
						}
					}
				}
			}

			if (GetAttributeColor(pNode, "background", crBackground))
				pTableCtrl->SetBackgroundColour(crBackground);
		}

		void LoadCustomBackgroundItem(wxWindow *pOwner, CustomWidgets &customWidgets, CXmlNode* pNode)
		{
			bool					bReturn = true;
			std::string				strName, strId;
			wxRect					rcRect;
			bool                    show{ true };

			GetAttributeText(pNode, "id", strId);
			bReturn = GetAttributeText(pNode, "name", strName);
			bReturn = bReturn && GetAttributeRect(pNode, rcRect);
			show = AttributeAsBool(pNode, "visible", show);

			if (bReturn)
			{
				BackgroundPtr			backgroundPtr = Singleton<ResourceMgr>::Instance()->GetBackground(strName.c_str());

				if (nullptr != backgroundPtr.get())
					customWidgets.AddBackgroundElement(strId, backgroundPtr, rcRect, show);
			}
		}

		void LoadCustomIconItem(wxWindow *pOwner, CustomWidgets &customWidgets, CXmlNode* pNode)
		{
			bool					bReturn = true;
			std::string				strName;
			std::string				strIcon;
			wxRect					rcRect;
			bool                    show{ true };
			
			GetAttributeText(pNode, "name", strName);
			bReturn = GetAttributeText(pNode, "icon", strIcon);
			bReturn = bReturn && GetAttributeRect(pNode, rcRect);
			show = AttributeAsBool(pNode, "visible", show);

			if (bReturn)
			{
				PicturePtr			iconPtr = Singleton<ResourceMgr>::Instance()->GetPicture(strIcon.c_str());

				if (nullptr != iconPtr.get())
					customWidgets.AddIconElement(strName, iconPtr, rcRect, show);
			}
		}

		void LoadCustomTextItem(wxWindow *pOwner, CustomWidgets &customWidgets, CXmlNode* pNode)
		{
			wxSize					szButton = pOwner->GetSize();
			bool					bReturn = true;
			std::string				strName;
			std::string				strFont;
			std::string				strText;
			wxRect					rcRect;
			wxColour				crColor;
			uint32_t                uStyle{ 0 };

			GetAttributeText(pNode, "name", strName);
			bReturn = bReturn && GetAttributeText(pNode, "text", strText);
			bReturn = bReturn && GetAttributeRect(pNode, rcRect);
			bReturn = bReturn && GetAttributeText(pNode, "font", strFont);
			bReturn = bReturn && GetAttributeColor(pNode, "color", crColor);

			if (bReturn)
			{
				FontPtr			fontPtr = Singleton<ResourceMgr>::Instance()->GetFont(strFont.c_str());

				assert(nullptr != g_pLocalizationMgr);
				strText = g_pLocalizationMgr->ToLocalString(strText);

				if (GetTextAttributeStyle(pNode, "style", uStyle)) {
					customWidgets.AddTextElement(strName, fontPtr, crColor, wxString::FromUTF8(strText), rcRect, uStyle);
				}
				else {
					customWidgets.AddTextElement(strName, fontPtr, crColor, wxString::FromUTF8(strText), rcRect);
				}
			}
		}

		void LoadCustomLineItem(wxWindow *pOwner, CustomWidgets &customWidgets, CXmlNode* pNode)
		{
			std::string				strResName;
			std::string             strId = "";
			wxPoint					ptStart;
			wxPoint					ptEnd;

			if (!GetAttributeText(pNode, "name", strResName))
				return;

			if (!GetAttributePoint(pNode, "start", ptStart))
				return;

			if (!GetAttributePoint(pNode, "end", ptEnd))
				return;

			GetAttributeText(pNode, "id", strId);

			PenPtr penPtr = Singleton<ResourceMgr>::Instance()->GetPen(strResName.c_str());

			customWidgets.AddLineElement(strId, penPtr, ptStart, ptEnd);
		}

		void LoadCustomWidgetsItems(wxWindow *pOwner, CustomWidgets &customWidgets, CXmlNode* pNode)
		{
			for (auto pCur = pNode->first_node(); nullptr != pCur; pCur = pCur->next_sibling())
			{
				const	char* lpNodeName = pCur->name();

				if (0 == _stricmp(lpNodeName, "Background"))
				{
					LoadCustomBackgroundItem(pOwner, customWidgets, pCur);
					continue;
				}

				if (0 == _stricmp(lpNodeName, "Icon"))
				{
					LoadCustomIconItem(pOwner, customWidgets, pCur);
					continue;
				}
				
				if (0 == _stricmp(lpNodeName, "Text"))
				{
					LoadCustomTextItem(pOwner, customWidgets, pCur);
					continue;
				}

				if (0 == _stricmp(lpNodeName, "Line"))
				{
					LoadCustomLineItem(pOwner, customWidgets, pCur);
					continue;
				}
			}
		}

		void LoadImageButtonExtraInfo(ImageButton* pButton, CXmlNode* pNode)
		{
			CXmlNode*			pFindNode = nullptr;
			wxColor				crBackground;

			if (GetAttributeColor(pNode, "background", crBackground))
				pButton->SetBackgroundColour(crBackground);

			pFindNode = pNode->first_node("GeneralRes", 0, false);
			if (pFindNode)
			{
				std::string		strResName;

				if (GetAttributeText(pFindNode, "name", strResName))
					pButton->SetControlRes(strResName);
			}

			pFindNode = pNode->first_node("Select", 0, false);
			if (pFindNode)
			{
				std::string		strSelectName;

				if (GetAttributeText(pFindNode, "name", strSelectName))
					pButton->SetSelectIconName(strSelectName);
			}
			
			pFindNode = pNode->first_node("Childs", 0, false);
			if (pFindNode)
				LoadCustomWidgetsItems(pButton, pButton->GetCustomWidgets(), pFindNode);
		}

		void LoadImageLabelExtraInfo(ImageLabel* pLabel, CXmlNode* pNode)
		{
			CXmlNode*					pFindNode = nullptr;
			wxColor						crBackground;

			if (GetAttributeColor(pNode, "background", crBackground))
				pLabel->SetBackgroundColour(crBackground);

			pFindNode = pNode->first_node("BackgroundRes", 0, false);
			if (pFindNode)
			{
				std::string				strBackground;

				GetAttributeText(pFindNode, "name", strBackground);
				if (!strBackground.empty())
				{
					BackgroundPtr			backgroundPtr = Singleton<ResourceMgr>::Instance()->GetBackground(strBackground.c_str());

					pLabel->SetBackground(backgroundPtr);
				}
			}

			pFindNode = pNode->first_node("Childs", 0, false);
			if (pFindNode)
				LoadCustomWidgetsItems(pLabel, pLabel->GetCustomWidgets(), pFindNode);
		}

		void LoadCustomComboBoxExtraInfo(CustomComboBox* pCustomComboBox, CXmlNode* pNode)
		{
			CXmlNode*			pFindNode = nullptr;
			wxColor				crBackground;
			std::string			strBackground;
			bool				bEnable = false;

			if (GetAttributeColor(pNode, "color", crBackground))
				pCustomComboBox->SetBackgroundColour(crBackground);

			bEnable = AttributeAsBool(pNode, "enable", true);
			pCustomComboBox->Enable(bEnable);

			if (bEnable)
			{
				if (GetAttributeText(pNode, "background", strBackground))
				{
					BackgroundPtr			backgroundPtr 
						= Singleton<ResourceMgr>::Instance()->GetBackground(strBackground.c_str());

					pCustomComboBox->SetBackground(backgroundPtr);
				}

			}
			else
			{
				BackgroundPtr			backgroundPtr
					= Singleton<ResourceMgr>::Instance()->GetBackground("right_edit_readonly");

				pCustomComboBox->SetBackground(backgroundPtr);
			}


			pFindNode = pNode->first_node("ButtonRes", 0, false);
			if (pFindNode)
			{
				std::string			strResName;
				uint32_t			uButtonSize;

				GetAttributeText(pFindNode, "name", strResName);
				uButtonSize = AttributeAsUInt(pFindNode, "size", 40);
				pCustomComboBox->SetButtonRes(strResName, uButtonSize);
			}

			pFindNode = pNode->first_node("ButtonIcon", 0, false);
			if (pFindNode)
			{
				std::string			strIconName;
				wxSize				szIcon;

				GetAttributeText(pFindNode, "name", strIconName);
				szIcon.x = AttributeAsUInt(pFindNode, "width", 10);
				szIcon.y = AttributeAsUInt(pFindNode, "height", 6);
				auto picturePtr = Singleton<ResourceMgr>::Instance()->GetPicture(strIconName.c_str());
				pCustomComboBox->SetButtonIcon(picturePtr, szIcon);
			}

			pFindNode = pNode->first_node("Options", 0, false);
			if (pFindNode)
			{
				std::string				strText;
				int32_t					nSel = AttributeAsInt(pFindNode, "selected", 0);
				std::string             strType;
				int32_t					nCount = 0;
				uint32_t				uUserData = 0;

				GetAttributeText(pFindNode, "type", strType);

				for (auto pCur = pFindNode->first_node(); nullptr != pCur; pCur = pCur->next_sibling())
				{
					const char* lpNodeName = pCur->name();
					if (0 == _stricmp(lpNodeName, "Option"))
					{
						uUserData = AttributeAsUInt(pCur, "value", 0);
						if (GetAttributeText(pCur, "text", strText))
						{
							assert(nullptr != g_pLocalizationMgr);
							strText = g_pLocalizationMgr->ToLocalString(strText);
							pCustomComboBox->Append(wxString::FromUTF8(strText.c_str()), uUserData);
							nCount++;	
						}
					}
				}

				if (nSel < nCount)
					pCustomComboBox->SetSelection(nSel);

				if ("dateTime" == strType) {
					pCustomComboBox->SetTypeDateTime();
				}
			}
		}

		void LoadImageSwitchButtonExtraInfo(ImageSwitchButton* pImageSwitchButton, CXmlNode* pNode)
		{
			CXmlNode*			pFindNode = nullptr;
			wxColor				crBackground;

			if (GetAttributeColor(pNode, "background", crBackground))
				pImageSwitchButton->SetBackgroundColour(crBackground);

			pFindNode = pNode->first_node("ButtonRes", 0, false);
			if (pFindNode)
			{
				std::string			strUncheckRes;
				std::string			strCheckRes;

				GetAttributeText(pFindNode, "uncheck", strUncheckRes);
				GetAttributeText(pFindNode, "check", strCheckRes);
				pImageSwitchButton->SetControlRes(strUncheckRes, strCheckRes);
			}

			pImageSwitchButton->SetCheck(AttributeAsBool(pNode, "check", false));

			pFindNode = pNode->first_node("UserData", 0, false);
			if (pFindNode)
			{
				pImageSwitchButton->SetUserData(false, AttributeAsUInt(pNode, "uncheck", 0));
				pImageSwitchButton->SetUserData(true, AttributeAsUInt(pNode, "check", 1));
			}
		}

		wxColour GetBackgroundColor(const std::string& strResName, const wxColour &crDefault)
		{
			BackgroundPtr			backgroundPtr = Singleton<ResourceMgr>::Instance()->GetBackground(strResName.c_str());
			
			if (nullptr != backgroundPtr.get())
			{
				PictureBackground*	pPictureBK = dynamic_cast<PictureBackground*>(backgroundPtr.get());
				
				if (pPictureBK)
					return pPictureBK->GetCenterPixel();
			}

			return crDefault;
		}

		void SetVirtualWndFont(VirtualWnd* pVirtualWnd, CXmlNode* pNode)
		{
			CXmlNode*		pFontInfo = pNode->first_node("Font", 0, false);
			bool			bUsedParentFont = true;

			if (nullptr != pFontInfo)
			{
				std::string			strFont;
				wxColour			textColor;

				if (GetAttributeText(pFontInfo, "name", strFont))
				{
					FontPtr			fontPtr = Singleton<ResourceMgr>::Instance()->GetFont(strFont.c_str());

					if (nullptr != fontPtr.get())
					{
						pVirtualWnd->SetFont(*fontPtr);
						bUsedParentFont = false;
					}
				}

				if (GetAttributeColor(pFontInfo, "color", textColor))
					pVirtualWnd->SetForegroundColour(textColor);
			}
		}

		void LoadVirtualLabelExtraInfo(VirtualLabel* pVirtualWnd, const WndCoordinate& wndCoordinate, CXmlNode* pNode)
		{
			wxColour		crBackground;

			if (GetAttributeColor(pNode, "background", crBackground))
				pVirtualWnd->SetBackgroundColour(crBackground);
		}

		void LoadVirtualButtonIconItem(VirtualButton* pButton, const WndCoordinate& wndCoordinate, CXmlNode* pNode)
		{
			bool					bReturn = true;
			std::string				strName;
			std::string				strNormal;
			std::string				strHover;
			std::string				strPushed;
			wxRect					rcRect;

			GetAttributeText(pNode, "name", strName);
			bReturn = GetAttributeText(pNode, "normal", strNormal);
			GetAttributeText(pNode, "hover", strHover);
			GetAttributeText(pNode, "pushed", strPushed);
			bReturn = bReturn && GetAttributeRect(pNode, rcRect);

			if (bReturn)
			{
				PicturePtr			normalPtr = Singleton<ResourceMgr>::Instance()->GetPicture(strNormal.c_str());
				PicturePtr			hoverPtr = Singleton<ResourceMgr>::Instance()->GetPicture(strHover.c_str());
				PicturePtr			pushedPtr = Singleton<ResourceMgr>::Instance()->GetPicture(strPushed.c_str());

				if (nullptr != normalPtr.get())
				{
					if (nullptr == hoverPtr.get())
						hoverPtr = normalPtr;

					if (nullptr == pushedPtr.get())
						pushedPtr = normalPtr;

					ScaleRect(rcRect, wndCoordinate);
					pButton->AddIconElement(strName, normalPtr, hoverPtr, pushedPtr, rcRect);
				}
			}
		}

		void LoadVirtualButtonTextItem(VirtualButton* pButton, const WndCoordinate& wndCoordinate, CXmlNode* pNode)
		{
			wxSize					szButton = pButton->GetSize();
			bool					bReturn = true;
			std::string				strName;
			std::string				strFont;
			std::string				strText;
			wxRect					rcRect;
			wxColour				crColor;

			GetAttributeText(pNode, "name", strName);
			bReturn = bReturn && GetAttributeText(pNode, "text", strText);
			bReturn = bReturn && GetAttributeRect(pNode, rcRect);
			bReturn = bReturn && GetAttributeText(pNode, "font", strFont);
			bReturn = bReturn && GetAttributeColor(pNode, "color", crColor);

			if (bReturn)
			{
				FontPtr			fontPtr = Singleton<ResourceMgr>::Instance()->GetFont(strFont.c_str());

				assert(nullptr != g_pLocalizationMgr);
				strText = g_pLocalizationMgr->ToLocalString(strText);
				ScaleRect(rcRect, wndCoordinate);
				pButton->AddTextElement(strName, fontPtr, crColor, wxString::FromUTF8(strText), rcRect);
			}
		}

		void LoadVirtualButtonChildItems(VirtualButton* pButton, const WndCoordinate& wndCoordinate, CXmlNode* pNode)
		{
			for (auto pCur = pNode->first_node(); nullptr != pCur; pCur = pCur->next_sibling())
			{
				const char* lpNodeName = pCur->name();

				if (0 == _stricmp(lpNodeName, "Icon"))
				{
					LoadVirtualButtonIconItem(pButton, wndCoordinate, pCur);
					continue;
				}

				if (0 == _stricmp(lpNodeName, "Text"))
				{
					LoadVirtualButtonTextItem(pButton, wndCoordinate, pCur);
					continue;
				}
			}
		}

		void LoadVirtualButtonExtraInfo(VirtualButton* pButton, const WndCoordinate& wndCoordinate, CXmlNode* pNode)
		{
			CXmlNode* pFindNode = pNode->first_node("Icon", 0, false);
			wxColor				crBackground;

			if (GetAttributeColor(pNode, "background", crBackground))
				pButton->SetBackgroundColour(crBackground);

			pFindNode = pNode->first_node("GeneralRes", 0, false);
			if (pFindNode)
			{
				std::string		strResName;

				if (GetAttributeText(pFindNode, "name", strResName))
					pButton->SetControlRes(strResName);
			}

			pFindNode = pNode->first_node("Childs", 0, false);
			if (pFindNode)
				LoadVirtualButtonChildItems(pButton, wndCoordinate, pFindNode);
		}

		void LoadVirtualProgressBarExtraInfo(VirtualProgressBar* pProgressBar, const WndCoordinate& wndCoordinate, CXmlNode* pNode)
		{
			bool			bHorizontal = AttributeAsBool(pNode, "horizontal", true);
			bool			bOnlyBackgroundClicked = AttributeAsBool(pNode, "OnlyBackgroundClicked", true);
			CXmlNode*		pFindNode = pNode->first_node("Background", 0, false);
			
			pProgressBar->SetDirection(bHorizontal);
			pProgressBar->SetClickRange(bOnlyBackgroundClicked);

			if (pFindNode)
			{
				int32_t					nHeight = AttributeAsInt(pFindNode, "height", 8);
				std::string				strNormalRes;
				std::string				strSelectedRes;
				BackgroundPtr			normalBKPtr;
				BackgroundPtr			selectedBKPtr;

				if (GetAttributeText(pFindNode, "normal", strNormalRes))
					normalBKPtr = Singleton<ResourceMgr>::Instance()->GetBackground(strNormalRes.c_str());

				if (GetAttributeText(pFindNode, "selected", strSelectedRes))
					selectedBKPtr = Singleton<ResourceMgr>::Instance()->GetBackground(strSelectedRes.c_str());

				if (bHorizontal)
					nHeight = (int32_t)(nHeight * wndCoordinate.m_fScaleY);
				else
					nHeight = (int32_t)(nHeight * wndCoordinate.m_fScaleX);
				pProgressBar->AddBackground(normalBKPtr, selectedBKPtr, nHeight);
			}

			pFindNode = pNode->first_node("Slider", 0, false);
			if (pFindNode)
			{
				std::string				strSliderRes;
				PicturePtr				sliderPtr;

				if (GetAttributeText(pFindNode, "icon", strSliderRes))
				{
					sliderPtr = Singleton<ResourceMgr>::Instance()->GetPicture(strSliderRes.c_str());
					pProgressBar->AddSlider(sliderPtr);
				}
			}

			pFindNode = pNode->first_node("Range", 0, false);
			if (pFindNode)
			{
				int32_t					nMin = AttributeAsInt(pFindNode, "min", 0);
				int32_t					nMax = AttributeAsInt(pFindNode, "max", 100);
				int32_t					nValue = AttributeAsInt(pFindNode, "value", 0);

				pProgressBar->SetRange(nMin, nMax);
				pProgressBar->SetPos(nValue);
			}
		}

		void LoadVirtualFigureExtraInfo(VirtualFigure *pFigure, const WndCoordinate& wndCoordinate, CXmlNode* pNode)
		{
			std::string			strText;

			if (GetAttributeText(pNode, "type", strText))
			{
				auto			itFind = g_figureTypes.find(strText);
				
				if (g_figureTypes.end() != itFind)
					pFigure->SetFigure(itFind->second);
			}

			if (GetAttributeText(pNode, "main_pen", strText))
			{
				PenPtr			mainPenPtr = Singleton<ResourceMgr>::Instance()->GetPen(strText.c_str());

				if (nullptr != mainPenPtr)
					pFigure->SetMainPen(mainPenPtr);
			}

			if (GetAttributeText(pNode, "sub_pen", strText))
			{
				PenPtr			mainPenPtr = Singleton<ResourceMgr>::Instance()->GetPen(strText.c_str());

				if (nullptr != mainPenPtr)
					pFigure->SetSubPen(mainPenPtr);
			}
		}
	}

	void SetDefaultEditBackground(const std::string& strNormal, const std::string& strReadonly)
	{
		wxColour			crDefault;

		g_strNormalBKRes = strNormal;
		g_strReadonlyBKRes = strReadonly;

		crDefault.Set(255, 255, 255);
		g_crEditNormalBK = GetBackgroundColor(g_strNormalBKRes, crDefault);

		crDefault.Set(248, 248, 248);
		g_crEditReadonlyBK = GetBackgroundColor(g_strReadonlyBKRes, crDefault);
	}

	bool LoadWindowRect(const WndCoordinate& wndCoordinate, CXmlNode* pNode, wxRect& rcRect)
	{
		wxRect				rcRoot(wxPoint(0, 0), wndCoordinate.m_szOrigin);

		if (LoadWindowRect(rcRoot, pNode, rcRect))
		{
			ScaleRect(rcRect, wndCoordinate);
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool LoadWindowRect(const wxRect& rcRoot, CXmlNode* pNode, wxRect& rcRect)
	{
		rcRect.width = AttributeAsInt(pNode, "width", 0); 
		rcRect.height = AttributeAsInt(pNode, "height", 0);
		rcRect.x = AttributeAsInt(pNode, "x", 0);
		if (rcRect.x < 0)
			rcRect.x = (rcRoot.x + rcRoot.width) + rcRect.x - rcRect.width;
		rcRect.y = AttributeAsInt(pNode, "y", 0);
		if (rcRect.y < 0)
			rcRect.y = (rcRoot.y + rcRoot.height) + rcRect.y - rcRect.height;
	
		return true;
	}
	
	bool LoadWindowRect(wxWindow* pParent, CXmlNode* pNode, wxRect &rcWindow)
	{
		wxSize				szParent = pParent->GetClientSize();
		
		return LoadWindowRect(szParent, pNode, rcWindow);
	}

	bool LoadCellRect(CXmlNode* pNode, float64_t& fColumnPos, float64_t& fLinePos, float64_t& fWidth, float64_t& fHeight)
	{
		fColumnPos = AttributeAsFloat(pNode, "x", 0.0);
		fLinePos = AttributeAsFloat(pNode, "y", 0.0);
		fWidth = AttributeAsFloat(pNode, "width", 1.0);
		fHeight = AttributeAsFloat(pNode, "height", 1.0);
		return true;
	}

	bool LoadCellPos(CXmlNode* pNode, int32_t& nColumnPos, int32_t& nLinePos)
	{
		nColumnPos = AttributeAsInt(pNode, "x", 0);
		nLinePos = AttributeAsInt(pNode, "y", 0);
		return true;
	}

	void LoadExtraInfo(wxWindow* pControl, ControlType& eType, PanelGraphic* pPanelGraphic, CXmlNode* pNode)
	{
		SetControlFont(pControl, pNode);
		switch (eType)
		{
			case why::ControlType::e_static_ctrl:
				LoadStaticExtraInfo(dynamic_cast<wxStaticText*>(pControl), pNode);
				break;
			case why::ControlType::e_edit_ctrl:
				LoadEditExtraInfo(dynamic_cast<EditCtrl*>(pControl), pPanelGraphic, pNode);
				break;
			case why::ControlType::e_combobox_ctrl:
				LoadComboBoxExtraInfo(dynamic_cast<wxComboBox*>(pControl), pNode);
				break;
			case why::ControlType::e_button_ctrl:
				LoadButtonExtraInfo(dynamic_cast<wxButton*>(pControl), pNode);
				break;
			case why::ControlType::e_groupbox_ctrl:
				break;
			case why::ControlType::e_checkbox_ctrl:
				LoadCheckBoxExtraInfo(dynamic_cast<wxCheckBox*>(pControl), pNode);
				break;
			case why::ControlType::e_radiobtn_ctrl:
				LoadRaidoButtonExtraInfo(dynamic_cast<wxRadioButton*>(pControl), pNode);
				break;
			case why::ControlType::e_listbox_ctrl:
				LoadListBoxExtraInfo(dynamic_cast<wxListBox*>(pControl), pNode);
				break;
			case why::ControlType::e_tree_ctrl:
				LoadTreeCtrlExtraInfo(dynamic_cast<wxTreeCtrl*>(pControl), pNode);
				break;
			case why::ControlType::e_progress_bar_ctrl:
				LoadProgressBarExtraInfo(dynamic_cast<ProgressBar*>(pControl), pNode);
				break;
			case why::ControlType::e_table_ctrl:
				LoadTableCtrlExtraInfo(dynamic_cast<wxDataViewListCtrl*>(pControl), pNode);
				break;
			case why::ControlType::e_image_button_ctrl:
				LoadImageButtonExtraInfo(dynamic_cast<ImageButton*>(pControl), pNode);
				break;
			case why::ControlType::e_image_label_ctrl:
				LoadImageLabelExtraInfo(dynamic_cast<ImageLabel*>(pControl), pNode);
				break;
			case why::ControlType::e_custom_combobox:
				LoadCustomComboBoxExtraInfo(dynamic_cast<CustomComboBox*>(pControl), pNode);
				break;
			case why::ControlType::e_switch_button:
				LoadImageSwitchButtonExtraInfo(dynamic_cast<ImageSwitchButton*>(pControl), pNode);
				break;
			default:
				break;
		}
	}

	void BindData(wxWindow* pControl, DataExchange *pDataExchange, CXmlNode* pNode)
	{
		if (nullptr != pDataExchange)
		{
			CXmlNode*		pBindInfo = pNode->first_node("Bind", 0, false);

			if (nullptr != pBindInfo)
			{
				BaseBindDataInfo baseDataInfo;
				baseDataInfo.m_pWindow = pControl;

				if (!GetAttributeText(pBindInfo, "Domain", baseDataInfo.m_strDomain))
					return;

				if (!GetAttributeText(pBindInfo, "Name", baseDataInfo.m_strName))
					return;

				GetAttributeText(pBindInfo, "Member", baseDataInfo.m_strChildName);
				GetAttributeText(pBindInfo, "Format", baseDataInfo.m_strFormat);
				GetAttributeText(pBindInfo, "Display", baseDataInfo.m_strDisplay);

				if (HasAttribute(pBindInfo, "min"))
				{
					baseDataInfo.m_uThresholdMask |= DataThreshold::g_uMin;
					GetAttributeText(pBindInfo, "Min", baseDataInfo.m_strMin);
				}

				if (HasAttribute(pBindInfo, "max"))
				{
					baseDataInfo.m_uThresholdMask |= DataThreshold::g_uMax;
					GetAttributeText(pBindInfo, "Max", baseDataInfo.m_strMax);
				}

				auto pSubscribeInfo = pBindInfo->first_node("Subscribe", 0, false);
				if (pSubscribeInfo)
					BindDynamicData(baseDataInfo, pDataExchange, pSubscribeInfo);
				else
					pDataExchange->Bind(baseDataInfo);
			}
		}
	}

	void BindDynamicData(BaseBindDataInfo& dataInfo, DataExchange *pDataExchange, CXmlNode* pNode)
	{
		std::string strSubDomain;
		std::string strSubName;
		std::vector<DataNameInfo> vecSubscribeDatas;
		for (auto pDataNode = pNode->first_node("data"); pDataNode; pDataNode = pDataNode->next_sibling("data"))
		{
			strSubDomain.clear();
			GetAttributeText(pDataNode, "Domain", strSubDomain);

			strSubName.clear();
			GetAttributeText(pDataNode, "Name", strSubName);
			vecSubscribeDatas.push_back({ strSubDomain, strSubName });
		}

		if (!vecSubscribeDatas.empty())
			pDataExchange->Bind(dataInfo, vecSubscribeDatas);
	}

	void LoadVirtualWndInfo(VirtualWnd* pVirtualWnd, const WndCoordinate& wndCoordinate, VirtualWndType& eType, CXmlNode* pNode)
	{
		SetVirtualWndFont(pVirtualWnd, pNode);
		switch (eType)
		{
			case VirtualWndType::e_label_ctrl:
				LoadVirtualLabelExtraInfo(dynamic_cast<VirtualLabel*>(pVirtualWnd), wndCoordinate, pNode);
				break;
			case VirtualWndType::e_button_ctrl:
				LoadVirtualButtonExtraInfo(dynamic_cast<VirtualButton*>(pVirtualWnd), wndCoordinate, pNode);
				break;
			case VirtualWndType::e_progress_bar_ctrl:
				LoadVirtualProgressBarExtraInfo(dynamic_cast<VirtualProgressBar*>(pVirtualWnd), wndCoordinate, pNode);
				break;
			case VirtualWndType::e_figure_ctrl:
				LoadVirtualFigureExtraInfo(dynamic_cast<VirtualFigure*>(pVirtualWnd), wndCoordinate, pNode);
				break;
			default:
				break;
		}
	}
}