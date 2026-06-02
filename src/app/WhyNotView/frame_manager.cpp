#include "event_dispatcher.h"

#include "logger.h"
#include "frame_manager.h"

#include "main_catalog.h"
namespace why
{
	FrameManager::FrameManager()
	{
		// 窗口名小写，板块名大写
		m_mapPanelFactory = {
			{"main_catalog_0_0", new PanelFactory<MainCatalog>()},
		};
	}

	FrameManager::~FrameManager()
	{
		
	}

	void FrameManager::OpenWindow(const std::string& strWindowName)
	{
		auto itFind = m_mapPanelFactory.find(strWindowName);
		if (m_mapPanelFactory.end() != itFind)
		{
			wxPoint framePoint = m_pMainFrame->GetPosition();
			wxSize frameSize = m_pMainFrame->GetClientSize();

			m_curWindowInfo.m_vecPanels = itFind->second->CreatePanels(m_pMainFrame);
			m_curWindowInfo.m_strWindowName = strWindowName;

			for (auto pPanel : m_curWindowInfo.m_vecPanels)
			{
				pPanel->InitializePanel();
				pPanel->Show();
				pPanel->EnablePanelEx(!m_curWindowInfo.m_bReadOnly);
			}
			LOG_INFO << "OpenWindow: " << strWindowName;
		}
		else
		{
			LOG_ERROR << "OpenWindow failed by no window:" << strWindowName;
		}
	}

	void FrameManager::CloseWindow(const WindowInfo& windowInfo)
	{
		GetEventDispatcher()->DoItOnMainThreadAsync([windowInfo]() {

			for (auto pPanel : windowInfo.m_vecPanels)
			{
				if (pPanel)
					pPanel->Close();
			}
			LOG_INFO << "DestroyWindowInfo name:" << windowInfo.m_strWindowName;
			}, this);
	}

	void FrameManager::OnSize()
	{
	}
}