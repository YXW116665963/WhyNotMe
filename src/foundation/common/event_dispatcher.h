#pragma once
#include "event_dispatcher_interface.h"
#include "standard.h"
#include <functional>
#include <mutex>
#include <windows.h>
#include <map>
#include "procedure_call_interface.h"

namespace why
{
	class EventDispatcher : public IEventDispatcher
	{
	public:
		enum PrivateEventType
		{
			event_none = 0,
			event_do_it = 1,
		};

		struct PrivateEvent
		{
			PrivateEventType    type{ event_none };
			DoFunction			doFunction;
			uint64_t            uTimeMilli{ 0 };
			uint64_t            uStartTime{ 0 };
			void *              pFunThis{ nullptr };
		};
		typedef std::list<PrivateEvent>	WaitQueue;

	public:
		// 插入任务到m_waitQueue，并同步执行m_waitQueue的所有任务
		virtual void DoItOnMainThread(const DoFunction &doFunction);

		// 插入定时任务到m_waitTimeQueue，插入非定时任务到m_waitQueue，并异步执行m_waitQueue的所有任务（也就是不用等待）
		virtual void DoItOnMainThreadAsync(const DoFunction &doFunction, void *pFunThis, uint64_t uTimeMilli = 0);

		// 插入任务到m_cycleTimeMap，如果要求马上执行（run_now等于true）m_waitQueue也要插入该任务。
		virtual int DoCycleTimer(const DoFunction &doFunction, void* pFunThis, uint64_t time_milli, bool run_now = false);

		virtual void ClearCycleTimer(int &id);

		virtual void DestroyWaitQueue();

		virtual void ClearThisFun(void *pFunThis);
		void Destroy() override;

	public:
		// 事件分发器（why）：通知主线程执行的函数任务就是GLFWdonthinkeventfun
		void GLFWdonthinkeventfun(unsigned int wParam, long lParam);

		static EventDispatcher *GetInstance();

	private:
		EventDispatcher();
		~EventDispatcher();

		void Init();

		// 事件分发器（why）：派发在定时器任务队列m_waitTimeQueue和m_cycleTimeMap中符合时间间隔要求的任务到等待队列m_waitQueue
		int RunTimer();

		bool GetOneWaitEvent(PrivateEvent &event);

		void CallHelpFunc(uint32_t uParam);

	private:
		std::mutex						m_lockWait;

		// 所有实际要执行的任务都会被插入到该队列，因为GLFWdonthinkeventfun只执行该队列的任务
		WaitQueue						m_waitQueue;
		// 来源：只会在DoItOnMainThreadAsync中被插入定时任务
		WaitQueue						m_waitTimeQueue;

		std::map<int, PrivateEvent>     m_cycleTimeMap;
		std::shared_ptr<std::thread>    m_threadPtr;
		bool                            m_run{ false };
		IProcedureCall*					m_pProcedureCall{ nullptr };
	};
}