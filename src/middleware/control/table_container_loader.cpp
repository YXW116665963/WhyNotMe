#include "table_container_loader.h"
#include "common_control_loader.h"
#include "control_factory.h"

namespace why
{
	TableContainerLoader::TableContainerLoader(DataExchange* pDataExchange)
				: m_pDataExchange(pDataExchange)
	{

	}

	TableContainerLoader::~TableContainerLoader()
	{

	}

	bool TableContainerLoader::Load(wxWindow* pParent, CXmlNode* pNode)
	{
		std::string			strName;
		wxRect				rcRect;
		const char*			lpName = nullptr;
		ControlType			eType = ControlType::e_empty_ctrl;
		int32_t				nColumnPos;
		int32_t				nLinePos;
		std::string			strText;
		wxWindow* pCell = nullptr;

		GetAttributeText(pNode, "name", strName);
		if (!LoadWindowRect(pParent, pNode, rcRect))
			return false;

		m_pTableContainer = new TableContainer(pParent, strName, rcRect);
		for (CXmlNode* pCur = pNode->first_node(); nullptr != pCur; pCur = pCur->next_sibling())
		{
			lpName = pCur->name();
			eType = GetControlType(lpName);
			if (ControlType::e_empty_ctrl != eType)
			{
				LoadCellPos(pCur, nColumnPos, nLinePos);
				GetAttributeText(pCur, "name", strName);
				GetAttributeText(pCur, "text", strText);
				pCell = m_pTableContainer->SetCell(nColumnPos, nLinePos, wxString::FromUTF8(strText), wxString::FromUTF8(strName));
				LoadExtraInfo(pCell, eType, nullptr, pCur);
				if (nullptr != m_pDataExchange)
					BindData(pCell, m_pDataExchange, pCur);
			}
		}
		return true;
	}
}