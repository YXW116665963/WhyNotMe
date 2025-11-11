#include "event_dispatcher.h"
#include "Logger.h"
#include "util.h"
#include "logger.h"

namespace
{
	static int m_g_id = 0;
	static const uint64_t m_cycle_time_milli = 1000;
	why::EventDispatcher *m_instance = nullptr;
}

namespace why
{
	IEventDispatcher* GetEventDispatcher()
	{
		return why::EventDispatcher::GetInstance();
	}

	EventDispatcher* EventDispatcher::GetInstance()
	{
		if (nullptr == m_instance)
		{
			m_instance = new why::EventDispatcher();
			m_instance->Init();
		}
		return m_instance;
	}

	EventDispatcher::EventDispatcher()
	{
		LOG_INFO << "new EventDispatcher:" << this;

		m_run = true;
		m_threadPtr.reset(new std::thread(&EventDispatcher::RunTimer, this));
	}

	void EventDispatcher::Init()
	{
		m_pProcedureCall = GetProcedureCallInstance(std::bind(&EventDispatcher::CallHelpFunc, this, std::placeholders::_1));
	}

	void EventDispatcher::CallHelpFunc(uint32_t uParam)
	{
		GLFWdonthinkeventfun(uParam, uParam);
	}

	EventDispatcher::~EventDispatcher()
	{
		DestroyWaitQueue();
		LOG_INFO << "start ~EventDispatcher:" << this;
	}

	void EventDispatcher::DoItOnMainThread(const DoFunction &doFunction)
	{
		if (why::IsMainThread())
		{
			doFunction();
		}
		else
		{
			PrivateEvent		privateEvent;

			privateEvent.type = event_do_it;
			privateEvent.doFunction = doFunction;

			{
				std::lock_guard<std::mutex>		guard(m_lockWait);

				m_waitQueue.push_back(privateEvent);
			}
			// 同步执行当前等待队列m_waitQueue的所有任务
			m_pProcedureCall->CallSynchronously(0);
		}
	}

	void EventDispatcher::DoItOnMainThreadAsync(const DoFunction &doFunction, void *pFunThis, uint64_t uTimeMilli)
	{
		PrivateEvent		privateEvent;

		privateEvent.type = event_do_it;
		privateEvent.doFunction = doFunction;
		privateEvent.uTimeMilli = uTimeMilli;
		privateEvent.uStartTime = why::GetTimeStamp64();
		privateEvent.pFunThis = pFunThis;

		if (0 != uTimeMilli)
		{
			std::lock_guard<std::mutex>		guard(m_lockWait);
			m_waitTimeQueue.push_back(privateEvent);
		}
		else
		{
			{
				std::lock_guard<std::mutex>		guard(m_lockWait);

				m_waitQueue.push_back(privateEvent);
			}

			m_pProcedureCall->CallAsynchronous(0);
		}
	}

	int EventDispatcher::DoCycleTimer(const DoFunction &doFunction, void* pFunThis, uint64_t time_milli, bool run_now)
	{
		if (time_milli < m_cycle_time_milli)
		{
			LOG_ERROR << "time_milli too small time:" << time_milli;
			return 0;
		}

		int id = 0;

		PrivateEvent		privateEvent;

		privateEvent.type = event_do_it;
		privateEvent.doFunction = doFunction;
		privateEvent.uTimeMilli = time_milli;
		privateEvent.uStartTime = why::GetTimeStamp64();
		privateEvent.pFunThis = pFunThis;

		{
			std::lock_guard<std::mutex>		guard(m_lockWait);
			id = (++m_g_id);
			m_cycleTimeMap.emplace(id, privateEvent);
			LOG_INFO << "DoCycleTimer id:" << id << ", pFunThis:0x" << pFunThis;
		}

		if (true == run_now)
		{
			{
				std::lock_guard<std::mutex>		guard(m_lockWait);
				m_waitQueue.push_back(privateEvent);
			}
			// 异步执行当前等待队列m_waitQueue的所有任务
			m_pProcedureCall->CallAsynchronous(0);
		}

		return id;
	}

	void EventDispatcher::ClearCycleTimer(int &id)
	{
		if (0 == id) return;

		std::lock_guard<std::mutex>		guard(m_lockWait);
		auto iter = m_cycleTimeMap.find(id);
		if (iter != m_cycleTimeMap.end())
		{
			LOG_INFO << "ClearCycleTimer id:" << id;
			m_cycleTimeMap.erase(iter);
		}

		id = 0;
	}

