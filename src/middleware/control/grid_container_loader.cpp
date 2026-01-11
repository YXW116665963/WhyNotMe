#include "grid_container_loader.h"
#include "common_control_loader.h"
#include "control_factory.h"
#include <sstream>

namespace why
{
	GridContainerLoader::GridContainerLoader(DataExchange* pDataExchange)
		: m_pDataExchange(pDataExchange)
	{

	}

	GridContainerLoader::~GridContainerLoader()
	{

	}

	bool GridContainerLoader::Load(wxWindow* pParent, CXmlNode* pNode)
	{
		GridLayOutCfg               tGridCfg;
		tGridCfg.m_uColumn =    AttributeAsUInt(pNode, "column", 1);
		tGridCfg.m_uLine =      AttributeAsUInt(pNode, "line", 1);
		tGridCfg.m_MarginLeft = AttributeAsInt(pNode, "leftMargin", 0);
		tGridCfg.m_MarginRight = AttributeAsInt(pNode, "rightMargin", 0);
		tGridCfg.m_MarginTop =  AttributeAsInt(pNode, "topMargin", 0);
		tGridCfg.m_MarginBottom = AttributeAsInt(pNode, "bottomMargin", 0);
		tGridCfg.m_uSpaceX =    AttributeAsUInt(pNode, "xSpace", 2);
		tGridCfg.m_uSpaceY =  AttributeAsUInt(pNode, "ySpace", 2);
		
		const char*					lpName = nullptr;
		ControlType					eType = ControlType::e_empty_ctrl;
		float64_t					fColumnPos;
		float64_t					fLinePos;
		float64_t					fWidth;
		float64_t					fHeight;
		std::string					strText;
		wxWindow*					pCell = nullptr;
		wxRect						rcRect;
		std::string					strName;
		GetAttributeText(pNode, "name", strName);
		if (!LoadWindowRect(pParent, pNode, rcRect))
			return false;

		m_pGridContainer = new GridContainer(pParent, strName, rcRect, tGridCfg);
		for (CXmlNode* pCur = pNode->first_node(); nullptr != pCur; pCur = pCur->next_sibling())
		{
			lpName = pCur->name();
			eType = GetControlType(lpName);
			if (ControlType::e_empty_ctrl != eType)
			{
				LoadCellRect(pCur, fColumnPos, fLinePos, fWidth, fHeight);
				GetAttributeText(pCur, "name", strName);
				GetAttributeText(pCur, "text", strText);
				pCell = m_pGridContainer->SetCell(fColumnPos, fLinePos, fWidth, fHeight, eType, wxString::FromUTF8(strText), wxString::FromUTF8(strName));
				LoadExtraInfo(pCell, eType, nullptr, pCur);
				if (nullptr != m_pDataExchange)
					BindData(pCell, m_pDataExchange, pCur);
			}
		}
		return true;
	}
}