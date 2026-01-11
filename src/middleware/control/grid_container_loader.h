#pragma once

#include "gui_import.h"

#include <string>

#include "xml_util.h"
#include "data_exchange.h"
#include "grid_container.h"
#include "image_button.h"
#include "image_label.h"

namespace why
{
	class GridContainerLoader
	{
	public:
		GridContainerLoader(DataExchange *pDataExchange);
		~GridContainerLoader();

		bool Load(wxWindow* pParent, CXmlNode* pNode);

	private:
		DataExchange*		m_pDataExchange;
		GridContainer*		m_pGridContainer{ nullptr };
	};
}

