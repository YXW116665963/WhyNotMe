#pragma once

#include "standard.h"
#include "data_observer_interface.h"
#include "data_center_interface.h"
#include "util.h"

#include <map>
#include <memory>
#include <functional>

namespace why
{
	typedef std::function<void(INamedData* pINamedData)> DataObserver;
	typedef std::function<void(IDataValue* pDataValue)> ValueObserver;
	typedef std::function<void(IObjectDataValue* pObjectDataValue)> ObjectValueObserver;
	typedef std::function<void(IListDataValue* pListDataValue)> ListValueObserver;

	class DataObserverWrapper : public IDataObserver
	{
	public:
		DataObserverWrapper(const DataObserver& onChangedMethod)
			: m_onChangedMethod(onChangedMethod)
		{

		}

		void OnDataChanged(INamedData* pData)
		{
			if (m_onChangedMethod)
				m_onChangedMethod(pData);
		}

	private:
		DataObserver			m_onChangedMethod;
	};

	class ValueObserverWrapper : public IDataObserver
	{
	public:
		ValueObserverWrapper(const ValueObserver& onChangedMethod)
			: m_onChangedMethod(onChangedMethod)
		{

		}

		void OnDataChanged(INamedData* pData)
		{
			if (m_onChangedMethod)
				m_onChangedMethod(pData->GetValue());
		}

	private:
		ValueObserver			m_onChangedMethod;
	};

	class ObjectValueObserverWrapper : public IDataObserver
	{
	public:
		ObjectValueObserverWrapper(const ObjectValueObserver& onChangedMethod)
			: m_onChangedMethod(onChangedMethod)
		{

		}

		void OnDataChanged(INamedData* pData)
		{
			if (m_onChangedMethod)
			{
				IObjectDataValue* pObjectDataValue = dynamic_cast<IObjectDataValue*>(pData->GetValue());

				m_onChangedMethod(pObjectDataValue);
			}
		}

	private:
		ObjectValueObserver			m_onChangedMethod;
	};

	class ListValueObserverWrapper : public IDataObserver
	{
	public:
		ListValueObserverWrapper(const ListValueObserver& onChangedMethod)
			: m_onChangedMethod(onChangedMethod)
		{

		}

		void OnDataChanged(INamedData* pData)
		{
			if (m_onChangedMethod)
			{
				IListDataValue* pObjectDataValue = dynamic_cast<IListDataValue*>(pData->GetValue());

				m_onChangedMethod(pObjectDataValue);
			}
		}

	private:
		ListValueObserver			m_onChangedMethod;
	};

	class DataObserverHelper
	{
	public:
		DataObserverHelper(IDataCenter* pDataCenter) : m_pCenter(pDataCenter)
		{

		}

		~DataObserverHelper()
		{
			if (m_pCenter)
			{
				for (auto& observerInfo : m_subscribedDatas)
					m_pCenter->Unsubscribe(observerInfo.first.first.c_str(), observerInfo.first.second.c_str(), observerInfo.second.get());
			}
			m_subscribedDatas.clear();
		}

	public:
		void Subscribe(const std::string& strDomain, const std::string strName, const DataObserver& fnObserver)
		{
			DataID				dataID = std::make_pair(strDomain, strName);
			auto				itFind = m_subscribedDatas.find(dataID);

			if (itFind == m_subscribedDatas.end())
			{
				DataObserverWrapper* pDataObserverWrapper = new DataObserverWrapper(fnObserver);

				m_subscribedDatas.insert(std::make_pair(dataID, ObserverWrapperPtr(pDataObserverWrapper, InterfaceDeleter<DataObserverWrapper, IDataObserver>())));
				m_pCenter->Subscribe(strDomain.c_str(), strName.c_str(), pDataObserverWrapper);
			}
		}

		void SubscribeValue(const std::string& strDomain, const std::string strName, const ValueObserver& fnObserver)
		{
			DataID				dataID = std::make_pair(strDomain, strName);
			auto				itFind = m_subscribedDatas.find(dataID);

			if (itFind == m_subscribedDatas.end())
			{
				ValueObserverWrapper* pDataObserverWrapper = new ValueObserverWrapper(fnObserver);

				m_subscribedDatas.insert(std::make_pair(dataID, ObserverWrapperPtr(pDataObserverWrapper, InterfaceDeleter<ValueObserverWrapper, IDataObserver>())));
				m_pCenter->Subscribe(strDomain.c_str(), strName.c_str(), pDataObserverWrapper);
			}
		}

		void SubscribeObjectValue(const std::string& strDomain, const std::string strName, const ObjectValueObserver& fnObserver)
		{
			DataID				dataID = std::make_pair(strDomain, strName);
			auto				itFind = m_subscribedDatas.find(dataID);

			if (itFind == m_subscribedDatas.end())
			{
				ObjectValueObserverWrapper* pDataObserverWrapper = new ObjectValueObserverWrapper(fnObserver);

				m_subscribedDatas.insert(std::make_pair(dataID, ObserverWrapperPtr(pDataObserverWrapper, InterfaceDeleter<ObjectValueObserverWrapper, IDataObserver>())));
				m_pCenter->Subscribe(strDomain.c_str(), strName.c_str(), pDataObserverWrapper);
			}
		}

