#include "data_exchange.h"
#include "logger.h"
#include "text_util.h"
#include "numeric_text_ctrl.h"
#include "image_label.h"
#include "image_button.h"
#include "custom_combo_box.h"
#include "image_switch_button.h"
#include "ui_def.h"

namespace why
{
	namespace DataThreshold
	{
		const uint32_t g_uNone = 0;
		const uint32_t g_uMin = 1;
		const uint32_t g_uMax = 2;
	};

	wxDEFINE_EVENT(DATA_CHECKING_EVENT, wxCommandEvent);

	namespace
	{
		IDataValue* GetChildDataValue(IDataValue* pDataValue, const std::string& strChildName)
		{
			if (ValueType::eObject == pDataValue->Type())
			{
				IObjectDataValue* pObjectValue = dynamic_cast<IObjectDataValue*>(pDataValue);

				return pObjectValue->Get(strChildName.c_str());
			}

			if (ValueType::eList == pDataValue->Type())
			{
				IListDataValue* pListValue = dynamic_cast<IListDataValue*>(pDataValue);

				return pListValue->GetAt(static_cast<uint32_t>(std::stoul(strChildName)));
			}

			return nullptr;
		}
	}

	DataExchange::DataExchange(IDataCenter* pDataCenter)
		: m_pDataCenter(pDataCenter)
	{
		m_crChangedBK.Set(151, 220, 231);
	}

	DataExchange::~DataExchange()
	{
		Unsubscribe();
	}

	void DataExchange::Bind(const BaseBindDataInfo& baseDataInfo)
	{
		BindWnd				bindWnd;

		bindWnd.m_pWindow = baseDataInfo.m_pWindow;
		bindWnd.m_strChildName = baseDataInfo.m_strChildName;
		bindWnd.m_strFormat = baseDataInfo.m_strFormat;
		if (baseDataInfo.m_strDisplay.empty())
			bindWnd.m_strDisplay = baseDataInfo.m_strDomain + '|' + baseDataInfo.m_strName + '|' + baseDataInfo.m_strChildName;
		else
			bindWnd.m_strDisplay = baseDataInfo.m_strDisplay;
		bindWnd.m_eControlType = QueryControlType(baseDataInfo.m_pWindow);
		bindWnd.m_uThresholdMask = baseDataInfo.m_uThresholdMask;
		bindWnd.m_strMin = baseDataInfo.m_strMin;
		bindWnd.m_strMax = baseDataInfo.m_strMax;

		auto itData = FindBindData(baseDataInfo.m_strDomain, baseDataInfo.m_strName);
		if (m_bindList.end() == itData)
		{
			BindData			bindData;

			bindData.m_strDomain = baseDataInfo.m_strDomain;
			bindData.m_strName = baseDataInfo.m_strName;
			bindData.m_bSubscribe = false;
			bindData.m_list.push_back(bindWnd);
			m_bindList.push_back(bindData);
		}
		else
		{
			itData->m_list.push_back(bindWnd);
		}

		if (baseDataInfo.m_strName.empty())
			baseDataInfo.m_pWindow->SetLabel(wxString::FromUTF8(baseDataInfo.m_strDomain));
	}

	void DataExchange::Bind(BaseBindDataInfo& baseDataInfo, const std::vector<DataNameInfo>& arraySubscribeInfo)
	{
		if (!m_pDynamicDataHandler)
			m_pDynamicDataHandler.reset(new DynamicDataHandler(m_pDataCenter, this));

		m_pDynamicDataHandler->Bind(baseDataInfo, arraySubscribeInfo);
	}

	void DataExchange::Unbind(wxWindow* pWindow)
	{
		for (auto itData = m_bindList.begin(); itData != m_bindList.end(); itData++)
		{
			for (auto itWnd = itData->m_list.begin(); itWnd != itData->m_list.end(); itWnd++)
			{
				if (itWnd->m_pWindow == pWindow)
				{
					itData->m_list.erase(itWnd);
					return;
				}
			}
		}
	}

