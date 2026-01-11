#include "virtual_wnd_owner.h"
#include "singleton.h"
#include "resource_manager.h"
#include "ui_util.h"
#include "util.h"
#include "logger.h"

namespace why
{
	const char* VirtualWndOwner::m_name_ui = "ui";
	const char* VirtualWndOwner::m_name_template = "template";
	const char* VirtualWndOwner::m_name_align = "align";
	const char* VirtualWndOwner::m_name_bld_mark = "bld_mark";
	const char* VirtualWndOwner::m_name_monitor = "monitor";

	VirtualWndOwner::VirtualWndOwner(wxWindow* parent)
		: m_pOwner(parent)
	{
		assert(nullptr != m_pOwner);
		m_rcRefresh.SetPosition({ 0,0 });
		m_rcRefresh.SetSize(parent->GetSize());
	}

	void VirtualWndOwner::RefreshRect(const wxRect& rcRect)
	{
		m_rcRefresh += rcRect;
		if (!m_rcRefresh.IsEmpty())
		{
			if (m_pOwner)
				m_pOwner->RefreshRect(m_rcRefresh, false);
		}
	}

	void VirtualWndOwner::AddVirtualWnd(const std::string& name, std::shared_ptr<VirtualWnd> pVirtualWnd)
	{
		auto iter = m_childs.find(name);
		if (iter == m_childs.end())
		{
			VirtualWndConfig config;
			config.list.push_back(pVirtualWnd);
			m_childs.emplace(name, std::move(config));
		}
		else
		{
			iter->second.list.push_back(pVirtualWnd);
		}
	}

	void VirtualWndOwner::UpdateCameraPos(const std::string& name, ui::CameraPos uType)
	{
		auto iter = m_childs.find(name);
		if (iter == m_childs.end())
		{
			VirtualWndConfig config;
			config.uType = uType;
			m_childs.emplace(name, std::move(config));
		}
		else
		{
			iter->second.uType = uType;
		}
	}

	std::shared_ptr<VirtualWnd> VirtualWndOwner::FindVirtualWnd(const wxString& strName, const std::string& domain)
	{
		auto iter = m_childs.find(domain);
		if (iter == m_childs.end()) {
			return nullptr;
		}

		for (auto it = iter->second.list.begin(); it != iter->second.list.end(); it++)
		{
			if (strName == (*it)->Name())
				return (*it);
		}

		return nullptr;
	}

	void VirtualWndOwner::Show(const std::string& domain, bool bShow)
	{
		m_bShow = bShow;
		m_showName = m_bShow ? domain : "";

		if (true == m_bShow)
		{
			m_rcRefresh.SetPosition({ 0,0 });
			m_rcRefresh.SetSize(m_pOwner->GetSize());
			m_pOwner->Refresh();
		}
	}

	std::string VirtualWndOwner::GetCurrentUIName()
	{
		return m_showName;
	}

	wxRect VirtualWndOwner::GetCameraRect(const wxRect& rect)
	{
		//<! 默认左下角
		wxRect rt{ 0, (int32_t)(rect.GetHeight() / 2), (int32_t)(rect.GetWidth() / 2), (int32_t)(rect.GetHeight() / 2) };

		auto iter = m_childs.find(m_showName);
		if (iter == m_childs.end()) {
			return rt;
		}

		switch (iter->second.uType)
		{
		case ui::CameraPos::eLeftTop:
			rt = wxRect{ 0, 0, (int32_t)(rect.GetWidth() / 2), (int32_t)(rect.GetHeight() / 2) };
			break;
		case ui::CameraPos::eRightTop:
			rt = wxRect{ (int32_t)(rect.GetWidth() / 2), 0, (int32_t)(rect.GetWidth() / 2), (int32_t)(rect.GetHeight() / 2) };
			break;
		case ui::CameraPos::eLeftBottom:
			rt = wxRect{ 0, (int32_t)(rect.GetHeight() / 2), (int32_t)(rect.GetWidth() / 2), (int32_t)(rect.GetHeight() / 2) };
			break;
		case ui::CameraPos::eRightBottom:
			rt = wxRect{ (int32_t)(rect.GetWidth() / 2), (int32_t)(rect.GetHeight() / 2), (int32_t)(rect.GetWidth() / 2), (int32_t)(rect.GetHeight() / 2) };
			break;
		case ui::CameraPos::eFull:
			rt = wxRect{ 0, 0, rect.GetWidth(), rect.GetHeight() };
			break;
		default:
			break;
		}

		return rt;
	}

