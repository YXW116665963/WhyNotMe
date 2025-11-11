#pragma once
#include <functional>

namespace why
{
	class IEventDispatcher
	{
	public:
		typedef std::function<void(void)> DoFunction;

	public:
		virtual void DoItOnMainThread(const DoFunction &doFunction) = 0;

		virtual void DoItOnMainThreadAsync(const DoFunction &doFunction, void *pFunThis, uint64_t uTimeMilli = 0) = 0;

		//<! time_milli must greater than 1000
		virtual int DoCycleTimer(const DoFunction &doFunction, void* pFunThis, uint64_t time_milli, bool run_now = false) = 0;

		virtual void ClearCycleTimer(int &id) = 0;

		virtual void DestroyWaitQueue() = 0;

		virtual void ClearThisFun(void *pFunThis) = 0;
		virtual void Destroy() = 0;
	};

	IEventDispatcher* GetEventDispatcher();
}