	void DataExchange::UnbindAll()
	{
		m_bindList.clear();
	}

	void DataExchange::SetCustomDisplayMethod(wxWindow* pWindow, const DisplayMethon& displayMethon)
	{
		bool				bFind = false;

		for (auto itData = m_bindList.begin(); itData != m_bindList.end(); itData++)
		{
			for (auto itWnd = itData->m_list.begin(); itWnd != itData->m_list.end(); itWnd++)
			{
				if (itWnd->m_pWindow == pWindow)
				{
					itWnd->m_displayMethon = displayMethon;
					bFind = true;
					break;
				}
			}

			if (bFind)
			{
				if (m_pDataCenter)
				{	
					auto pNameData = m_pDataCenter->FindData(itData->m_strDomain.c_str(), itData->m_strName.c_str());

					if (nullptr != pNameData)
						displayMethon(pWindow, pNameData);
				}
				break;
			}
		}
	}

	void DataExchange::Subscribe()
	{
		for (auto itData = m_bindList.begin(); itData != m_bindList.end(); itData++)
		{
			if (itData->m_strDomain.empty() || itData->m_strName.empty())
				continue;

			if (itData->m_bSubscribe)
				continue;

			Subscribe(itData->m_strDomain, itData->m_strName);
			HookWndEvent(itData->m_list);
			itData->m_bSubscribe = true;
		}
	}
	
	void DataExchange::Unsubscribe()
	{
		for (auto itData = m_bindList.begin(); itData != m_bindList.end(); itData++)
		{
			if (itData->m_strDomain.empty() || itData->m_strName.empty())
				continue;

			if (!itData->m_bSubscribe)
				continue;

			Unsubscribe(itData->m_strDomain, itData->m_strName);
			itData->m_bSubscribe = false;
		}
	}

	void DataExchange::ChangeBind(
		const std::string& strOldDoMain, const std::string& strOldName,
		const std::string& strNewDoMain, const std::string& strNewName)
	{
		if (strOldDoMain.empty() || strOldName.empty() || strNewDoMain.empty() || strNewName.empty())
			return;

		for (auto itData = m_bindList.begin(); itData != m_bindList.end(); itData++)
		{
			if (itData->m_strDomain == strOldDoMain && itData->m_strName == strOldName)
			{
				itData->m_bSubscribe = false;
				Unsubscribe(strOldDoMain, strOldName);
				itData->m_strDomain = strNewDoMain;
				itData->m_strName = strNewName;
				Subscribe(strNewDoMain, strNewName);
				itData->m_bSubscribe = true;
				break;
			}
		}
	}

	void DataExchange::HookWndEvent(BindWndList& bindWndList)
	{
		for (auto itWnd = bindWndList.begin(); itWnd != bindWndList.end(); itWnd++)
		{
			itWnd->m_crNormalBK = itWnd->m_pWindow->GetBackgroundColour();
			
			if ((ControlType::e_combobox_ctrl == itWnd->m_eControlType) || (ControlType::e_custom_combobox == itWnd->m_eControlType))
			{
				itWnd->m_pWindow->Bind(wxEVT_COMBOBOX, &DataExchange::OnSelectedChanged, this);
				continue;
			}

			if (ControlType::e_switch_button == itWnd->m_eControlType)
			{
				itWnd->m_pWindow->Bind(wxEVT_BUTTON, &DataExchange::OnSwitchChanged, this);
				continue;
			}

			itWnd->m_pWindow->Bind(wxEVT_TEXT, &DataExchange::OnTextChanged, this);
			itWnd->m_pWindow->Bind(DATA_CHECKING_EVENT, &DataExchange::OnDataChecking, this);
		}
	}

	void DataExchange::OnDataChanged(INamedData* pData)
	{
		auto			itFind = FindBindData(pData->Domain(), pData->Name());

		if (m_bindList.end() != itFind)
		{
			for (BindWnd& bindWnd : itFind->m_list)
				UpdateData(bindWnd, pData);
		}
	}

