#pragma once

#include "standard.h"
namespace why
{
	/**
	* @brief 值观察器
	*/
	class IValueWatcher
	{
	public:
		/**
		* @brief 暂停处理值变化通知
		* @return void
		*/
		virtual void Pause() = 0;

		/**
		* @brief 恢复处理值变化通知
		* @return void
		*/
		virtual void Resume() = 0;

		/**
		* @brief 值发生变化
		* @return
		*/
		virtual void OnValueChanged(IDataValue* pDataValue) = 0;
	};
}