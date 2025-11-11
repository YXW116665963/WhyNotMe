#include "timer.h"
#include "util.h"
#include "logger.h"
#include "event_dispatcher_interface.h"

namespace why
{ 
	Timer::Timer(bool bSingleHanded, const std::string& strName /*= ""*/)
		: m_bSingleHanded(bSingleHanded)
		, m_strName(strName)
	{

	}

	Timer::~Timer()
	{
		m_bWouldExit = true;
		if (nullptr != m_taskThreadPtr.get())
		{
			if (m_taskThreadPtr->joinable())
				m_taskThreadPtr->join();
			m_taskThreadPtr = nullptr;
		}
	}

	bool Timer::Startup()
	{
		if (nullptr == m_taskThreadPtr.get())
		{
			m_bWouldExit = false;
			m_taskThreadPtr.reset(new std::thread(&Timer::Run, this));
		}

		return true;
	}

	void Timer::Shutdown()
	{
		if (m_taskThreadPtr.get())
		{
			m_bWouldExit = true;
			if (m_bSingleHanded)
			{
				if (m_taskThreadPtr->joinable())
					m_taskThreadPtr->join();
				m_taskThreadPtr = nullptr;
			}
		}
	}
 
	TimerID Timer::AddTask(const TimerProc &taskProc, uint32_t uInterval)
	{
		TimerID				uTaskID = m_uNextID++;
		
		AddTask(uTaskID, taskProc, uInterval);
		return uTaskID;
	}

	void Timer::RemoveTask(TimerID& uTaskID)
	{
		if (uTaskID <= 0)
			return;

		TaskItem						taskItem;

		taskItem.m_taskProc = nullptr;
		taskItem.m_uTaskID = uTaskID;
		taskItem.m_uInterval = 0;
		taskItem.m_uDoTaskStamp = 0;
		
		{
			std::lock_guard<std::mutex>		lockGuard(m_lock);
			bool							bAdd = true;

			for (auto it = m_waitTasks.begin(); it != m_waitTasks.end(); it++)
			{
				if (it->m_uTaskID == uTaskID)
				{
					bAdd = false;
					if (nullptr != it->m_taskProc)
					{
						m_waitTasks.erase(it);
						break;
					}
				}
			}

			if (bAdd)
				m_waitTasks.push_back(taskItem);

			uTaskID = 0;
		}
	}

	void Timer::AddTask(TimerID uTaskID, const TimerProc &taskProc, uint32_t uInterval)
	{
		TaskItem						taskItem;
		uint64_t						uStamp = GetTimeStamp64();

		LOG_INFO << "Timer[" << this << "] AddTask:" << uTaskID << ", Interval:" << uInterval;

		taskItem.m_taskProc = taskProc;
		taskItem.m_uTaskID = uTaskID;
		taskItem.m_uInterval = uInterval;
		if (0 == uInterval)
			taskItem.m_uDoTaskStamp = uStamp;
		else
			taskItem.m_uDoTaskStamp = uStamp + uInterval;

		{
			std::lock_guard<std::mutex>		lockGuard(m_lock);

			m_waitTasks.push_back(taskItem);
		}
	}

	void Timer::Run()
	{
		uint64_t				uStamp = GetTimeStamp64();
		bool					bProcceed = false;

		LOG_INFO << "Timer "<< m_strName<<"[" << this << "]enter task proces thread:" << std::this_thread::get_id();
		while (!m_bWouldExit)
		{
			bProcceed = false;
			GetWaitTasks();
			if (m_bWouldExit)
				break;

			uStamp = GetTimeStamp64();
			for (auto it = m_processTasks.begin(); it != m_processTasks.end(); )
			{
				if (m_bWouldExit)
				{
					bProcceed = true;
					break;
				}

				if (!IsInvalid(it->m_uTaskID) && (it->m_uDoTaskStamp < uStamp))
				{
					TimerStatus			taskResult{ TimerStatus::keep_on };
					auto&				taskProc{ it->m_taskProc };

					bProcceed = true;
					if (m_bSingleHanded)
						taskResult = taskProc();
					else
						GetEventDispatcher()->DoItOnMainThread([&taskProc, &taskResult]() {taskResult = taskProc(); });

					if (taskResult == TimerStatus::keep_on)
					{
						if (0 != it->m_uInterval)
						{
							it->m_uDoTaskStamp = uStamp + it->m_uInterval;
							it++;
						}
						else
						{	
							it = m_processTasks.erase(it);
						}
					}
					else
					{
						it = m_processTasks.erase(it);
					}
				}
				else
				{
					it++;
				}
			}
			/*if (m_strName == "SoftwareCard")
			{
				LOG_EVERY_N(INFO, 100) << "Timer[" << this << "] run:" << m_strName << " thread:" << std::this_thread::get_id() << " size:" << m_processTasks.size();
			}*/
					
			if (!bProcceed)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		LOG_INFO << "Timer " << m_strName << "[" << this << "]leave task proces thread:" << std::this_thread::get_id();
	}

	void Timer::GetWaitTasks()
	{
		WaitTaskVector			waitTasks;

		{
			std::lock_guard<std::mutex>			lockGuard(m_lock);

			if (m_waitTasks.empty())
				return;

			waitTasks.swap(m_waitTasks);
		}

		for (auto it = waitTasks.begin(); it != waitTasks.end(); it++)
		{
			if (nullptr == it->m_taskProc)
				DeleteTask(it->m_uTaskID);
			else
				m_processTasks.push_back(*it);
		}
	}

	void Timer::DeleteTask(TimerID uTaskID)
	{
		for (auto it = m_processTasks.begin(); it != m_processTasks.end(); )
		{
			if (uTaskID == it->m_uTaskID)
				it = m_processTasks.erase(it);
			else
				it++;
		}
	}

	bool Timer::IsInvalid(TimerID uTaskID)
	{
		std::lock_guard<std::mutex>		lockGuard(m_lock);
		bool							bInvalid = false;

		for (auto it = m_waitTasks.begin(); it != m_waitTasks.end(); it++)
		{
			if (it->m_uTaskID == uTaskID)
			{
				if (nullptr == it->m_taskProc)
					bInvalid = true;
				break;
			}
		}

		return bInvalid;
	}

	void SafeRemoveTimerID(Timer& taskProcessor, TimerID& uTaskID)
	{
		if (0 != uTaskID)
		{
			taskProcessor.RemoveTask(uTaskID);
			uTaskID = 0;
		}
	}

	void SafeRemoveTimerID(Timer* pTaskProcessor, TimerID& uTaskID)
	{
		if ((nullptr != pTaskProcessor) && (0 != uTaskID))
		{
			pTaskProcessor->RemoveTask(uTaskID);
			uTaskID = 0;
		}
	}
}
