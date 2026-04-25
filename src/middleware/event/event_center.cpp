#include "event_center.h"
#include "event_dispatcher_interface.h"
#include "util.h"

namespace why
{
	EventCenter*	g_pEventCenter = nullptr;
	
	void CreateEventCenter()
	{
		g_pEventCenter = new EventCenter();
	}

	EventCenter* GetEventCenter()
	{
		return g_pEventCenter;
	}

	void DestroyEventCenter()
	{
		if (g_pEventCenter)
		{
			delete g_pEventCenter;
			g_pEventCenter = nullptr;
		}
	}

	EventCenter::EventCenter()
	{

	}

	EventCenter::~EventCenter()
	{
		GetEventDispatcher()->ClearThisFun(this);
	}

	void EventCenter::FireEvent(const std::string& strEventName, std::optional<EventData> eventData)
	{
		assert(why::IsMainThread());
		auto	itFind = m_eventMap.find(strEventName);
		if (itFind != m_eventMap.end())
		{
			auto&  eventSubList = itFind->second;
			for (auto& item : eventSubList)
			{
				if (!Find(item.first))
				{
					continue;
				}
				GetEventDispatcher()->DoItOnMainThreadAsync([this, strEventName, eventData, eventSubId = item.first, eventSubFunction = item.second]() {
					DoFireEvent(strEventName, eventSubId, eventSubFunction, eventData);
				}, this);
			}
		}	
	}

	void EventCenter::DoFireEvent(const std::string & strEventName, uint64_t eventSubId, const EventSubFunction& fnEventSub, std::optional<EventData> eventData)
	{
		if (Find(eventSubId))
		{
			fnEventSub(eventData);
		} 
	}

	uint64_t EventCenter::Subscribe(const std::string & strEventName, const EventSubFunction & fnEventSub)
	{
		assert(why::IsMainThread()); 

		uint64_t uEventSubId = GenerateID();

		auto	itFind = m_eventMap.find(strEventName);
		if (itFind == m_eventMap.end())
		{ 
			EventSubMap eventSubMap;
			eventSubMap.insert(std::make_pair(uEventSubId, fnEventSub));
			m_eventMap.insert(std::make_pair(strEventName, eventSubMap));
		}
		else
		{
			auto&  eventSubMap = itFind->second;
			eventSubMap.insert(std::make_pair(uEventSubId, fnEventSub));
		}
		return uEventSubId;
	}

	void EventCenter::UnSubscribe(const std::string & strEventName, uint64_t uEventSubId)
	{
		assert(why::IsMainThread());
		auto	itFind = m_eventMap.find(strEventName);
		if (itFind != m_eventMap.end())
		{
			auto&  eventSubMap = itFind->second;
			eventSubMap.erase(uEventSubId);
		}
		Unregister(uEventSubId);
	} 

	uint64_t EventCenter::GenerateID()
	{ 
		uint64_t		uID = m_uNextID++;

		m_useds.insert(uID);
		return uID;
	}

	void EventCenter::Unregister(uint64_t uID)
	{
		m_useds.erase(uID);
	}

	bool EventCenter::Find(uint64_t uID)
	{
		auto	itFind = m_useds.find(uID);

		return (itFind != m_useds.end());
	}

	EventCenterGuard::EventCenterGuard()
	{ 
	}

	EventCenterGuard::~EventCenterGuard()
	{
		EventCenter* pEventCenter = GetEventCenter();
		if (pEventCenter)
		{
			for (auto& item : m_eventSubed)
			{
				pEventCenter->UnSubscribe(item.second, item.first);
			}
		}
	
	}

	void EventCenterGuard::FireEvent(const std::string & strEventName, std::optional<EventData> eventData)
	{
		EventCenter* pEventCenter = GetEventCenter();
		if (pEventCenter)
		{
			pEventCenter->FireEvent(strEventName, eventData);
		}
	}

	void EventCenterGuard::Subscribe(const std::string& strEventName, const EventSubFunction& fnEventSub)
	{
		EventCenter* pEventCenter = GetEventCenter();
		if (pEventCenter)
		{
			auto eventSubId = pEventCenter->Subscribe(strEventName, fnEventSub);
			m_eventSubed.insert(std::make_pair(eventSubId, strEventName));
		}
	}
}
