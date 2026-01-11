#pragma once

#include "gui_import.h"

namespace why
{
	class TableContainer : public wxPanel
	{
	public:
		TableContainer(wxWindow* pWindow, const wxString &strName, const wxRect& rcRect);
		~TableContainer();

	public:
		uint32_t GetMargin() { return m_uMargin; }
		void SetMargin(uint32_t uMargin);

		uint32_t GetHeaderHeight() { return m_uHeaderHeight; }
		void SetHeaderHeight(uint32_t uHeight);

		uint32_t GetLineHeight() { return m_uLineHeight; }
		void SetLineHeight(uint32_t uHeight);

		uint32_t ColumnCount();
		uint32_t AddColumn(ControlType eType, uint32_t uWidth, uint32_t uHeight, const wxString& strText);
		void RemoveColumn(int32_t uColumnPos);

		wxWindow* SetCell(int32_t nLinePos, int32_t nColumnPos, const wxString& strText, const wxString& strName);
		wxWindow* FindCell(const wxString& strName);

		void AutoLayout(const wxRect& rcRect);

	private:
		struct ColumnItem
		{
			ControlType			m_eType{ ControlType::e_empty_ctrl };
			uint32_t			m_uWidth{ 0 };
			uint32_t			m_uHeight{ 0 };
			wxStaticText*		m_pHeader{nullptr};
		};
		typedef std::unique_ptr<ColumnItem> ColumnItemPtr;
		typedef std::vector<ColumnItemPtr> ColumnArray;

		struct CellItem
		{
			wxString			m_strName;
			int32_t				m_nColumn{ 0 };
			int32_t				m_nLine{ 0 };
			wxWindow*			m_pCellControl{ nullptr };
		};
		typedef std::unique_ptr<CellItem> CellItemPtr;
		typedef std::vector<CellItemPtr> CellArray;

	private:
		ColumnItemPtr CreateColumnItem(ControlType eType, uint32_t uColumnPos, uint32_t uWidth, uint32_t uHeight, const wxString& strText);
		void GetHeaderRect(uint32_t uColumn, uint32_t uWidth, wxRect& rcItem);
		void GetCellRect(uint32_t uColumn, uint32_t uLine, wxRect& rcItem);

	private:
		ColumnArray						m_columns;
		CellArray						m_cells;
		wxRect							m_rcRect;
		uint32_t						m_uHeaderHeight{28};
		uint32_t						m_uLineHeight{32};
		uint32_t						m_uMargin{ 2 };
		uint32_t						m_uLineCount { 0 } ;
	};

	typedef std::unique_ptr<TableContainer> TableContainerPtr;
}
