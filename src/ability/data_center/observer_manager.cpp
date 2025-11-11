#include "observer_manager.h"

namespace why
{
	ObserverManager::ObserverManager(DataDB* pDataDB)
		: m_pDataDB(pDataDB)
	{
		// 
		m_pProcedureCall = GetProcedureCallInstance(std::bind(&ObserverManager::CallProcedure, this, std::placeholders::_1));
	}

	ObserverManager::~ObserverManager()
	{
		m_pProcedureCall->Destory();
		m_pProcedureCall = nullptr;
	}

	void ObserverManager::Subscribe(const std::string& strDomain,
		const std::string& strName, const std::string& strMember, IDataObserver* pObserver)
	{
		if (false == m_running) return;

		if (pObserver)
		{
			WriteLockGuard			lockGuard(m_lock);
			auto					itFindDomain = m_observers.find(strDomain);

			if (m_observers.end() == itFindDomain)
			{
				m_observers.insert(std::make_pair(strDomain, DomainDataObservers()));
				itFindDomain = m_observers.find(strDomain);
			}

			auto					itFindData = itFindDomain->second.find(strName);

			if (itFindDomain->second.end() == itFindData)
			{
				itFindDomain->second.insert(std::make_pair(strName, ObserverList()));
				itFindData = itFindDomain->second.find(strName);
			}

			itFindData->second.push_back({ strMember, pObserver });
		}
	}

	void ObserverManager::Unsubscribe(const std::string& strDomain, const std::string& strName, IDataObserver* pObserver)
	{
		if (false == m_running) return;

		if (pObserver)
		{
			WriteLockGuard			lockGuard(m_lock);
			auto					itFindDomain = m_observers.find(strDomain);

			if (m_observers.end() != itFindDomain)
			{
				auto				itFindData = itFindDomain->second.find(strName);

				if (itFindDomain->second.end() != itFindData)
				{
					for (auto itCur = itFindData->second.begin(); itCur != itFindData->second.end(); itCur++)
					{
						if (itCur->m_pObserver == pObserver)
						{
							itFindData->second.erase(itCur);
							break;
						}
					}

					if (itFindData->second.empty())
					{
						itFindDomain->second.erase(itFindData);
						if (itFindDomain->second.empty())
							m_observers.erase(itFindDomain);
					}
				}
			}
		}
	}

	void ObserverManager::OnValueReady(const std::string& strDomain, const std::string& strName, INamedData* pNamedData)
	{
		if (false == m_running) return;

		if (pNamedData)
		{
			uint32_t			uEventID = 0;

			{
				std::lock_guard<std::mutex>			lockGuard(m_eventLock);
				EventData							eventData;

				uEventID = m_uCurrentID++;
				eventData.m_strDomain = strDomain;
				eventData.m_strName = strName;
				eventData.m_bReady = true;
				eventData.m_pNamedData = pNamedData;
				m_events.insert(std::make_pair(uEventID, eventData));
			}

			m_pProcedureCall->CallSynchronously(uEventID);
		}
	}

	void ObserverManager::OnValueChanged(const std::string& strDomain, const std::string& strName, const std::string& strMember, INamedData* pNamedData)
	{
		if (false == m_running) return;
		OnValueChangedEx(strDomain, strName, strMember, pNamedData, nullptr);
	}

	void ObserverManager::OnValueChangedEx(const std::string& strDomain, const std::string& strName, const std::string& strMember, INamedData* pNamedData, IDataObserver* pObserver)
	{
		if (false == m_running) return;

		if (pNamedData)
		{
			bool				bSaveToDB = (DataStyle::ePersistence == pNamedData->GetStyle());
			uint32_t			uEventID = 0;

			if (bSaveToDB && (nullptr != m_pDataDB) && m_pDataDB->IsStartup())
				m_pDataDB->WriteData(pNamedData);

			{
				std::lock_guard<std::mutex>			lockGuard(m_eventLock);
				EventData							eventData;

				uEventID = m_uCurrentID++;
				eventData.m_strDomain = strDomain;
				eventData.m_strName = strName;
				eventData.m_strMember = strMember;
				eventData.m_pNamedData = pNamedData;
				eventData.m_pObserver = pObserver;
				m_events.insert(std::make_pair(uEventID, eventData));
			}

			m_pProcedureCall->CallSynchronously(uEventID);
		}
	}

	void ObserverManager::Stop()
	{
		m_running = false;
	}

	void ObserverManager::CallProcedure(uint32_t uParam)
	{
		if (false == m_running) return;

		EventData							eventData;
		bool								bFind = false;

		{
			std::lock_guard<std::mutex>			lockGuard(m_eventLock);
			auto								itFind = m_events.find(uParam);

			if (m_events.end() != itFind)
			{
				eventData = itFind->second;
				bFind = true;
				m_events.erase(itFind);
			}
		}

		if (bFind)
		{
			if (nullptr == eventData.m_pObserver)
			{
				// 
				ObserverVector		observerVector;
				ObserverVector		observerVectorAll;

				{
					ReadLockGuard			lockGuard(m_lock);
					auto					itFindDomain = m_observers.find(eventData.m_strDomain);

					if (m_observers.end() != itFindDomain)
					{
						auto				itFindData = itFindDomain->second.find(eventData.m_strName);
						if (itFindDomain->second.end() != itFindData)
						{
							for (auto itCur = itFindData->second.begin(); itCur != itFindData->second.end(); itCur++)
							{
								if (eventData.m_bReady || (eventData.m_strMember == itCur->m_strMember))
									observerVector.push_back(itCur->m_pObserver);
							}
						}

						// ？：为啥要查找空字符串的成员？
						// why：为了遍历对象类型使用，itFindDataAll是list类型，用来存储可能的多个回调
						auto itFindDataAll = itFindDomain->second.find("");
						if (itFindDomain->second.end() != itFindDataAll)
						{
							for (auto itCur = itFindDataAll->second.begin(); itCur != itFindDataAll->second.end(); itCur++)
							{
								if (eventData.m_strMember.empty())
									observerVectorAll.push_back(itCur->m_pObserver);
							}
						}
					}
				}

				// 触发回调
				for (auto pObserver : observerVector)
					pObserver->OnDataChanged(eventData.m_pNamedData);

				for (auto pObserver : observerVectorAll)
					pObserver->OnDataChanged(eventData.m_pNamedData);
			}
			else
			{
				eventData.m_pObserver->OnDataChanged(eventData.m_pNamedData);
			}
		}
	}
}