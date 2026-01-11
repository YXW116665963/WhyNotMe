#include "grid_container.h"
#include "control_factory.h"

namespace why
{
	GridContainer::GridContainer(wxWindow* pWindow, const wxString &strName, const wxRect& rcRect, const GridLayOutCfg& tGridCfg)
		: wxPanel(pWindow, wxID_ANY, rcRect.GetLeftTop(), rcRect.GetSize(), wxTAB_TRAVERSAL| wxNO_BORDER, strName)
	{
		assert(0 != tGridCfg.m_uColumn);
		assert(0 != tGridCfg.m_uLine);

		m_rcRect = GetClientRect();
		m_uColumnWidth = m_rcRect.width / tGridCfg.m_uColumn;
		m_uLineHeight = m_rcRect.height / tGridCfg.m_uLine;
		m_tGridCfg = tGridCfg;
	}

	GridContainer::~GridContainer()
	{
		m_cells.clear();
	}

	void GridContainer::SetMargin(const int32_t leftMargin, const int32_t rightMargin, const int32_t topMargin, const int32_t bottomMargin)
	{
		m_tGridCfg.m_MarginLeft = leftMargin;
		m_tGridCfg.m_MarginRight = rightMargin;
		m_tGridCfg.m_MarginTop = topMargin;
		m_tGridCfg.m_MarginBottom = bottomMargin;
	}

	void GridContainer::GetMargin(int32_t& leftMargin, int32_t& rightMargin, int32_t& topMargin, int32_t& bottomMargin)
	{
		leftMargin = m_tGridCfg.m_MarginLeft;
		rightMargin = m_tGridCfg.m_MarginRight;
		topMargin = m_tGridCfg.m_MarginTop;
		bottomMargin = m_tGridCfg.m_MarginBottom;
	}

	void GridContainer::SetMaxLineHeight(uint32_t uLineHeight)
	{
		m_uMaxLineHeight = uLineHeight;
		AutoLayout(m_rcRect);
	}

	void GridContainer::SetMaxColumnWidth(uint32_t uColumnWidth)
	{
		m_uMaxColumnWidth = uColumnWidth;
		AutoLayout(m_rcRect);
	}

	wxWindow* GridContainer::SetCell(float64_t fColumnPos, float64_t fLinePos, float64_t fWidth, float64_t fHeight, ControlType eType, const wxString &strText, const wxString& strName)
	{
		CellItemPtr				itemPtr(new CellItem);
		wxRect					rcCell;
		wxWindow*				pCellControl{ nullptr };

		itemPtr->m_eType = eType;
		itemPtr->m_strName = strName;
		itemPtr->m_fColumnPos = fColumnPos;
		itemPtr->m_fLinePos = fLinePos;
		itemPtr->m_fWidth = fWidth;
		itemPtr->m_fHeight = fHeight;

		GetCellRect(fColumnPos, fLinePos, fWidth, fHeight, rcCell, (ControlType::e_groupbox_ctrl == eType));
		pCellControl = CreateControl(eType, this, wxID_ANY, rcCell, strName, "");
		if (!strText.IsEmpty())
			pCellControl->SetLabel(strText);
		
		itemPtr->m_pCellControl = pCellControl;
		m_cells.push_back(std::move(itemPtr));
		return pCellControl;
	}

	wxWindow* GridContainer::FindCell(const wxString& strName)
	{
		wxWindow				*pCellControl = nullptr;
		uint32_t				uCount = (uint32_t)m_cells.size();

		for (uint32_t i = 0; i < uCount; i++)
		{
			if (m_cells[i]->m_strName == strName)
			{
				pCellControl = m_cells[i]->m_pCellControl;
				break;
			}
		}

		return pCellControl;
	}

	void GridContainer::AutoLayout(const wxRect& rcRect)
	{
		uint32_t				uCount = (uint32_t)m_cells.size();
		wxRect					rcCell;
		uint32_t				uColumnWidth = 0;
		uint32_t				uLineHeight = 0;

		m_rcRect = rcRect;
		uColumnWidth = rcRect.width / m_tGridCfg.m_uColumn;
		if ((0 != m_uMaxColumnWidth) && (uColumnWidth > m_uMaxColumnWidth))
			uColumnWidth = m_uMaxColumnWidth;

		uLineHeight = rcRect.height / m_tGridCfg.m_uLine;
		if ((0 != m_uMaxLineHeight) && (uLineHeight > m_uMaxLineHeight))
			uLineHeight = m_uMaxLineHeight;

		if ((m_uColumnWidth != uColumnWidth) || (m_uLineHeight != uLineHeight))
		{
			m_uColumnWidth = uColumnWidth;
			m_uLineHeight = uLineHeight;

			for (uint32_t i = 0; i < uCount; i++)
			{
				if (m_cells[i]->m_pCellControl)
				{
					GetCellRect(m_cells[i]->m_fColumnPos, m_cells[i]->m_fLinePos, m_cells[i]->m_fWidth, m_cells[i]->m_fHeight, rcCell);
					m_cells[i]->m_pCellControl->SetPosition({ rcCell.x, rcCell.y });
					m_cells[i]->m_pCellControl->SetSize({ rcCell.width, rcCell.height });
				}
			}
		}
	}

	void GridContainer::ClearCells()
	{
		m_cells.clear();
	}

	void GridContainer::GetCellRect(float64_t fColumnPos, float64_t fLinePos, float64_t fWidth, float64_t fHeight, wxRect &rcCell, bool bIgnoreMargin)
	{
		rcCell.x = m_rcRect.x + (int32_t)(fColumnPos * m_uColumnWidth) + m_tGridCfg.m_uSpaceX;
		rcCell.y = m_rcRect.y + (int32_t)(fLinePos * m_uLineHeight) + m_tGridCfg.m_uSpaceY;
		rcCell.width = static_cast<int32_t>(fWidth * m_uColumnWidth) - 2*m_tGridCfg.m_uSpaceX;
		rcCell.height = static_cast<int32_t>(fHeight * m_uLineHeight) - 2*m_tGridCfg.m_uSpaceY;
	
		if (!bIgnoreMargin)
		{
			rcCell.x += m_tGridCfg.m_MarginLeft;
			rcCell.y += m_tGridCfg.m_MarginTop;
			rcCell.width -= (m_tGridCfg.m_MarginLeft + m_tGridCfg.m_MarginRight);
			rcCell.height -= (m_tGridCfg.m_MarginTop + m_tGridCfg.m_MarginBottom);
		}
	}
}