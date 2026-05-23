#include "env_var_data_def.h"
#include "data_center_value_util.h"
#include "ui_loader.h"
#include "text_util.h"
#include "file_util.h"
#include "background.h"
#include "resource_manager.h"
#include "singleton.h"
#include "logger.h"
#include "control_factory.h"
#include "virtual_wnd_owner.h"
#include "common_control_loader.h"
#include "grid_container_loader.h"
#include "table_container_loader.h"
#include "util.h"

namespace why
{
	namespace
	{
		std::string							g_strConfigPath;		
		std::string							g_strConfigTargetPath;


		std::string	g_strPicturePath;
		std::string g_strXmlPath;
	}

	void TransformPoint(wxPoint& ptPos, const WndCoordinate& wndCoordinate)
	{
		ptPos.x = (int32_t)(ptPos.x * wndCoordinate.m_fScaleX);
		ptPos.y = (int32_t)(ptPos.y * wndCoordinate.m_fScaleY);
		ptPos.x -= wndCoordinate.m_ptOrigin.x;
		ptPos.y -= wndCoordinate.m_ptOrigin.y;
	}

	void TransformRect(wxRect& rcRect, const WndCoordinate& wndCoordinate)
	{
		rcRect.x = (int32_t)(rcRect.x * wndCoordinate.m_fScaleX);
		rcRect.y = (int32_t)(rcRect.y * wndCoordinate.m_fScaleY);
		rcRect.width = (int32_t)(rcRect.width * wndCoordinate.m_fScaleX);
		rcRect.height = (int32_t)(rcRect.height * wndCoordinate.m_fScaleY);

		rcRect.x -= wndCoordinate.m_ptOrigin.x;
		rcRect.y -= wndCoordinate.m_ptOrigin.y;
	}

	void SetLocalizationManager(LocalizationManager* pLocalizationMgr)
	{
		g_pLocalizationMgr = pLocalizationMgr;
	}

	void SetDataExceedLimitNotify(uint32_t uLowLimitCode, uint32_t uHighLimitCode, const OnDataExceedLimit& fnDataExceedLimitNotify)
	{
		g_uLowLimitCode = uLowLimitCode;
		g_uHighLimitCode = uHighLimitCode;
		g_fnDataExceedLimitNotify = fnDataExceedLimitNotify;
	}

	void SetUIResourcePath(const char* pConfigPath, const char* pPicturePath, const char* pResourceTargetPath)
	{
		g_strConfigPath = pConfigPath;
		g_strPicturePath = pPicturePath;
		g_strConfigTargetPath = pResourceTargetPath;
	}

	void LoadCommonControl(wxWindow* pParent, const WndCoordinate& wndCoordinate, DataExchange* pDataExchange, PanelGraphic* pPanelGraphic, CXmlNode* pNode, ControlType eType)
	{
		std::string			strName;
		wxRect				rcRect;
		std::string			strText;
		std::string			strStyle;
		bool				bVisible = true;
		wxWindow*			pControl = nullptr;

		GetAttributeText(pNode, "name", strName);
		GetAttributeText(pNode, "text", strText);
		GetAttributeText(pNode, "style", strStyle);
		bVisible = AttributeAsBool(pNode, "visible", true);

		if (!LoadWindowRect(pParent, pNode, rcRect))
			return;
		TransformRect(rcRect, wndCoordinate);

		//strText = g_pLocalizationMgr->ToLocalString(strText);
		pControl = CreateControl(eType, pParent, wxID_ANY, rcRect, strName, strStyle);
		
		if (nullptr != pControl)
		{
			pControl->SetLabel(wxString::FromUTF8(strText));
			LoadExtraInfo(pControl, eType, pPanelGraphic, pNode);
			if (pDataExchange)
				BindData(pControl, pDataExchange, pNode);
			pControl->Show(bVisible);
		}
	}

