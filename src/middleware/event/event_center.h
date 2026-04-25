#pragma once

#include "standard.h" 
#include "event_data.h"

#include <functional>
#include <memory>
#include <map>
#include <set>
#include <optional>
#include "logger.h"

namespace why
{
	typedef std::function<void(const std::optional<EventData>& EventData)> EventSubFunction;
	class EventCenterGuard;
	class EventCenter
	{
	public:
		EventCenter();
		~EventCenter();

	public:
		void FireEvent(const std::string& strEventName, std::optional<EventData> eventData = std::nullopt);
		friend class EventCenterGuard;
	
	private:
		uint64_t Subscribe(const std::string& strEventName, const EventSubFunction& fnEventSub);
		void UnSubscribe(const std::string& strEventName, uint64_t uEventSubId);

	private:
		uint64_t GenerateID();
		void Unregister(uint64_t uID);
		bool Find(uint64_t uID);
		void DoFireEvent(const std::string & strEventName, uint64_t m_eventSubId, const EventSubFunction& fnEventSub, std::optional<EventData> EventData = std::nullopt);

	private:
		typedef std::map<uint64_t, EventSubFunction> EventSubMap;
		typedef std::map<std::string, EventSubMap> EventMap;

	private:
		uint64_t					m_uNextID{ 1 };
		std::set<uint64_t>			m_useds;
		EventMap					m_eventMap;
	};

	class EventCenterGuard
	{
	public:
		EventCenterGuard();
		~EventCenterGuard();
		void FireEvent(const std::string& strEventName, std::optional<EventData> eventData = std::nullopt);
		void Subscribe(const std::string& strEventName, const EventSubFunction& fnEventSub);
	private:
		std::map<uint64_t, std::string>			m_eventSubed;
	};
	
	void CreateEventCenter();
	EventCenter* GetEventCenter();
	void DestroyEventCenter();

	//定义一个异步处理器的哨兵对象
#define USING_EVENT_CENTER() \
private:\
	EventCenterGuard		m_eventCenterGuard;

}
