#pragma once

#include "named_data_interface.h"
#include "value_watcher_interface.h"
#include "observer_manager.h"

#include <functional>
#include <memory>
#include <set>

namespace why
{
	class NamedData : public INamedData, public IValueWatcher
	{
	public:
		NamedData(ObserverManager* pObserverMgr, const std::string& strDomain, const std::string& strName, DataStyle eStyle);
		~NamedData();

	public:
		//INamedData interface
		const char* Domain() const override;
		const char* Name() const override;
		DataStyle GetStyle() const override { return m_eStyle; }
		IDataValue* GetValue() override;
		void PauseNotify() override;
		void ResumeNotify() override;
		bool Assign(INamedData* pOther, bool& changed) override;

		//IValueWatcher interface
		void Pause() override;
		void Resume() override;
		// 各种类型的Value值变化执行的回调
		void OnValueChanged(IDataValue* pDataValue) override;

	public:
		// 数据订阅：值改变的回调传递。
		// NamedData实现IValueWatcher接口，然后创建BigIntergerValue时传入this指针，进而实现Value类型的IValueWatcher接口。
		bool CreateIntValue(int32_t nDefault);
		bool CreateUIntValue(uint32_t uDefault);
		bool CreateBigIntValue(int64_t nDefault);
		bool CreateBigUIntValue(uint64_t uDefault);
		bool CreateFloatValue(float64_t fDefault);
		bool CreateStringValue(const std::string& strDefaultt);
		IListDataValue* CreateListValue(ValueType eType, uint32_t uSize, const std::string& strDefault);
		IObjectDataValue* CreateObjectValue();
		void SetReady();
		bool CreateValue(NamedData* pOtherData);

	private:
		typedef std::function<void(IDataValue*)> DataValueDeleter;
		typedef std::unique_ptr<IDataValue, DataValueDeleter> DataValuePtr;

	private:
		IListDataValue* CreateIntListValue(uint32_t uSize, const std::string& strDefault);
		IListDataValue* CreateUIntListValue(uint32_t uSize, const std::string& strDefault);
		IListDataValue* CreateBigIntListValue(uint32_t uSize, const std::string& strDefault);
		IListDataValue* CreateBigUIntListValue(uint32_t uSize, const std::string& strDefault);
		IListDataValue* CreateFloatListValue(uint32_t uSize, const std::string& strDefault);
		IListDataValue* CreateStringListValue(uint32_t uSize, const std::string& strDefault);
		std::string GetMemberName(IDataValue* pDataValue);

	private:
		typedef std::unique_lock<std::shared_mutex> WriteLockGuard;

	private:
		ObserverManager* m_pObserverMgr;
		std::string						m_strDomain;
		std::string						m_strName;
		DataStyle						m_eStyle;
		DataValuePtr					m_value;
		bool							m_bReady{ false };
		bool							m_bPauseNotify{ false };
		bool							m_bChanged{ false };
		uint32_t						m_uWatchPausRef{ 0 };
		std::set<std::string>			m_members;
		mutable std::shared_mutex		m_lock;
	};
}