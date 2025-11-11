#include "procedure_call.h"
#include <tchar.h>
#define WM_CALL_PROCE WM_USER+500

namespace why
{
	ProcedureCall::ProcedureCall(const std::function<void(uint32_t)>& callProce)
	{
		WNDCLASS 		wc = { 0 };
		ATOM 			ret = 0;

		m_callProce = callProce;

		wc.style = 0;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hIcon = nullptr;
		wc.lpfnWndProc = ProcedureCall::_WindowProc;
		wc.hInstance = nullptr;
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = _T("Data Pump");
		ret = ::RegisterClass(&wc);
		if ((0 == ret) && (ERROR_CLASS_ALREADY_EXISTS != ::GetLastError()))
			return;

		// 窗口过程（why）：传递this指针（LPVOID类型）给窗口过程函数中的LPARAM lParam参数
		m_hWnd = ::CreateWindowEx(0, _T("Data Pump"), _T("Data Mgr"), WS_POPUP, 0, 0, 1, 1, nullptr, nullptr, nullptr, this);
		if (nullptr == m_hWnd)
			return;

		m_threadID = std::this_thread::get_id();
		::EnableWindow(m_hWnd, FALSE);
	}

	ProcedureCall::~ProcedureCall()
	{
		if (m_hWnd)
		{
			DestroyWindow(m_hWnd);
			m_hWnd = nullptr;
		}
	}

	LRESULT CALLBACK ProcedureCall::_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		ProcedureCall* pThis = nullptr;

		if (WM_CREATE == uMsg)
		{
			LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);

			pThis = static_cast<ProcedureCall*>(lpcs->lpCreateParams);
			pThis->m_hWnd = hWnd;
			
#pragma warning(disable: 4244)
			// 窗口过程（why）：::CreateWindowEx函数会传递WM_CREATE消息	
			// ::CreateWindowEx的参数列表中的LPVOID lpParam传递给窗口过程函数（也就是当前函数）中的LPARAM lParam参数
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
#pragma warning(default: 4244)
		}
		else
		{
#pragma warning(disable: 4312)
			// 窗口过程（why）：如果是其他消息，获取处理WM_CREATE消息时存储的指针（单列实例ProcedureCall）
			pThis = reinterpret_cast<ProcedureCall*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
#pragma warning(default: 4312)

			if ((WM_DESTROY == uMsg) && (NULL != pThis))
			{
				::SetWindowLongPtr(pThis->m_hWnd, GWLP_USERDATA, 0L);
				pThis->m_hWnd = NULL;
				return 0;
			}
		}

		if (nullptr != pThis)
		{
			LRESULT			hResult;
			BOOL			bHandled = FALSE;

			// 窗口过程（why）：基本都是在响应WM_CALL_PROCE消息
			hResult = pThis->WindowProc(uMsg, wParam, lParam, bHandled);
			if (bHandled)
				return hResult;
		}

		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	bool ProcedureCall::IsSameThread()
	{
		return (m_threadID == std::this_thread::get_id());
	}

	void ProcedureCall::CallSynchronously(uint32_t uParam)
	{
		// 如果在同一线程，直接调用函数；否则发送消息叫来主线程处理。
		if (IsSameThread())
			m_callProce(uParam);
		else
			//同步函数，发送消息后会等待接收窗口处理完消息后才返回，适用于需要立即得到处理结果的场景。
			::SendMessage(m_hWnd, WM_CALL_PROCE, uParam, 0);
	}

	void ProcedureCall::CallAsynchronous(uint32_t uParam)
	{
		//异步函数，发送消息后会立即返回，消息会被加入接收窗口的消息队列，适用于不需要立即处理结果的场景。
		::PostMessage(m_hWnd, WM_CALL_PROCE, uParam, 0);
	}

	LRESULT ProcedureCall::WindowProc(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if (WM_CALL_PROCE == uMsg)
		{
			m_callProce((uint32_t)wParam);
			bHandled = TRUE;
		}
		else
		{
			bHandled = FALSE;
		}

		return 0;
	}

	IProcedureCall* GetProcedureCallInstance(const std::function<void(uint32_t)>& callProce)
	{
		ProcedureCall* pProcedureCall = new ProcedureCall(callProce);

		if (pProcedureCall->IsStartup())
			return pProcedureCall;

		pProcedureCall->Destory();
		pProcedureCall = nullptr;

		return nullptr;
	}
}