	void EventDispatcher::DestroyWaitQueue()
	{
		LOG_INFO << "start DestroyWaitQueue:0x" << this;

		{
			std::lock_guard<std::mutex>		guard(m_lockWait);
			m_waitQueue.clear();
			m_waitTimeQueue.clear();
			m_cycleTimeMap.clear();
		}

		if (m_pProcedureCall)
		{
			m_pProcedureCall->Destory();
			m_pProcedureCall = nullptr;
		}

		m_run = false;
		if (m_threadPtr.get() && m_threadPtr->joinable())
			m_threadPtr->join();
		m_threadPtr.reset();

		LOG_INFO << "end DestroyWaitQueue:0x" << this;
	}

	void EventDispatcher::ClearThisFun(void *pFunThis)
	{
		if (nullptr == pFunThis)
			return;

		std::lock_guard<std::mutex>		guard(m_lockWait);

		for (auto iter = m_waitQueue.begin(); iter != m_waitQueue.end();)
		{
			if (iter->pFunThis == pFunThis)
			{
				iter = m_waitQueue.erase(iter);
			}
			else
			{
				iter++;
			}
		}

		for (auto iter = m_waitTimeQueue.begin(); iter != m_waitTimeQueue.end();)
		{
			if (iter->pFunThis == pFunThis)
			{
				iter = m_waitTimeQueue.erase(iter);
			}
			else
			{
				iter++;
			}
		}

		for (auto iter = m_cycleTimeMap.begin(); iter != m_cycleTimeMap.end(); )
		{
			if (iter->second.pFunThis == pFunThis)
			{
				iter = m_cycleTimeMap.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	
	void EventDispatcher::GLFWdonthinkeventfun(unsigned int wParam, long lParam)
	{
		bool bRun = false;
		uint64_t start = why::GetTimeStamp64();

		do
		{
			PrivateEvent event;
			bRun = GetOneWaitEvent(event);

			if (bRun && event.doFunction)
			{
				start = why::GetTimeStamp64();
				event.doFunction();
				if (why::GetTimeStamp64() - start >= 100)
				{
					LOG_ERROR << "Running too slow pFunThis:0x" << event.pFunThis << ", type:" << (int)event.type;
				}
			}

		} while (bRun);
	}

	bool EventDispatcher::GetOneWaitEvent(PrivateEvent &event)
	{
		std::lock_guard<std::mutex>		guard(m_lockWait);
		if (m_waitQueue.empty())
			return false;
		event = m_waitQueue.front();
		m_waitQueue.pop_front();
		return true;
	}

	int EventDispatcher::RunTimer()
	{
		LOG_INFO << "start RunTimer id:" << ::GetCurrentThreadId();
		uint64_t uTime = why::GetTimeStamp64();
		bool bUpate = false;

		do
		{
			uTime = why::GetTimeStamp64();
			bUpate = false;

			{
				std::lock_guard<std::mutex>		guard(m_lockWait);
				for (auto iter = m_waitTimeQueue.begin(); iter != m_waitTimeQueue.end(); )
				{
					if (iter->uTimeMilli + iter->uStartTime <= uTime)
					{
						m_waitQueue.push_back(*iter);
						iter = m_waitTimeQueue.erase(iter);
						bUpate = true;
					}
					else
					{
						iter++;
					}
				}
			}

			{
				std::lock_guard<std::mutex>		guard(m_lockWait);
				for (auto iter = m_cycleTimeMap.begin(); iter != m_cycleTimeMap.end(); iter++)
				{
					if (iter->second.uTimeMilli + iter->second.uStartTime <= uTime)
					{
						m_waitQueue.push_back(iter->second);
						iter->second.uStartTime += iter->second.uTimeMilli;
						bUpate = true;
					}
				}
			}

			if (bUpate) m_pProcedureCall->CallAsynchronous(0);

			if (m_run) std::this_thread::sleep_for(std::chrono::milliseconds(10));

		} while (m_run);

		LOG_INFO << "end RunTimer id:" << ::GetCurrentThreadId();

		return 0;
	}

	void EventDispatcher::Destroy()
	{
		m_instance = nullptr;
		DestroyWaitQueue();
	}
}
