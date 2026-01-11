#pragma once

#include "gui_import.h"
#include <array>

namespace why
{
	/**
	* @brief 网格控件容器
	* @details 主要目的是方便快速生成含有大量控件且又基本对齐的窗口
	*/
	class GridContainer : public wxPanel
	{
	public:
		GridContainer(wxWindow* pWindow, const wxString &strName, const wxRect& rcRect, const GridLayOutCfg& tGridCfg);
		~GridContainer();

	public:
		/**
		* @brief 获取控件显示区域四周的空白尺寸(单位：像素)
		* @return 留白尺寸
		*/
		void GetMargin(int32_t& leftMargin, int32_t& rightMargin, int32_t& topMargin, int32_t& bottomMargin);

		/**
		* @brief 设置控件显示区域四周的空白尺寸(单位：像素)
		* @param uMargin 新的留白尺寸
		* @return void
		*/
		void SetMargin(const int32_t leftMargin, const int32_t rightMargin, const int32_t topMargin, const int32_t bottomMargin);

		/**
		* @brief 设置最大行高(单位：像素)
		* @param uLineHeight 表格的行高
		* @return void
		* @warning 
		* - 此处设置的行高，为行高的最大值，仅用于限制自动布局产生的行高的上限
		* - 默认该值为0，表示不限制自动布局计算的行高
		*/
		void SetMaxLineHeight(uint32_t uLineHeight);

		/**
		* @brief 设置最大列宽(单位：像素)
		* @param uColumnWidth 表格的列宽
		* @return void
		* @warning 
		* - 此处设置的列宽，为列宽的最大值，仅用于限制自动布局产生的列宽的上限
		* - 默认该值为0，表示不限制自动布局计算的列宽
		*/
		void SetMaxColumnWidth(uint32_t uColumnWidth);

		/**
		* @brief 添加子控件
		* @param fColumnPos 子控件的起始列
		* @param fLinePos 子控件的起始行
		* @param fWidth 子控件的宽
		* @param fHeight 子控件的高
		* @param eType 子控件的类型
		* @param strText 子控件的显示文本
		* @param strName 子控件的标识符，只要在当前表格容器中唯一即可
		* @return 新创建子控件对象对象首地址
		* @warning 子控件的位置和尺寸的单位为行/列偏移
		*/
		wxWindow* SetCell(float64_t fColumnPos, float64_t fLinePos, float64_t fWidth, float64_t fHeight, ControlType eType, const wxString &strText, const wxString &strName);
		
		/**
		* @brief 查找子控件
		* @param strName 子控件的标识符，由SetCell时传入
		* @return 
		* - == nullptr 对应子控件不存在
		* - != nullptr 返回子控件的对象首地址
		*/
		wxWindow* FindCell(const wxString& strName);

		/**
		* @brief 依据新的显示区域重新排布子控件的显示位置
		* @param rcRect 网格容器的新的区域
		* @return
		*/
		void AutoLayout(const wxRect& rcRect);

		/**
		 * @brief 清空所有子控件
		 * @return void
		 */
		void ClearCells();

	private:
		void GetCellRect(float64_t fColumnPos, float64_t fLinePos, float64_t uWidth, float64_t uHeight, wxRect& rcCell, bool bIgnoreMargin = false);

		struct CellItem
		{
			ControlType					m_eType;
			wxString					m_strName;
			float64_t					m_fColumnPos;
			float64_t					m_fLinePos;
			float64_t					m_fHeight;
			float64_t					m_fWidth;
			wxWindow*					m_pCellControl;
		};
		typedef std::unique_ptr<CellItem> CellItemPtr;
		typedef std::vector<CellItemPtr> CellArray;

	private:
		CellArray						m_cells;
		wxRect							m_rcRect;
		uint32_t						m_uMaxColumnWidth{0};
		uint32_t						m_uMaxLineHeight{0};
		uint32_t						m_uColumnWidth{0};  // 列宽
		uint32_t						m_uLineHeight{0};   // 行高
		GridLayOutCfg                   m_tGridCfg;
	};

	typedef std::unique_ptr<GridContainer> GridContainerPtr;
}

