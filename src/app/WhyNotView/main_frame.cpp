
#include "util.h"
#include "xml_util.h"
#include "file_util.h"
#include "singleton.h"
#include "main_frame.h"
#include "event_dispatcher.h"
#include "global_pointer.h"
#include "data_center_value_util.h"
#include "env_var_data_def.h"
#include "logger.h"
#include <filesystem>
#include "window_manager.h"
namespace why
{
	MainFrame::MainFrame(wxFrame* frame, wxWindowID id, const wxString& title,
		const wxPoint& pos, const wxSize& size, long style)
		: wxFrame(frame, id, title, pos, size, style)
	{
		SetIcon(wxICON(wxpoem));
		SetBackgroundColour(wxColor(240, 240, 240));
		this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnCloseWindow, this);
		this->Bind(wxEVT_TIMER, &MainFrame::OnTimer, this);

		//SetDefaultEditBackground("edit_bk", "edit_readonly_bk");

		m_pTimer = new wxTimer(this, wxID_ANY);
		m_pTimer->Start(1000, wxTIMER_CONTINUOUS);

		//GetEventDispatcher()->DoItOnMainThreadAsync()

		LoadMainFrame();
	}

	MainFrame::~MainFrame()
	{

	}
	void MainFrame::OnCloseWindow(wxCloseEvent& event)
	{
		if (m_pTimer)
		{
			m_pTimer->Stop();
			delete m_pTimer;
			m_pTimer = nullptr;
		}

		this->Destroy();
	}

	void MainFrame::OnTimer(wxTimerEvent& event)
	{

	}

	void MainFrame::LoadMainFrame()
	{
		PathAppender pathAppender;
		std::string	strMainFrame_filePath
			= pathAppender.SetSourcePath(GetStringValue(envVar::g_Domain, envVar::strUIXml_dirPath))
			.AppendChildPath("MainFrame.xml")
			.GetPath();

		LoadFrame(strMainFrame_filePath);
	}

	void MainFrame::LoadFrame(const std::string& strFrameXml_filePath)
	{
		if (!std::filesystem::exists(strFrameXml_filePath))
		{
			LOG_ERROR << "SearchResourceFullPath not find:" << strFrameXml_filePath;
			return;
		}

		std::string						strXMLFileName = UTF8ToLocal(strFrameXml_filePath);
		rapidxml::file<char>			fdoc(strXMLFileName.c_str());
		rapidxml::xml_document<char>	doc;
		CXmlNode* pRoot					= nullptr;
		CXmlNode* pFrame				= nullptr;
		int32_t							nWidth = 1280;
		int32_t							nHeight = 1080;
		wxRect							rcClient;
		wxRect							rcPart;

		try
		{
			doc.parse<0>(fdoc.data());
		}
		catch (const rapidxml::parse_error& ex)
		{
			LOG_ERROR << "invalidate xml file:" << strFrameXml_filePath << ",where:" << ex.where<char>();
			throw ex;
		}
		catch (const std::exception& e)
		{
			LOG_ERROR << "invalidate xml file:" << strFrameXml_filePath;
			throw e;
		}
		pRoot = doc.first_node();
		if (nullptr == pRoot)
		{
			LOG_INFO << "invalidate xml file:" << strFrameXml_filePath;
			return;
		}

		nWidth = AttributeAsInt(pFrame, "width", 1280);
		nHeight = AttributeAsInt(pFrame, "height", 1024);
		this->SetSize(nWidth, nHeight);
		rcClient = GetClientRect();

		SINGLETON_PTR(WindowManager)->SetMainFrame(this);

		SINGLETON_PTR(WindowManager)->OpenWindow("main_catalog_0_0");
	}

	
}