	void DataExchange::Subscribe(const std::string& strDomain, const std::string& strName)
	{
		if (m_pDataCenter)
			m_pDataCenter->Subscribe(strDomain.c_str(), strName.c_str(), this);
	}

	void DataExchange::Unsubscribe(const std::string& strDomain, const std::string& strName)
	{
		if (m_pDataCenter)
			m_pDataCenter->Unsubscribe(strDomain.c_str(), strName.c_str(), this);
	}

	DataExchange::BindDataListIt DataExchange::FindBindData(const std::string& strDomain, const std::string& strName)
	{
		BindDataListIt		itFind = m_bindList.end();

		for (auto it = m_bindList.begin(); it != m_bindList.end(); it++)
		{
			if ((it->m_strDomain == strDomain) && (it->m_strName == strName))
			{
				itFind = it;
				break;
			}
		}

		return itFind;
	}

	DataExchange::BindWnd* DataExchange::FindBindWnd(wxWindow* pWnd)
	{
		BindWnd* pReturn = nullptr;

		for (auto it = m_bindList.begin(); it != m_bindList.end(); it++)
		{
			for (auto itWnd = it->m_list.begin(); itWnd != it->m_list.end(); itWnd++)
			{
				if (itWnd->m_pWindow == pWnd)
				{
					pReturn = &(itWnd.operator*());
					break;
				}
			}

			if (nullptr != pReturn)
				break;
		}

		return pReturn;
	}

	ControlType DataExchange::QueryControlType(wxWindow* pWnd)
	{
		ControlType				eType = ControlType::e_empty_ctrl;

		do
		{
			if (pWnd->IsKindOf(wxCLASSINFO(wxStaticText))) 
			{
				eType = ControlType::e_static_ctrl;
				break;
			}

			if (pWnd->IsKindOf(wxCLASSINFO(EditCtrl)))
			{
				eType = ControlType::e_edit_ctrl;
				break;
			}

			if (pWnd->IsKindOf(wxCLASSINFO(NumericTextCtrl)))
			{
				eType = ControlType::e_edit_ctrl;
				break;
			}

			if (pWnd->IsKindOf(wxCLASSINFO(wxComboBox)))
			{
				eType = ControlType::e_combobox_ctrl;
				break;
			}

			if (pWnd->IsKindOf(wxCLASSINFO(wxButton)))
			{
				eType = ControlType::e_button_ctrl;
				break;
			}

			if (pWnd->IsKindOf(wxCLASSINFO(ImageButton)))
			{
				eType = ControlType::e_image_button_ctrl;
				break;
			}

			if (pWnd->IsKindOf(wxCLASSINFO(ImageLabel)))
			{
				eType = ControlType::e_image_label_ctrl;
				break;
			}

			if (pWnd->IsKindOf(wxCLASSINFO(CustomComboBox)))
			{
				eType = ControlType::e_custom_combobox;
				break;
			}

			if (pWnd->IsKindOf(wxCLASSINFO(ImageSwitchButton)))
			{
				eType = ControlType::e_switch_button;
				break;
			}
		} while (false);
		
		return eType;
	}

	void DataExchange::UpdateData(BindWnd& bindWnd, INamedData* pData)
	{
		if (bindWnd.m_displayMethon)
		{
			bindWnd.m_displayMethon(bindWnd.m_pWindow, pData);
		}
		else
		{
			std::string						strText;

			if (!GetNamedDataText(pData, bindWnd.m_strChildName, bindWnd.m_strFormat, strText))
				return;

			if (bindWnd.m_strBaseValue != strText)//why
			{
				bindWnd.m_strBaseValue = strText;
				bindWnd.m_strCurrentValue = strText;
				bindWnd.m_bChanged = false;
				SetWindowText(bindWnd.m_eControlType, bindWnd.m_pWindow, strText);
			}
		}
	}

