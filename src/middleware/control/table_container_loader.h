#pragma once

#include "gui_import.h"

#include <string>

#include "xml_util.h"
#include "resource_loader.h"
#include "data_exchange.h"
#include "table_container.h"

namespace why
{
	class TableContainerLoader
	{
	public:
		TableContainerLoader(DataExchange* pDataExchange);
		~TableContainerLoader();

		bool Load(wxWindow* pParent, CXmlNode* pNode);

	private:
		DataExchange*			m_pDataExchange;
		TableContainer*			m_pTableContainer{ nullptr };
	};
}