	void LoadVirtualWnd(VirtualWndOwner* pOwner, const WndCoordinate& wndCoordinate, CXmlNode* pNode, VirtualWndType eType)
	{
		wxSize				szOwner = pOwner->GetWindow()->GetSize();
		std::string			strName;
		wxRect				rcRect;
		std::string			strText;
		std::string			strStyle;
		bool				bVisible = true;
		std::shared_ptr<VirtualWnd> pVirtualWnd = nullptr;
	
		GetAttributeText(pNode, "name", strName);
		GetAttributeText(pNode, "text", strText);
		GetAttributeText(pNode, "style", strStyle);
		bVisible = AttributeAsBool(pNode, "visible", true);
		LoadWindowRect(wndCoordinate, pNode, rcRect);
		
		pVirtualWnd.reset(CreateVirtualWnd(eType, pOwner, rcRect, strName, strStyle));
		if (nullptr != pVirtualWnd)
		{
			pOwner->AddVirtualWnd(VirtualWndOwner::m_name_ui, pVirtualWnd);

			pVirtualWnd->SetLabel(wxString::FromUTF8(strText));
			LoadVirtualWndInfo(pVirtualWnd.get(), wndCoordinate, eType, pNode);
			if (bVisible)
				pVirtualWnd->Show(true);
		}
	}

	void LoadWindow(wxWindow* pParent, const WndCoordinate& wndCoordinate, DataExchange *pDataExchange, PanelGraphic* pPanelGraphic, CXmlNode* pNode)
	{
		const char*			lpName = nullptr;
		ControlType			eControlType = ControlType::e_empty_ctrl;

		//assert(nullptr != g_pLocalizationMgr);
		for (CXmlNode* pCur = pNode->first_node(); nullptr != pCur; pCur = pCur->next_sibling())
		{
			lpName = pCur->name();
			eControlType = GetControlType(lpName);
			if (ControlType::e_empty_ctrl == eControlType)
			{
				if (0 == _stricmp(lpName, "GridLayout"))
				{
					GridContainerLoader				loader(pDataExchange);

					loader.Load(pParent, pCur);
					continue;
				}

				if (0 == _stricmp(lpName, "TableLayout"))
				{
					TableContainerLoader			loader(pDataExchange);

					loader.Load(pParent, pCur);
					continue;
				}
			}
			else
			{
				LoadCommonControl(pParent, wndCoordinate, pDataExchange, pPanelGraphic, pCur, eControlType);
			}
		}
	}

	void LoadVirtualWindows(VirtualWndOwner* pOwner, const WndCoordinate& wndCoordinate, CXmlNode* pNode)
	{
		const char*			lpName = nullptr;
		VirtualWndType		eVirtualWndType = VirtualWndType::e_empty_ctrl;

		for (CXmlNode* pCur = pNode->first_node(); nullptr != pCur; pCur = pCur->next_sibling())
		{
			lpName = pCur->name();
			eVirtualWndType = GetVirtualWndType(lpName);
			if (VirtualWndType::e_empty_ctrl != eVirtualWndType)
				LoadVirtualWnd(pOwner, wndCoordinate, pCur, eVirtualWndType);
		}
	}

	void LoadPictureGraphic(const WndCoordinate& wndCoordinate, CXmlNode* pNode, PanelGraphic* pPanelGraphic)
	{
		std::string				strResName;
		std::string             strId = "";
		wxRect					rcRect;

		if (!GetAttributeText(pNode, "name", strResName))
			return;
		if (!GetAttributeRect(pNode, rcRect))
			return;
		TransformRect(rcRect, wndCoordinate);
		GetAttributeText(pNode, "id", strId);
		pPanelGraphic->AddPicture(strResName, rcRect, strId);

		bool bVisible = AttributeAsBool(pNode, "visible", true);
		if (!strId.empty() && false == bVisible)
			pPanelGraphic->ShowElement(strId, false);
	}

	void LoadBackgroundGraphic(const WndCoordinate& wndCoordinate, CXmlNode* pNode, PanelGraphic* pPanelGraphic)
	{
		std::string				strResName = "";
		std::string             strId = "";
		wxRect					rcRect;

		if (!GetAttributeText(pNode, "name", strResName))
			return;
		if (!GetAttributeRect(pNode, rcRect))
			return;
		TransformRect(rcRect, wndCoordinate);
		GetAttributeText(pNode, "id", strId);
		pPanelGraphic->AddBackground(strResName, rcRect, strId);

		bool bVisible = AttributeAsBool(pNode, "visible", true);
		if (!strId.empty() && false == bVisible)
			pPanelGraphic->ShowElement(strId, false);
	}