	bool DataExchange::GetNamedDataText(INamedData* pNamedData, const std::string& strChildName, const std::string& strFormat, std::string& strText)
	{
		IDataValue*				pDataValue = pNamedData->GetValue();

		if (!strChildName.empty())
		{
			IDataValue*			pChildValue = GetChildDataValue(pDataValue, strChildName);

			if (nullptr == pChildValue)
			{
				LOG_ERROR << "GetDataText[" << pNamedData->Domain() << "/" << pNamedData->Name() << " don't found child value:" << strChildName;
				return false;
			}
			
			return GetDataValueText(pChildValue, strFormat, strText);
		}
		else
		{
			return GetDataValueText(pDataValue, strFormat, strText);
		}
	}

	bool DataExchange::SetNamedDataText(INamedData* pNamedData, const std::string& strChildName, const std::string& strText)
	{
		IDataValue* pDataValue = pNamedData->GetValue();

		if (!strChildName.empty())
		{
			IDataValue*		pChildValue = GetChildDataValue(pDataValue, strChildName);

			if (nullptr == pChildValue)
			{
				LOG_ERROR << "SetDataText[" << pNamedData->Domain() << "/" << pNamedData->Name() << " don't found child value:" << strChildName;
				return false;
			}

			return SetDataValueText(pChildValue, strText);
		}
		else
		{
			return SetDataValueText(pDataValue, strText);
		}
	}

	bool DataExchange::GetDataValueText(IDataValue* pDataValue, const std::string& strFormat, std::string& strText)
	{
		ValueType			eType = pDataValue->Type();
		bool				bRet = false;

		if ((strFormat.empty()) || (ValueType::eString == eType))
		{
			switch (eType)
			{
				case ValueType::eInt:
					strText = std::to_string(pDataValue->GetIntValue());
					bRet = true;
					break;
				case ValueType::eUInt:
					strText = std::to_string(pDataValue->GetUIntValue());
					bRet = true;
					break;
				case ValueType::eBigInt:
					strText = std::to_string(pDataValue->GetBigIntValue());
					bRet = true;
					break;
				case ValueType::eBigUInt:
					strText = std::to_string(pDataValue->GetBigUIntValue());
					bRet = true;
					break;
				case ValueType::eFloat:
					strText = std::to_string(pDataValue->GetFloatValue());
					bRet = true;
					break;
				case ValueType::eString:
					{
						uint32_t uDataSize = pDataValue->GetStringValue(nullptr, 0);
						strText.resize(uDataSize);
						pDataValue->GetStringValue(strText.data(), uDataSize+1);
						bRet = true;
					}
					break;
				default:
					break;
			}
		}
		else
		{
			switch (eType)
			{
				case ValueType::eInt:
					strText = StringFormat(strFormat.c_str(), pDataValue->GetIntValue());
					bRet = true;
					break;
				case ValueType::eUInt:
					strText = StringFormat(strFormat.c_str(), pDataValue->GetUIntValue());
					bRet = true;
					break;
				case ValueType::eBigInt:
					strText = StringFormat(strFormat.c_str(), pDataValue->GetBigIntValue());
					bRet = true;
					break;
				case ValueType::eBigUInt:
					strText = StringFormat(strFormat.c_str(), pDataValue->GetBigUIntValue());
					bRet = true;
					break;
				case ValueType::eFloat:
					strText = StringFormat(strFormat.c_str(), pDataValue->GetFloatValue());
					bRet = true;
					break;
				default:
					break;
			}
		}

		return bRet;
	}

	bool DataExchange::SetDataValueText(IDataValue* pDataValue, const std::string& strText)
	{
		ValueType			eType = pDataValue->Type();
		bool				bRet = false;

		switch (eType)
		{
			case ValueType::eInt:
				pDataValue->SetIntValue(atoi(strText.c_str()));
				bRet = true;
				break;
			case ValueType::eUInt:
				pDataValue->SetUIntValue((uint32_t)atoi(strText.c_str()));
				bRet = true;
				break;
			case ValueType::eBigInt:
				pDataValue->SetBigIntValue(_atoi64(strText.c_str()));
				bRet = true;
				break;
			case ValueType::eBigUInt:
				pDataValue->SetBigUIntValue((uint64_t)_atoi64(strText.c_str()));
				bRet = true;
				break;
			case ValueType::eFloat:
				pDataValue->SetFloatValue(atof(strText.c_str()));
				bRet = true;
				break;
			case ValueType::eString:
				pDataValue->SetStringValue(strText.c_str());
				bRet = true;
				break;
			default:
				break;
		}

		return bRet;
	}

