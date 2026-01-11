#pragma once

#include "virtual_wnd.h"
#include "resource_define.h"
#include <atomic>
#include <map>
#include "ui_struct_def.h"

namespace why
{
	class VirtualWndOwner : public wxEvtHandler
	{
	public:
		VirtualWndOwner(wxWindow* pOwner);
		~VirtualWndOwner() = default;

	public:
		wxWindow* GetWindow() { return m_pOwner; }
		void RefreshRect(const wxRect& rcRect);

		/*
		* @brief 设置子图像
		* @param name：同一类图像使用相同名字
		*/
		void AddVirtualWnd(const std::string& name, std::shared_ptr<VirtualWnd> pVirtualWnd);
		void UpdateCameraPos(const std::string& name, ui::CameraPos uType);

		std::shared_ptr<VirtualWnd> FindVirtualWnd(const wxString& strName, const std::string& domain = m_name_ui);
		void Show(const std::string& name, bool bShow);
		std::string GetCurrentUIName();
		wxRect GetCameraRect(const wxRect& rect);

		void OnPaint(wxDC& dc);
		void OnLButtonDown(wxMouseEvent& event);
		void OnLButtonUp(wxMouseEvent& event);
		void OnMouseMove(wxMouseEvent& event);
		void OnEnterWindow(wxMouseEvent& event);
		void OnLeaveWindow(wxMouseEvent& event);
		void OnLButtonDClick(wxMouseEvent& event);

		uint32_t AddMouseFence(const wxRect& rcFence);
		void UpdateMouseFence(uint32_t uFenceID, const wxRect& rcFence);
		void RemoveMouseFence(uint32_t uFenceID);

		void OnCurrentLeaveWindow(wxMouseEvent& event);

	private:
		void CreateBufferBitmap();
		void DrawVirtualWnd(wxDC& dc);
		bool MouseInFence(const wxPoint& ptMouse);

	public:
		//<! AddVirtualWnd函数中的name参数
		static const char* m_name_ui;
		static const char* m_name_template;
		static const char* m_name_align;
		static const char* m_name_bld_mark;
		static const char* m_name_monitor;

	private:
		struct VirtualWndConfig
		{
			std::list<std::shared_ptr<VirtualWnd>> list;
			ui::CameraPos uType{ ui::CameraPos::eNone };
		};

	private:
		wxWindow* m_pOwner;
		std::map<std::string, VirtualWndConfig> m_childs;
		wxBitmap						m_bufferBitmap;
		wxSize							m_szBitmap;
		wxRect							m_rcRefresh;
		std::shared_ptr<VirtualWnd>		m_pCapture{ nullptr };
		std::shared_ptr<VirtualWnd>		m_pCurrent{ nullptr };
		bool							m_bCapture{ false };
		std::atomic_bool                m_bShow{ true };
		std::string                     m_showName{ m_name_ui };
		uint32_t						m_uLastFenceID{ 1 };
		std::map<uint32_t, wxRect>		m_mouseFences;
	};
}