	void LoadLineGraphic(const WndCoordinate& wndCoordinate, CXmlNode* pNode, PanelGraphic* pPanelGraphic)
	{
		std::string				strResName;
		std::string             strId = "";
		wxPoint					ptStart;
		wxPoint					ptEnd;

		if (!GetAttributeText(pNode, "name", strResName))
			return;

		if (!GetAttributePoint(pNode, "start", ptStart))
			return;
		TransformPoint(ptStart, wndCoordinate);

		if (!GetAttributePoint(pNode, "end", ptEnd))
			return;
		TransformPoint(ptEnd, wndCoordinate);

		GetAttributeText(pNode, "id", strId);
		pPanelGraphic->AddLine(strResName, ptStart, ptEnd, strId);

		bool bVisible = AttributeAsBool(pNode, "visible", true);
		if (!strId.empty() && false == bVisible)
			pPanelGraphic->ShowElement(strId, false);
	}

	void LoadTextGraphic(const WndCoordinate& wndCoordinate, CXmlNode* pNode, PanelGraphic* pPanelGraphic)
	{
		std::string				strResName;
		std::string				strText;
		std::string             strId = "";
		wxColor					crColor;
		wxRect					rcText;
		uint32_t				uStyle;

		if (!GetAttributeText(pNode, "name", strResName))
			return;

		if (!GetAttributeColor(pNode, "color", crColor))
			return;

		if (!GetAttributeText(pNode, "text", strText))
			return;

		if (!GetAttributeRect(pNode, rcText))
			return;
		TransformRect(rcText, wndCoordinate);
		GetAttributeText(pNode, "id", strId);

		//assert(nullptr != g_pLocalizationMgr);
		//strText = g_pLocalizationMgr->ToLocalString(strText);
		if (GetTextAttributeStyle(pNode, "style", uStyle))
		{
			pPanelGraphic->AddText(strResName, crColor, wxString::FromUTF8(strText), rcText, strId, uStyle);
		}
		else
		{
			pPanelGraphic->AddText(strResName, crColor, wxString::FromUTF8(strText), rcText, strId);
		}
		
		bool bVisible = AttributeAsBool(pNode, "visible", true);
		if (!strId.empty() && false == bVisible)
			pPanelGraphic->ShowElement(strId, false);
	}

	void LoadPanelGraphic(const WndCoordinate &wndCoordinate, CXmlNode* pNode, PanelGraphic* pPanelGraphic)
	{
		const char*			lpName = nullptr;

		for (CXmlNode* pCur = pNode->first_node(); nullptr != pCur; pCur = pCur->next_sibling())
		{
			lpName = pCur->name();
	
			if (0 == _stricmp(lpName, "Picture"))
			{
				LoadPictureGraphic(wndCoordinate, pCur, pPanelGraphic);
				continue;
			}

			if (0 == _stricmp(lpName, "Background"))
			{
				LoadBackgroundGraphic(wndCoordinate, pCur, pPanelGraphic);
				continue;
			}
			
			if (0 == _stricmp(lpName, "Line"))
			{
				LoadLineGraphic(wndCoordinate, pCur, pPanelGraphic);
				continue;
			}

			if (0 == _stricmp(lpName, "Text"))
			{
				LoadTextGraphic(wndCoordinate, pCur, pPanelGraphic);
				continue;
			}
		}
	}