	void DataExchange::OnTextChanged(wxCommandEvent& event)
	{
		wxWindow* pWnd = dynamic_cast<wxWindow*>(event.GetEventObject());

		if (pWnd)
		{
			auto		pBindWnd = FindBindWnd(pWnd);

			if (pBindWnd)
			{
				if (ControlType::e_edit_ctrl == pBindWnd->m_eControlType)
				{
					EditCtrl* textCtrl = dynamic_cast<EditCtrl*>(pWnd);
					wxString newText = textCtrl->GetValue();
					if (pBindWnd->m_strBaseValue != std::string(newText.ToUTF8()))
					{
						pBindWnd->m_strCurrentValue = newText;
						if (!pBindWnd->m_bChanged)
						{
							pBindWnd->m_bChanged = true;
							pBindWnd->m_bChecked = false;
						}
						pWnd->SetBackgroundColour(m_crChangedBK);
					}
					else
					{
						pBindWnd->m_strCurrentValue = pBindWnd->m_strBaseValue;
						pBindWnd->m_bChanged = false;
						pBindWnd->m_bChecked = true;
						pWnd->SetBackgroundColour(pBindWnd->m_crNormalBK);
					}
				}
			}
		}
	}

	void DataExchange::OnSelectedChanged(wxCommandEvent& event)
	{
		wxWindow* pWnd = dynamic_cast<wxWindow*>(event.GetEventObject());

		if (pWnd)
		{
			auto		pBindWnd = FindBindWnd(pWnd);

			if (pBindWnd)
			{
				if (ControlType::e_combobox_ctrl == pBindWnd->m_eControlType)
				{
					wxComboBox* pComboBox = dynamic_cast<wxComboBox*>(pWnd);
					wxString newText = std::to_string(pComboBox->GetCurrentSelection());
					if (pBindWnd->m_strBaseValue != std::string(newText.ToUTF8()))
					{
						pBindWnd->m_strCurrentValue = newText;
						pBindWnd->m_bChanged = true;
						pComboBox->SetBackgroundColour(m_crChangedBK);
					}
					else
					{
						pBindWnd->m_strCurrentValue = newText;
						pBindWnd->m_bChanged = false;
						pComboBox->SetBackgroundColour(pBindWnd->m_crNormalBK);
					}
					return;
				}

				if (ControlType::e_custom_combobox == pBindWnd->m_eControlType)
				{
					CustomComboBox* pComboBox = dynamic_cast<CustomComboBox*>(pWnd);
					wxString newText = std::to_string(pComboBox->GetCurrentData());
					if (pBindWnd->m_strBaseValue != std::string(newText.ToUTF8()))
					{
						pBindWnd->m_strCurrentValue = newText;
						pBindWnd->m_bChanged = true;
						pComboBox->SetBackgroundColour(m_crChangedBK);
					}
					else
					{
						pBindWnd->m_strCurrentValue = newText;
						pBindWnd->m_bChanged = false;
						pComboBox->SetBackgroundColour(pBindWnd->m_crNormalBK);
					}
				}
			}
		}
	}

