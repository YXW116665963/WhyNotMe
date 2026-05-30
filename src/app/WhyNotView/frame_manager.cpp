#include "event_dispatcher.h"

#include "logger.h"
#include "frame_manager.h"

#include "main_catalog.h"
namespace why
{
	WindowManager::WindowManager()
	{
		m_mapWindowFactory = {
			{"main_catalog_0_0", new FrameFactory<MainCatalog>()},
		};
	}

	WindowManager::~WindowManager()
	{
		
	}

	void WindowManager::OpenWindow(const std::string& strWindowName)
	{
		auto itFind = m_mapWindowFactory.find(strWindowName);
		if (m_mapWindowFactory.end() != itFind)
		{
			WindowInfo newWindow;
			newWindow.m_strWindowName = strWindowName;

			wxPoint framePoint = m_pMainFrame->GetPosition();
			wxSize frameSize = m_pMainFrame->GetClientSize();

			newWindow.m_pPanel = itFind->second->CreatePanel(m_pMainFrame, framePoint, frameSize);

			m_curWindowInfo = newWindow;
			m_curWindowInfo.m_pPanel->InitializePanel();
			m_curWindowInfo.m_pPanel->Show();
			m_curWindowInfo.m_pPanel->EnablePanelEx(!m_curWindowInfo.m_bReadOnly);
			LOG_INFO << "OpenWindow: " << strWindowName;
		}
		else
		{
			LOG_ERROR << "OpenWindow failed by no window:" << strWindowName;
		}
	}

	void WindowManager::CloseWindow(const WindowInfo& windowInfo)
	{
		GetEventDispatcher()->DoItOnMainThreadAsync([windowInfo]() {
			if (windowInfo.m_pPanel)
				windowInfo.m_pPanel->Close();

			LOG_INFO << "DestroyWindowInfo name:" << windowInfo.m_strWindowName;
			}, this);
	}
	void WindowManager::OnSize()
	{
	}
}