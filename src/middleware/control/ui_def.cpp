#include "ui_def.h"

namespace why
{
	namespace edit_style
	{
		const uint32_t g_uNumeric = (uint32_t)(1 << 28);
		const uint32_t g_uNoneBackgroud = (uint32_t)(1 << 29);
	}

	uint32_t				g_uLowLimitCode{ 0 };
	uint32_t				g_uHighLimitCode{ 0 };
	OnDataExceedLimit		g_fnDataExceedLimitNotify{ nullptr };
	LocalizationManager*	g_pLocalizationMgr{ nullptr };
}