	void DataExchange::OnSwitchChanged(wxCommandEvent& event)
	{
		wxWindow* pWnd = dynamic_cast<wxWindow*>(event.GetEventObject());

		if (pWnd)
		{
			auto		pBindWnd = FindBindWnd(pWnd);

			if (pBindWnd)
			{
				if (ControlType::e_switch_button == pBindWnd->m_eControlType)
				{
					ImageSwitchButton*	pSwitchButton = dynamic_cast<ImageSwitchButton*>(pWnd);
					bool				bChecked = pSwitchButton->IsChecked();
					wxString			newText = std::to_string(pSwitchButton->GetUserData(bChecked));
					
					if (pBindWnd->m_strBaseValue != std::string(newText.ToUTF8()))
					{
						pBindWnd->m_strCurrentValue = newText;
						pBindWnd->m_bChanged = true;
						pSwitchButton->SetBackgroundColour(m_crChangedBK);
					}
					else
					{
						pBindWnd->m_strCurrentValue = newText;
						pBindWnd->m_bChanged = false;
						pSwitchButton->SetBackgroundColour(pBindWnd->m_crNormalBK);
					}
				}
			}
		}
	}

	void DataExchange::OnDataChecking(wxCommandEvent& event)
	{
		wxWindow* pWnd = dynamic_cast<wxWindow*>(event.GetEventObject());

		if (pWnd)
		{
			auto		pBindWnd = FindBindWnd(pWnd);

			if (pBindWnd && DataThreshold::g_uNone != pBindWnd->m_uThresholdMask)
			{
				uint32_t			uWarningCode = 0;
				float64_t			fValue = 0.0;
				std::string			strLimitValue;

				fValue = (float64_t)atof(pBindWnd->m_strCurrentValue.c_str());
				if (pBindWnd->m_uThresholdMask & DataThreshold::g_uMin)
				{
					float64_t	fMin = (float64_t)atof(pBindWnd->m_strMin.c_str());

					if (fValue < fMin)
					{
						uWarningCode = g_uLowLimitCode;
						strLimitValue = pBindWnd->m_strMin;
					}
				}

				if ((0 == uWarningCode) && (pBindWnd->m_uThresholdMask & DataThreshold::g_uMax))
				{
					float64_t	fMax = (float64_t)atof(pBindWnd->m_strMax.c_str());

					if (fValue >= fMax)
					{
						uWarningCode = g_uHighLimitCode;
						strLimitValue = pBindWnd->m_strMax;
					}
				}

				pBindWnd->m_bChecked = true;
				if (0 != uWarningCode)
				{
					std::string			strPrefix = g_pLocalizationMgr->ToLocalString(pBindWnd->m_strDisplay);

					if (g_fnDataExceedLimitNotify)
						g_fnDataExceedLimitNotify(uWarningCode, strPrefix, pBindWnd->m_strCurrentValue, strLimitValue);

					SetWindowText(pBindWnd->m_eControlType, pBindWnd->m_pWindow, pBindWnd->m_strBaseValue);
					pBindWnd->m_pWindow->SetBackgroundColour(pBindWnd->m_crNormalBK);
					pBindWnd->m_strCurrentValue = pBindWnd->m_strBaseValue;
					pBindWnd->m_bChanged = false;
				}
			}
		}
	}

	void DataExchange::Apply()
	{
		INamedData*					pNameData = nullptr;
		IDataValue*					pDataValue = nullptr;
		std::string					strText;

		for (BindData& bindData : m_bindList)
		{
			if (bindData.m_strDomain.empty() || bindData.m_strName.empty())
				continue;

			pNameData = m_pDataCenter->FindData(bindData.m_strDomain.c_str(), bindData.m_strName.c_str());
			if (nullptr != pNameData)
			{
				pNameData->PauseNotify();
				pDataValue = pNameData->GetValue();
				for (BindWnd& bindWnd : bindData.m_list)
				{
					if (bindWnd.m_bChanged)
					{
						GetWindowText(bindWnd.m_eControlType, bindWnd.m_pWindow, strText);
						if (bindWnd.m_strChildName.empty())
						{
							SetDataValueText(pDataValue, strText);
							LOG_INFO << "Save data changed name:" << bindData.m_strName << "\t" << "Value:" << strText;
						}
						else
						{
							auto pObjectValue = dynamic_cast<IObjectDataValue*>(pDataValue);
							auto pChildValue = pObjectValue->Get(bindWnd.m_strChildName.c_str());

							if (nullptr != pChildValue)
								SetDataValueText(pChildValue, strText);

							LOG_INFO << "Save data changed name:" << bindData.m_strName << "\t" << "child:" << bindWnd.m_strChildName << "\t" << "Value:" << strText;;
						}

						bindWnd.m_pWindow->SetBackgroundColour(bindWnd.m_crNormalBK);
						bindWnd.m_strBaseValue = strText;
						bindWnd.m_strCurrentValue = strText;
						bindWnd.m_bChanged = false;
					}
				}
				pNameData->ResumeNotify();
			}
		}
	}

