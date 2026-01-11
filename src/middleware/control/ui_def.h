#pragma once

#include "standard.h"
#include "localization_manager.h"
#include <functional>

namespace why
{
	namespace edit_style
	{
		// 使用较高的位，避免和系统已定义值冲突
		extern const uint32_t g_uNumeric;
		extern const uint32_t g_uNoneBackgroud;
	}

	typedef std::function<void(uint32_t uCode, const std::string& strPrefix, const std::string& strCurrentValue, const std::string& strLimitValue)> OnDataExceedLimit;

	extern uint32_t				g_uLowLimitCode;
	extern uint32_t				g_uHighLimitCode;
	extern OnDataExceedLimit	g_fnDataExceedLimitNotify;
	extern LocalizationManager* g_pLocalizationMgr;
}
