#pragma once

#include "standard.h"
#include "procedure_call_interface.h"

#include <functional>
#include <thread>
#include <windows.h>

namespace why
{
	class ProcedureCall : public IProcedureCall
	{
	public:
		ProcedureCall(const std::function<void(uint32_t)>& callProce);
		~ProcedureCall();

	public:
		bool IsSameThread();
		// 数据订阅：移交到窗口线程，同步执行回调。
		void CallSynchronously(uint32_t uParam);
		// 数据订阅：移交到窗口线程，异步执行回调。
		void CallAsynchronous(uint32_t uParam);
		void Destory() { delete this; }

	public:
		bool IsStartup() { return (nullptr != m_hWnd); }

	private:
		static LRESULT CALLBACK _WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// 数据订阅：WM_CALL_PROCE消息处理函数
		LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	private:
		std::function<void(uint32_t)>	m_callProce;
		HWND							m_hWnd;
		std::thread::id					m_threadID;
	};
}