	void DataExchange::Reset()
	{
		INamedData*				pNameData = nullptr;

		for (auto bindData : m_bindList)
		{
			if (bindData.m_strDomain.empty() || bindData.m_strName.empty())
				continue;

			pNameData = m_pDataCenter->FindData(bindData.m_strDomain.c_str(), bindData.m_strName.c_str());
			if (nullptr != pNameData)
			{
				for (auto bindWnd : bindData.m_list)
				{
					if (bindWnd.m_bChanged)
					{
						UpdateData(bindWnd, pNameData);

						bindWnd.m_pWindow->SetBackgroundColour(bindWnd.m_crNormalBK);
						bindWnd.m_strCurrentValue = bindWnd.m_strBaseValue;
						bindWnd.m_bChanged = false;
					}
				}
			}
		}
	}

	bool DataExchange::GetWindowText(ControlType eType, wxWindow* pWnd, std::string& strText)
	{
		bool		bRet = false;

		switch (eType)
		{
			case why::ControlType::e_empty_ctrl:
				break;
			case why::ControlType::e_static_ctrl:
				{
					wxStaticText* pStaticText = dynamic_cast<wxStaticText*>(pWnd);
					assert(pStaticText);
					strText = pStaticText->GetLabel().ToUTF8();
					bRet = true;
				}
				break;
			case why::ControlType::e_edit_ctrl:
				{
					EditCtrl* pTextCtrl = dynamic_cast<EditCtrl*>(pWnd);
					assert(pTextCtrl);
					strText = pTextCtrl->GetValue().ToUTF8();
					bRet = true;
				}
				break;
			case why::ControlType::e_combobox_ctrl:
				{
					wxComboBox* pComboBox = dynamic_cast<wxComboBox*>(pWnd);
					assert(pComboBox);
					strText = std::to_string(pComboBox->GetCurrentSelection());
					bRet = true;
				}
				break;
			case why::ControlType::e_button_ctrl:
				{
					wxButton* pButtonCtrl = dynamic_cast<wxButton*>(pWnd);
					assert(pButtonCtrl);
					strText = pButtonCtrl->GetLabel().ToUTF8();
					bRet = true;
				}
				break;
			case why::ControlType::e_groupbox_ctrl:
				{
					wxPanel* pPanelCtrl = dynamic_cast<wxPanel*>(pWnd);
					assert(pPanelCtrl);
					strText = pPanelCtrl->GetLabel().ToUTF8();
					bRet = true;
				}
				break;
			case why::ControlType::e_image_label_ctrl:
				{
					ImageLabel* pImageLableCtrl = dynamic_cast<ImageLabel*>(pWnd);
					assert(pImageLableCtrl);
					strText = pImageLableCtrl->GetLabel().ToUTF8();
					bRet = true;
				}
				break;
			case why::ControlType::e_custom_combobox:
				{
					CustomComboBox* pComboBox = dynamic_cast<CustomComboBox*>(pWnd);
					assert(pComboBox);
					strText = std::to_string(pComboBox->GetCurrentData());
					bRet = true;
				}
				break;
			case why::ControlType::e_switch_button:
				{
					ImageSwitchButton* pSwitchButton = dynamic_cast<ImageSwitchButton*>(pWnd);
					assert(pSwitchButton);
					strText = std::to_string(pSwitchButton->GetUserData(pSwitchButton->IsChecked()));
					bRet = true;
				}
			break;
			default:
				break;
		}

		return bRet;
	}