	void VirtualWndOwner::OnPaint(wxDC& dc)
	{
		if ((false == m_bShow) || m_childs.empty())
			return;

		CreateBufferBitmap();
		if (!m_rcRefresh.IsEmpty())
		{
			wxMemoryDC			memoryDC;

			memoryDC.SelectObject(m_bufferBitmap);
			memoryDC.SetBackground(wxBrush(wxColour(0, 0, 0, 0)));
			memoryDC.Clear();
			DrawVirtualWnd(memoryDC);
			memoryDC.DestroyClippingRegion();
			memoryDC.SelectObject(wxNullBitmap);

			m_rcRefresh.SetPosition({ 0, 0 });
			m_rcRefresh.SetSize({ 0, 0 });
		}

		dc.DrawBitmap(m_bufferBitmap, 0, 0, true);
	}

	void VirtualWndOwner::CreateBufferBitmap()
	{
		wxSize			szButton = m_pOwner->GetClientSize();

		if (!m_bufferBitmap.IsOk())
		{
			m_szBitmap = szButton;
			m_bufferBitmap = wxBitmap(szButton.x, szButton.y);
			m_bufferBitmap.UseAlpha();
		}
		else
		{
			if ((m_szBitmap.x != szButton.x) || (m_szBitmap.y != szButton.y))
			{
				m_szBitmap = szButton;
				m_bufferBitmap = wxBitmap(szButton.x, szButton.y);
				m_bufferBitmap.UseAlpha();
			}
		}
	}

	void VirtualWndOwner::DrawVirtualWnd(wxDC& dc)
	{
		wxGraphicsContext* pGraphic = wxGraphicsContext::CreateFromUnknownDC(dc);

		if (pGraphic)
		{
			pGraphic->SetAntialiasMode(wxAntialiasMode::wxANTIALIAS_NONE);

			auto iter = m_childs.find(m_showName);
			if (iter != m_childs.end())
			{
				for (auto it = iter->second.list.begin(); it != iter->second.list.end(); it++)
				{
					if ((*it)->IsShow())
						(*it)->OnPaint(pGraphic);
				}
			}

			delete pGraphic;
		}
	}

	void VirtualWndOwner::OnLButtonDown(wxMouseEvent& event)
	{
		wxPoint				ptPosition = event.GetPosition();
		bool				bProcessed = false;

		auto iter = m_childs.find(m_showName);
		if (iter != m_childs.end() && true == m_bShow)
		{
			for (auto it = iter->second.list.begin(); it != iter->second.list.end(); it++)
			{
				wxRect			rcRect = (*it)->GetRect();

				if (!(*it)->IsShow())
					continue;

				if (!(*it)->AcceptMouseEvent())
					continue;

				if (PtInRect(ptPosition, rcRect))
				{
					if ((*it)->OnLButtonDown(event))
					{
						if (nullptr == m_pCapture)
						{
							m_pCurrent = *it;
							m_pCapture = *it;
							m_pOwner->CaptureMouse();
						}

						event.Skip(false);

						bProcessed = true;
						break;
					}
				}
			}
		}

		if (!bProcessed)
		{
			if (!MouseInFence(ptPosition))
			{
				wxMouseEvent			mouseEvent(event);

				mouseEvent.SetEventObject(this);
				AddPendingEvent(mouseEvent);
			}
		}
	}

	void VirtualWndOwner::OnLButtonUp(wxMouseEvent& event)
	{
		OnCurrentLeaveWindow(event);
	}

