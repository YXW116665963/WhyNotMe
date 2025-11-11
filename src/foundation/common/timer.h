#pragma once

#include "standard.h"

#include <memory>
#include <functional>
#include <mutex>
#include <thread>
#include <list>
#include <atomic>
#include <vector>

/**
	* @brief 定义定时任务运行后后续是否继续运行
	*/
enum class TimerStatus
{
	keep_on,		//继续等待下次运行
	stop,			//停止
};

namespace why
{
	/**
	* @brief 异步任务的过程函数原型
	* @return 
	* - true 如果任务为定时任务，则任务继续
	* - false 如果任务为定时任务，则定时任务取消
	*/
	typedef std::function<TimerStatus(void)> TimerProc;
	typedef std::function<void(void)> TimeoutProc;
	typedef uint64_t TimerID;

	class Timer
	{
	public:
		Timer(bool bSingleHanded, const std::string& strName = "");
		~Timer();

	public:
		bool Startup();
		void Shutdown();
		TimerID AddTask(const TimerProc &taskProc, uint32_t uInterval);

		/*
		* @brief 从任务队列中移除任务指定任务
		* @param 任务ID
		* @warning 清楚任务后，任务ID将会被设置为0
		*/
		void RemoveTask(TimerID& uTaskID);
		
	private:
		typedef std::unique_ptr<std::thread> ThreadPrt;

		struct TaskItem
		{
			TimerProc				m_taskProc{nullptr};
			uint64_t				m_uTaskID{ 0 };
			uint64_t				m_uDoTaskStamp{0};
			uint32_t				m_uInterval{ 0 };
			bool					m_b{ false };
		};

		typedef std::list<TaskItem> TaskQueue;
		typedef TaskQueue::iterator TaskIt;
		typedef std::vector<TaskItem> WaitTaskVector;

	private:
		void Run();
		void AddTask(TimerID uTaskID, const TimerProc &taskProc, uint32_t uInterval);
		void GetWaitTasks();
		void DeleteTask(TimerID uTaskID); 
		bool IsInvalid(TimerID uTaskID);
		void DoTaskOnMainThread(TimerID uTaskID);

	private:
		bool						m_bWouldExit{false};
		ThreadPrt					m_taskThreadPtr;
		std::mutex					m_lock;
		std::atomic<TimerID>		m_uNextID{1};
		WaitTaskVector				m_waitTasks;
		TaskQueue					m_processTasks;
		bool						m_bSingleHanded;
		std::string					m_strName{ "" };
	};

	void SafeRemoveTimerID(Timer& taskProcessor, TimerID& uTaskID);
	void SafeRemoveTimerID(Timer* pTaskProcessor, TimerID& uTaskID);
}