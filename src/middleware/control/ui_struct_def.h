#pragma once

#include "standard.h"

namespace why
{
	enum class VirtualWndType
	{
		e_empty_ctrl,
		e_label_ctrl,
		e_image_ctrl,
		e_button_ctrl,
		e_figure_ctrl,
		e_progress_bar_ctrl
	};

	namespace ui
	{
		enum class CameraPos
		{
			eNone = 0,
			eLeftTop = 1,
			eRightTop = 2,
			eLeftBottom = 3,
			eRightBottom = 4,
			eFull        = 5,
		};
	}
}