	void VirtualWndOwner::OnMouseMove(wxMouseEvent& event)
	{
		if (false == m_bShow) {
			return OnCurrentLeaveWindow(event);
		}

		wxPoint			ptPosition = event.GetPosition();
		bool			bProcessed = false;

		auto iter = m_childs.find(m_showName);
		if (iter != m_childs.end())
		{
			for (auto it = iter->second.list.begin(); it != iter->second.list.end(); it++)
			{
				wxRect			rcRect = (*it)->GetRect();

				if (!(*it)->IsShow())
					continue;

				if (!(*it)->AcceptMouseEvent())
					continue;

				if (PtInRect(ptPosition, rcRect))
				{
					bool			bEnter = false;

					if (m_pCurrent)
					{
						if (m_pCurrent != (*it))
						{
							OnCurrentLeaveWindow(event);
							bEnter = true;
						}
					}
					else
					{
						bEnter = true;
					}

					if (bEnter)
					{
						(*it)->OnEnterWindow(event);
						m_pCurrent = (*it);
					}

					(*it)->OnMouseMove(event);
					event.Skip(false);
					bProcessed = true;
					break;
				}
			}

			if (!bProcessed) {
				OnCurrentLeaveWindow(event);
			}
		}
	}

	void VirtualWndOwner::OnEnterWindow(wxMouseEvent& event)
	{
		if (false == m_bShow)
			return;
	}

	void VirtualWndOwner::OnLeaveWindow(wxMouseEvent& event)
	{
		OnCurrentLeaveWindow(event);
	}

	void VirtualWndOwner::OnCurrentLeaveWindow(wxMouseEvent& event)
	{
		if (m_pCapture)
		{
			m_pCapture->OnLButtonUp(event);
			m_pCapture = nullptr;
			if (m_pOwner == wxWindow::GetCapture()) {
				m_pOwner->ReleaseMouse();
			}
		}

		if (m_pCurrent)
		{
			m_pCurrent->OnLeaveWindow(event);
			m_pCurrent = nullptr;
			event.Skip(false);
		}
	}

	void VirtualWndOwner::OnLButtonDClick(wxMouseEvent& event)
	{
		wxPoint				ptPosition = event.GetPosition();
		bool				bProcessed = false;

		auto iter = m_childs.find(m_showName);
		if (iter != m_childs.end() && true == m_bShow)
		{
			for (auto it = iter->second.list.begin(); it != iter->second.list.end(); it++)
			{
				wxRect			rcRect = (*it)->GetRect();

				if (!(*it)->IsShow())
					continue;

				if (!(*it)->AcceptMouseEvent())
					continue;

				if (PtInRect(ptPosition, rcRect))
				{
					if ((*it)->OnLButtonDClick(event))
					{
						event.Skip(false);
						bProcessed = true;
						break;
					}
				}
			}
		}

		if (!bProcessed)
		{
			if (!MouseInFence(ptPosition))
			{
				wxMouseEvent			mouseEvent(event);

				mouseEvent.SetEventObject(this);
				AddPendingEvent(mouseEvent);
			}
		}
	}

	uint32_t VirtualWndOwner::AddMouseFence(const wxRect& rcFence)
	{
		uint32_t		uFenceID = m_uLastFenceID++;

		m_mouseFences.insert(std::make_pair(uFenceID, rcFence));
		return uFenceID;
	}

	void VirtualWndOwner::UpdateMouseFence(uint32_t uFenceID, const wxRect& rcFence)
	{
		auto			itFind = m_mouseFences.find(uFenceID);

		if (itFind != m_mouseFences.end())
			itFind->second = rcFence;
	}

	void VirtualWndOwner::RemoveMouseFence(uint32_t uFenceID)
	{
		auto			itFind = m_mouseFences.find(uFenceID);

		if (itFind != m_mouseFences.end())
			m_mouseFences.erase(itFind);
	}

	bool VirtualWndOwner::MouseInFence(const wxPoint& ptMouse)
	{
		for (const auto& mouseFence : m_mouseFences)
		{
			if (mouseFence.second.Contains(ptMouse))
				return true;
		}

		return false;
	}
}