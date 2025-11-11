#pragma once

#include "data_center_def.h"
#include "data_value_interface.h"

namespace why
{
	class INamedData
	{
	public:
		/**
		* @brief 获取命名数据的分类名称
		* @return 命名数据分类名称的首地址
		*/
		virtual const char* Domain() const = 0;

		/**
		* @brief 获取命名数据的名称
		* @return 命名数据名称的首地址
		*/
		virtual const char* Name() const = 0;

		/**
		* @brief 获取命名数据的风格
		* @return 命名数据名称的风格
		*/
		virtual DataStyle GetStyle() const = 0;

		/**
		* @brief 获取命名数据的值对象
		* @return 返回值对象的首地址
		* @warning 返回的值对象的首地址永不为空，值对象的内存由命名数据分配和负责回收
		*/
		virtual IDataValue* GetValue() = 0;

		/**
		* @brief 暂停发出通知
		* @return void
		* @warning 暂停发出通知后，数据的值发生变更后，将不会触发数据观察者回调。
		*/
		virtual void PauseNotify() = 0;

		/**
		* @brief 恢复发出通知
		* @return void
		* @warning
		* - 恢复发出通知，如果从之前的暂停通知后，数据的值有发生变化，恢复动作会立即补发一次观察者回调
		* - 恢复后，后续再有数组变化，将实时发出观察者回调
		*/
		virtual void ResumeNotify() = 0;

		/**
		* @brief 将一个数据的值赋值给当前数据
		* @param pOther 数据值的来源，pOther的值类型必须和当前数据的值类型一致，否则赋值失败
		* @return
		* - true 赋值成功
		* - false 赋值失败
		*/
		virtual bool Assign(INamedData* pOther, bool& changed) = 0;
	};
}