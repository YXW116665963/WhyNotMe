#include "table_container.h"
#include "control_factory.h"

namespace why
{
	TableContainer::TableContainer(wxWindow* pWindow, const wxString& strName, const wxRect& rcRect)
			: wxPanel(pWindow, wxID_ANY, rcRect.GetLeftTop(), rcRect.GetSize(), wxTAB_TRAVERSAL | wxNO_BORDER, strName)
			, m_rcRect(rcRect)
	{
		
	}

	TableContainer::~TableContainer()
	{
		m_cells.clear();
		m_columns.clear();
	}

	void TableContainer::SetMargin(uint32_t uMargin)
	{
		m_uMargin = uMargin;
	}

	void TableContainer::SetHeaderHeight(uint32_t uHeight)
	{
		m_uHeaderHeight = uHeight;
	}

	void TableContainer::SetLineHeight(uint32_t uHeight)
	{
		m_uLineHeight = (int32_t)uHeight;
	}

	uint32_t TableContainer::ColumnCount()
	{
		return (uint32_t)m_columns.size();
	}

	uint32_t TableContainer::AddColumn(ControlType eType, uint32_t uWidth, uint32_t uHeight, const wxString& strText)
	{
		uint32_t					uSize = (uint32_t)m_columns.size();
		ColumnItemPtr				columnItemPtr = CreateColumnItem(eType, uSize, uWidth, uHeight, strText);

		m_columns.push_back(std::move(columnItemPtr));
		return uSize;
	}

	void TableContainer::RemoveColumn(int32_t uColumnPos)
	{
		int32_t						nSize = (int32_t)m_columns.size();

		if (uColumnPos < nSize)
		{
			ColumnArray::iterator			itRemove = m_columns.begin();

			m_columns.erase(itRemove);
		}
	}

	wxWindow* TableContainer::SetCell(int32_t nLinePos, int32_t nColumnPos, const wxString& strText, const wxString& strName)
	{
		wxWindow*				pReturn = nullptr;

		if ((0 != strText.length()) || (strName.empty()))
		{
			int32_t				nColumns = (int32_t)m_columns.size();

			if (nColumnPos < nColumns)
			{
				CellItemPtr				itemPtr(new CellItem);
				wxRect					rcCell;

				itemPtr->m_strName = strName;
				itemPtr->m_nColumn = nColumnPos;
				itemPtr->m_nLine = nLinePos;

				GetCellRect(nColumnPos, nLinePos, rcCell);
				itemPtr->m_pCellControl = CreateControl(m_columns[nColumnPos]->m_eType, this, wxID_ANY, rcCell, strName, "");
				if (!strText.IsEmpty())
					itemPtr->m_pCellControl->SetLabel(strText);
				m_cells.push_back(std::move(itemPtr));

				if (nLinePos > (int32_t)m_uLineCount)
					m_uLineCount = (uint32_t)nLinePos;
			}
		}
		return pReturn;
	}

	wxWindow* TableContainer::FindCell(const wxString& strName)
	{
		wxWindow*			pWnd = nullptr;
		uint32_t			uCellCount = (uint32_t)m_cells.size();

		for (uint32_t i = 0; i < uCellCount; i++)
		{
			if (m_cells[i]->m_strName == strName)
			{
				pWnd = m_cells[i]->m_pCellControl;
				break;
			}
		}

		return pWnd;

	}

	TableContainer::ColumnItemPtr TableContainer::CreateColumnItem(ControlType eType, uint32_t uColumnPos, uint32_t uWidth, uint32_t uHeight, const wxString& strText)
	{
		ColumnItemPtr			itemPtr(new ColumnItem);
		wxRect					rcHeader;

		itemPtr->m_eType = eType;
		itemPtr->m_uWidth = uWidth;
		itemPtr->m_uHeight = uHeight;
		
		if (0 != m_uHeaderHeight)
		{
			GetHeaderRect(uColumnPos, uWidth, rcHeader);
			itemPtr->m_pHeader = dynamic_cast<wxStaticText *>(CreateControl(ControlType::e_static_ctrl, this, -1, rcHeader, wxEmptyString, ""));
			if (!strText.IsEmpty())
				itemPtr->m_pHeader->SetLabel(strText);
		}
		return itemPtr;
	}

	void TableContainer::GetHeaderRect(uint32_t uColumn, uint32_t uWidth, wxRect& rcItem)
	{
		rcItem.x = m_rcRect.x;
		for (uint32_t i = 0; i < uColumn; i++)
			rcItem.x += m_columns[i]->m_uWidth;
		rcItem.x += m_uMargin;
		rcItem.width = uWidth - m_uMargin*2;

		rcItem.y = m_rcRect.y + m_uMargin;
		rcItem.height = m_uHeaderHeight - m_uMargin*2;
	}

	void TableContainer::GetCellRect(uint32_t uColumn, uint32_t uLine, wxRect& rcItem)
	{
		rcItem.x = m_rcRect.x;
		for (uint32_t i = 0; i < uColumn; i++)
			rcItem.x += m_columns[i]->m_uWidth;
		rcItem.x += m_uMargin;
		rcItem.width = m_columns[uColumn]->m_uWidth - m_uMargin * 2;

		rcItem.y = m_rcRect.y + m_uHeaderHeight + uLine * m_uLineHeight;
		rcItem.height = m_uLineHeight - m_uMargin * 2;
	}

	void TableContainer::AutoLayout(const wxRect& rcRect)
	{
		uint32_t				uCellCount = 0;
		wxRect					rcCell;

		m_rcRect = rcRect;
		if (0 != m_uHeaderHeight)
		{
			uint32_t				uColumnCount = (uint32_t)m_columns.size();

			for (uint32_t i = 0; i < uColumnCount; i++)
			{
				if (nullptr != m_columns[i]->m_pHeader)
				{
					GetHeaderRect(i, m_columns[i]->m_uWidth, rcCell);
					m_columns[i]->m_pHeader->SetPosition({ rcCell.x, rcCell.y });
					m_columns[i]->m_pHeader->SetSize({ rcCell.width, rcCell.height });
				}
			}
		}

		uCellCount = (uint32_t)m_cells.size();
		for (uint32_t i = 0; i < uCellCount; i++)
		{
			if (nullptr != m_cells[i]->m_pCellControl)
			{
				GetCellRect(m_cells[i]->m_nColumn, m_cells[i]->m_nLine, rcCell);
				m_cells[i]->m_pCellControl->SetPosition({ rcCell.x, rcCell.y });
				m_cells[i]->m_pCellControl->SetSize({ rcCell.width, rcCell.height });
			}
		}
	}
}
