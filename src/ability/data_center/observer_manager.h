#pragma once

#include "standard.h"
#include "data_observer_interface.h"
#include "data_db.h"

#include <string>
#include <map>
#include <unordered_set>
#include <shared_mutex>

#include "procedure_call_interface.h"

namespace why
{
	class ObserverManager
	{
	public:
		ObserverManager(DataDB* pDataDB);
		~ObserverManager();

		/**
		* @brief 订阅数据。数据订阅：将回调存储在{ObserversPool}m_observers;
		* @param strDomain 数据的分类名称
		* @param strName 数据的名称
		* @param strMember 子元素的名称(仅对object类型的数据起效)
		* @param pObserver 观察者对象的接口
		* @return void
		* @warning
		* - 被订阅式的数据在订阅时可以还未被注册
		* - 当数据被注册的时候，如果数据有被订阅，会自动触发一次观察者回调的调用
		* - 订阅者必须保证提供的观察者对象在被取消订阅之前都是可用的。
		*/
		void Subscribe(const std::string& strDomain, const std::string& strName, const std::string& strMember, IDataObserver* pObserver);

		/**
		* @brief 取消对数据的订阅
		* @param strDomain 数据的分类名称
		* @param strName 数据的名称
		* @param pObserver 观察者对象的接口
		* @return void
		*/
		void Unsubscribe(const std::string& strDomain, const std::string& strName, IDataObserver* pObserver);

		/**
		* @brief 变量已经准备好
		* @param strDomain 数据的分类名称
		* @param strName 数据的名称
		* @param pNamedData 已经准备好的数据
		* @return void
		*/
		void OnValueReady(const std::string& strDomain, const std::string& strName, INamedData* pNamedData);

		/**
		* @brief 变量得值发生改变
		* @param strDomain 数据的分类名称
		* @param strName 数据的名称
		* @param pNamedData 发生变化的数据
		* @return void
		*/
		void OnValueChanged(const std::string& strDomain, const std::string& strName, const std::string& strMember, INamedData* pNamedData);

		/**
		* @brief 变量得值发生改变
		* @param strDomain 数据的分类名称
		* @param strName 数据的名称
		* @param pNamedData 发生变化的数据
		* @param pObserver 观察者对象的接口
		* @return void
		*/
		void OnValueChangedEx(const std::string& strDomain, const std::string& strName, const std::string& strMember, INamedData* pNamedData, IDataObserver* pObserver);
		/**
		* @brief 停止所有事件
		*/
		void Stop();
	private:
		typedef std::shared_lock<std::shared_mutex> ReadLockGuard;
		typedef std::unique_lock<std::shared_mutex> WriteLockGuard;

		struct ObserverInfo
		{
			std::string			m_strMember;
			IDataObserver* m_pObserver{ nullptr };
		};
		typedef std::list<ObserverInfo> ObserverList;
		typedef std::map<std::string, ObserverList> DomainDataObservers;
		typedef std::map<std::string, DomainDataObservers> ObserversPool;

		struct EventData
		{
			std::string		m_strDomain;
			std::string		m_strName;
			std::string		m_strMember;
			bool			m_bReady{ false };
			INamedData* m_pNamedData{ nullptr };
			IDataObserver* m_pObserver{ nullptr };
		};
		typedef std::map<uint32_t, EventData> EventMap;

		typedef std::vector<IDataObserver*> ObserverVector;
	private:
		void CallProcedure(uint32_t uParam);

	private:
		DataDB* m_pDataDB;
		mutable std::shared_mutex		m_lock;
		// 数据订阅：存储点3。
		// 值改变时查找对应的回调并执行
		ObserversPool					m_observers;
		IProcedureCall*					m_pProcedureCall{ nullptr };

		std::mutex						m_eventLock;
		uint32_t						m_uCurrentID{ 0 };
		// 数据订阅：存储点2。
		// 在每一次change中都会加入(EventMap)m_events中，然后执行之前再从m_events里删掉
		EventMap						m_events;
		bool							m_running{ true };
	};
}