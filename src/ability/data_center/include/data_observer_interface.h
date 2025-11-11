#pragma once

#include "standard.h"
#include "named_data_interface.h"

namespace why
{
	/**
	* @brief 定义命名数据的观察者接口
	*/
	class IDataObserver
	{
	public:
		/**
		* @brief 命名数据的值发生改变
		* @param pData 发生改变的命名数据接口
		* @return void
		*/
		virtual void OnDataChanged(INamedData* pData) = 0;
	};
}