		void SubscribeObjectValueEx(const std::string& strDomain, const std::string strName, const std::string strMember, const ObjectValueObserver& fnObserver)
		{
			DataID				dataID = std::make_pair(strDomain, strName);
			auto				itFind = m_subscribedDatas.find(dataID);

			if (itFind == m_subscribedDatas.end())
			{
				ObjectValueObserverWrapper* pDataObserverWrapper = new ObjectValueObserverWrapper(fnObserver);

				m_subscribedDatas.insert(std::make_pair(dataID, ObserverWrapperPtr(pDataObserverWrapper, InterfaceDeleter<ObjectValueObserverWrapper, IDataObserver>())));
				m_pCenter->SubscribeEx(strDomain.c_str(), strName.c_str(), strMember.c_str(), pDataObserverWrapper);
			}
		}

		void SubscribeListValue(const std::string& strDomain, const std::string strName, const ListValueObserver& fnObserver)
		{
			DataID				dataID = std::make_pair(strDomain, strName);
			auto				itFind = m_subscribedDatas.find(dataID);

			if (itFind == m_subscribedDatas.end())
			{
				ListValueObserverWrapper* pDataObserverWrapper = new ListValueObserverWrapper(fnObserver);

				m_subscribedDatas.insert(std::make_pair(dataID, ObserverWrapperPtr(pDataObserverWrapper, InterfaceDeleter<ListValueObserverWrapper, IDataObserver>())));
				m_pCenter->Subscribe(strDomain.c_str(), strName.c_str(), pDataObserverWrapper);
			}
		}

		void Unsubscribe(const std::string& strDomain, const std::string strName)
		{
			DataID				dataID = std::make_pair(strDomain, strName);
			auto				itFind = m_subscribedDatas.find(dataID);

			if (itFind != m_subscribedDatas.end())
			{
				m_pCenter->Unsubscribe(strDomain.c_str(), strName.c_str(), itFind->second.get());
				m_subscribedDatas.erase(itFind);
			}
		}

		void UnsubscribeAll()
		{
			for (auto& observerInfo : m_subscribedDatas)
			{
				m_pCenter->Unsubscribe(observerInfo.first.first.c_str(), observerInfo.first.second.c_str(), observerInfo.second.get());
			}
			m_subscribedDatas.clear();
		}

	private:
		typedef std::pair<std::string, std::string> DataID;
		typedef std::function<void(IDataObserver*)> DataObserverDeleter;
		typedef std::unique_ptr<IDataObserver, DataObserverDeleter> ObserverWrapperPtr;

	private:
		IDataCenter* m_pCenter;
		// 数据订阅：存储点1。
		// 已订阅过的目标，不再向dataCenter中注册
		std::map<DataID, ObserverWrapperPtr> 	m_subscribedDatas;
	};
}

/* 开始数据处理*/
#define  BEGIN_SUBSCRIBE_DATAS()\
private:\
	void SubscribeDatas(IDataCenter *pCenter)\
	{\
		if (nullptr == m_observerHelperPtr.get())\
			m_observerHelperPtr.reset(new DataObserverHelper(pCenter));

// why:常用订阅宏
#define ON_SUBSCRIBE_DATA(lpDomain, lpName, func) \
	m_observerHelperPtr->Subscribe(lpDomain, lpName, std::bind(&func, this, std::placeholders::_1));

/* void OnValueUpdate(IDataValue* pDataValue)*/
#define ON_SUBSCRIBE_VALUE(lpDomain, lpName, func) \
	m_observerHelperPtr->SubscribeValue(lpDomain, lpName, std::bind(&func, this, std::placeholders::_1));

/* void OnListValueUpdate(IListDataValue* pListDataValue)*/
#define ON_SUBSCRIBE_LIST_VALUE(lpDomain, lpName, func) \
	m_observerHelperPtr->SubscribeListValue(lpDomain, lpName, std::bind(&func, this, std::placeholders::_1));

/* void OnObjectValueUpdate(IObjectDataValue* IObjectDataValue)*/
#define ON_SUBSCRIBE_OBJECT_VALUE(lpDomain, lpName, func) \
	m_observerHelperPtr->SubscribeObjectValue(lpDomain, lpName, std::bind(&func, this, std::placeholders::_1));

/* void OnObjectValueUpdate(IObjectDataValue* IObjectDataValue)*/
#define ON_SUBSCRIBE_OBJECT_VALUE_Ex(lpDomain, lpName, lpMember, func) \
	m_observerHelperPtr->SubscribeObjectValueEx(lpDomain, lpName, lpMember, std::bind(&func, this, std::placeholders::_1));

/*DataObserver*/
#define ON_SUBSCRIBE_FUNC(lpDomain, lpName, func) \
	m_observerHelperPtr->Subscribe(lpDomain, lpName, func);

#define UNSUBSCRIBE_DATA(lpDomain, lpName) \
	m_observerHelperPtr->Unsubscribe(lpDomain, lpName);

/* 结束数据处理*/
#define END_SUBSCRIBE_DATAS()\
	}\
\
private:\
	std::unique_ptr<DataObserverHelper>		m_observerHelperPtr;

#define INITIALIZE_OBSERVERS(pCenter) SubscribeDatas(pCenter);