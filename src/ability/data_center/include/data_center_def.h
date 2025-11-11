#pragma once

#include "standard.h"

namespace why
{
	/**
	 * @brief 命名数据的风格
	 */
	enum class DataStyle
	{
		eNormal,			//!< 内存类型，也是命名数据的默认类型
		ePersistence,		//!< 持久化，此种类型的数据，值发生改变后，会自动更新到数据库
	};

	/**
	 * @brief 数据值的类型
	 */
	enum class ValueType
	{
		eInt,				//!< 32位整型数(有符号)
		eUInt,				//!< 32位整型数(无符号)
		eBigInt,			//!< 64位整型数(有符号)
		eBigUInt,			//!< 64位整型数(无符号)
		eFloat,				//!< 双精度浮点数
		eString,			//!< 字符串
		eList,				//!< 数组类型
		eObject				//!< 对象类型
	};
}