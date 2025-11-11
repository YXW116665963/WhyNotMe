#pragma once

#include "standard.h"

#pragma warning(push)
#pragma warning(disable:4996)
#include "wx/wx.h"
#include "wx/event.h"
#include "wx/rawbmp.h"
#include "wx/tokenzr.h"
#include "wx/panel.h"
#include "wx/treectrl.h"
#include "wx/radiobut.h"
#include "wx/dataview.h"
#include "wx/display.h"
#include "wx/dcbuffer.h"
#include "wx/graphics.h"
#include "wx/numformatter.h"
#include "wx/file.h"
#include "wx/stdpaths.h"
#pragma warning(pop)

namespace why
{
	enum class ControlType
	{
		e_empty_ctrl,
		e_static_ctrl,
		e_edit_ctrl,
		e_combobox_ctrl,
		e_button_ctrl,
		e_groupbox_ctrl,
		e_checkbox_ctrl,
		e_radiobtn_ctrl,
		e_listbox_ctrl,
		e_tree_ctrl,
		e_progress_bar_ctrl,
		e_table_ctrl,
		e_image_button_ctrl,
		e_image_label_ctrl,
		e_custom_combobox,
		e_switch_button,
	};

	struct GridLayOutCfg
	{
		uint32_t	m_uColumn{0};     // 列
		uint32_t	m_uLine{0};       // 行
		int32_t     m_MarginLeft{0};  // 左边距
		int32_t     m_MarginRight{0}; // 右边距
		int32_t     m_MarginTop{0};   // 上
		int32_t     m_MarginBottom{0};// 下
		uint32_t    m_uSpaceX{2};     // x方向间隔
		uint32_t    m_uSpaceY{2};     // y方向间隔
	};
}