	bool DataExchange::SetWindowText(ControlType eType, wxWindow* pWnd, const std::string& strText)
	{
		bool		bRet = false;

		switch (eType)
		{
			case why::ControlType::e_empty_ctrl:
				break;
			case why::ControlType::e_static_ctrl:
				{
					wxStaticText* pStaticText = dynamic_cast<wxStaticText*>(pWnd);
					assert(pStaticText);
					pStaticText->SetLabel(wxString::FromUTF8(strText));
					bRet = true;
				}
				break;
			case why::ControlType::e_edit_ctrl:
				{
					EditCtrl* pTextCtrl = dynamic_cast<EditCtrl*>(pWnd);
					assert(pTextCtrl);
					pTextCtrl->SetValue(wxString::FromUTF8(strText));
					bRet = true;
				}
				break;
			case why::ControlType::e_combobox_ctrl:
				{
					wxComboBox*			pComboBox = dynamic_cast<wxComboBox*>(pWnd);
					assert(pComboBox);

					uint32_t			uCount = pComboBox->GetCount();
					uint32_t			uCursel = (uint32_t)atoi(strText.c_str());

					if (uCursel < uCount)
						pComboBox->SetSelection(uCursel);

					bRet = true;
				}
				break;
			case why::ControlType::e_button_ctrl:
				{
					wxButton* pButtonCtrl = dynamic_cast<wxButton*>(pWnd);
					assert(pButtonCtrl);

					pButtonCtrl->SetLabel(wxString::FromUTF8(strText));
					bRet = true;
				}
				break;
			case why::ControlType::e_groupbox_ctrl:
				{
					wxPanel* pPanelCtrl = dynamic_cast<wxPanel*>(pWnd);
					assert(pPanelCtrl);
					pPanelCtrl->SetLabel(wxString::FromUTF8(strText));
					bRet = true;
				}
				break;
			case why::ControlType::e_image_label_ctrl:
				{
					ImageLabel* pImageLableCtrl = dynamic_cast<ImageLabel*>(pWnd);
					assert(pImageLableCtrl);
					pImageLableCtrl->SetLabel(wxString::FromUTF8(strText));
					bRet = true;
				}
				break;
			case why::ControlType::e_custom_combobox:
				{
					CustomComboBox*			pComboBox = dynamic_cast<CustomComboBox*>(pWnd);
					assert(pComboBox);
					uint32_t				uUserData = (uint32_t)atoi(strText.c_str());

					pComboBox->SetSelectionByUserData(uUserData);
					bRet = true;
				}
				break;
			case why::ControlType::e_switch_button:
				{
					ImageSwitchButton*		pSwitchButton = dynamic_cast<ImageSwitchButton*>(pWnd);
					assert(pSwitchButton);
					uint32_t				uUserData = (uint32_t)atoi(strText.c_str());

					pSwitchButton->SetCheck(pSwitchButton->MatchUserData(uUserData));
					bRet = true;
				}
			break;
			default:
				break;
		}

		return bRet;
	}

	std::vector<DataExchange::ChangedData> DataExchange::GetChangedDatas()
	{
		std::vector<ChangedData>			changedDatas;

		for (BindData& bindData : m_bindList)
		{
			if (bindData.m_strDomain.empty() || bindData.m_strName.empty())
				continue;

			for (BindWnd& bindWnd : bindData.m_list)
			{
				if (bindWnd.m_bChanged)
				{
					ChangedData			changedInfo;

					changedInfo.m_strDisplay = bindWnd.m_strDisplay;
					changedInfo.m_strDomain = bindData.m_strDomain + '|' + bindData.m_strName + '|' + bindWnd.m_strChildName;
					changedInfo.m_strOldData = bindWnd.m_strBaseValue;
					changedInfo.m_strNewData = bindWnd.m_strCurrentValue;
					changedDatas.push_back(changedInfo);
				}
			}
		}

		return changedDatas;
	}
}