	bool LoadPanel(wxWindow* pParent, DataExchange* pDataExchange, const std::string& strFileName, PanelGraphic* pPanelGraphic)
	{
		PathAppender pathAppender;
		std::string	strFullFileName = pathAppender.
			SetSourcePath(GetStringValue(envVar::g_Domain, envVar::strUIXml_dirPath)).
			AppendChildPath(strFileName).
			GetPath();

		{
			std::string						strXMLFileName = UTF8ToLocal(strFullFileName);
			rapidxml::file<char>			fdoc(strXMLFileName.c_str());
			rapidxml::xml_document<char>	doc;
			CXmlNode*						pRoot = nullptr;
			CXmlNode*						pGraphic = nullptr;
			wxPoint							szSize{ 0,0 };
			WndCoordinate					wndCoordinate;

			try
			{
				doc.parse<0>(fdoc.data());
			}
			catch (const rapidxml::parse_error& ex)
			{
				LOG_ERROR << "invalidate xml file:" << strFileName << ",where:" << ex.where<char>();
				throw ex;
			}
			catch (const std::exception& e)
			{
				LOG_ERROR << "invalidate xml file:" << strFileName;
				throw e;
			}
			pRoot = doc.first_node();
			if (nullptr == pRoot)
			{
				LOG_INFO << "invalidate xml file:" << strFileName;
				return false;
			}

			GetAttributePoint(pRoot, "origin", wndCoordinate.m_ptOrigin);

			wndCoordinate.m_szOrigin = pParent->GetSize();
			if (GetAttributePoint(pRoot, "size", szSize))
			{
				if ((0 == wndCoordinate.m_szOrigin.x) || (0 == wndCoordinate.m_szOrigin.y))
				{
					wndCoordinate.m_fScaleX = 1.0;
					wndCoordinate.m_fScaleY = 1.0;
					wndCoordinate.m_szOrigin.x = szSize.x;
					wndCoordinate.m_szOrigin.y = szSize.y;
					pParent->SetClientSize(szSize.x, szSize.y);
				}
				else
				{
					wndCoordinate.m_fScaleX = wndCoordinate.m_szOrigin.x * 1.0 / szSize.x;
					wndCoordinate.m_fScaleY = wndCoordinate.m_szOrigin.y * 1.0 / szSize.y;
					wndCoordinate.m_szOrigin.x = szSize.x;
					wndCoordinate.m_szOrigin.y = szSize.y;
				}
			}

			if (nullptr != pPanelGraphic)
			{
				pGraphic = pRoot->first_node("Graphic");
				if (nullptr != pGraphic)
					LoadPanelGraphic(wndCoordinate, pGraphic, pPanelGraphic);
			}
			
			uint64_t		uStart = GetTimeStamp64();
			uint64_t		uTimeUsed = 0;

			LoadWindow(pParent, wndCoordinate, pDataExchange, pPanelGraphic, pRoot);
			uTimeUsed = GetTimeStamp64() - uStart;
			if (uTimeUsed > 50)
				LOG_WARNING << "LoadWindow[" << strFileName << "] time:" << GetTimeStamp64() - uStart;
		}
		return true;
	}

	bool LoadVirtualWnd(VirtualWndOwner* pOwner, const std::string& strFileName)
	{
		assert(nullptr != pOwner);	
		std::string	strFullFileName = GetStringValue(envVar::g_Domain, envVar::strUIXml_dirPath) + strFileName;

		{
			std::string						strXMLFileName = UTF8ToLocal(strFullFileName);
			rapidxml::file<char>			fdoc(strXMLFileName.c_str());
			rapidxml::xml_document<char>	doc;
			CXmlNode*						pRoot = nullptr;
			WndCoordinate					wndCoordinate;
			wxPoint							szSize{ 0,0 };

			try
			{
				doc.parse<0>(fdoc.data());
			}
			catch (const rapidxml::parse_error& ex)
			{
				LOG_ERROR << "invalidate xml file:" << strFullFileName << ",where:" << ex.where<char>();
				throw ex;
			}
			catch (const std::exception& e)
			{
				LOG_ERROR << "invalidate xml file:" << strFullFileName;
				throw e;
			}
			pRoot = doc.first_node();
			if (nullptr == pRoot)
			{
				LOG_INFO << "invalidate xml file:" << strFileName;
				return false;
			}

			GetAttributePoint(pRoot, "origin", wndCoordinate.m_ptOrigin);
			wndCoordinate.m_szOrigin = pOwner->GetWindow()->GetSize();
			if (GetAttributePoint(pRoot, "size", szSize))
			{
				wndCoordinate.m_fScaleX = wndCoordinate.m_szOrigin.x*1.0 / szSize.x;
				wndCoordinate.m_fScaleY = wndCoordinate.m_szOrigin.y*1.0 / szSize.y;

				wndCoordinate.m_szOrigin.x = szSize.x;
				wndCoordinate.m_szOrigin.y = szSize.y;
			}
			LoadVirtualWindows(pOwner, wndCoordinate, pRoot);
		}
		return true;
	}
	
}