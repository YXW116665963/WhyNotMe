
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
#include "frame_manager.h"
namespace why
{
	// 菜单枚举量
	enum MenuID
	{
		ID_FILE_OPEN = wxID_HIGHEST + 1,

		ID_DRAW_,

		ID_CONTROL_,

		ID_SETTING_,

		ID_HELP_,
	};


	MainFrame::MainFrame(wxFrame* frame, wxWindowID id, const wxString& title,
		const wxPoint& pos, const wxSize& size, long style)
		: wxFrame(frame, id, title, pos, size, style)
		, m_lastSize(size)
	{
		SetIcon(wxICON(wxpoem));
		SetBackgroundColour(wxColor(240, 240, 240));
		this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnCloseWindow, this);
		this->Bind(wxEVT_TIMER, &MainFrame::OnTimer, this);

		Bind(wxEVT_SIZE, &MainFrame::OnSize, this);

		m_pTimer = new wxTimer(this, wxID_ANY);
		m_pTimer->Start(1000, wxTIMER_CONTINUOUS);



		const int ID_MENU_OPEN = wxID_HIGHEST + 1;
		// 1. 创建菜单
		wxMenu* fileMenu = new wxMenu();
		fileMenu->Append(ID_MENU_OPEN, "打开文件");
		fileMenu->Append(ID_MENU_OPEN + 2, "保存文件");

		

		wxMenu* formatMenu = new wxMenu();
		formatMenu->Append(ID_MENU_OPEN+4, "格式化 JSON");
		formatMenu->Append(ID_MENU_OPEN+5, "格式化 XML");
		fileMenu->AppendSubMenu(formatMenu, "格式");
		



		wxMenu* fileMenu2 = new wxMenu;
		fileMenu2->Append(ID_MENU_OPEN + 1, "打开文件2");


		wxMenuBar* menuBar = new wxMenuBar;
		menuBar->Append(fileMenu, "文件");
		menuBar->Append(fileMenu2, "文件2");
		SetMenuBar(menuBar);

		//
		Bind(wxEVT_MENU, &MainFrame::OnMenuOpen, this, ID_MENU_OPEN);

		LoadMainFrame();
	}

	// 3. 处理函数实现
	void MainFrame::OnMenuOpen(wxCommandEvent& event)
	{
		wxMessageBox("OnMenuOpen！");
	}

	void MainFrame::OnMenuOpen2(wxCommandEvent& event)
	{
		wxMessageBox("OnMenuOpen2！");
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

	void MainFrame::OnSize(wxSizeEvent& event)
	{
		wxSize currentSize = GetSize();
		int currWidth = currentSize.GetWidth();
		int currHeight = currentSize.GetHeight();

		int lastWidth = m_lastSize.GetWidth();
		int lastHeight = m_lastSize.GetHeight();

		bool bExpandH = (currWidth != lastWidth ? true : false);
		bool bExpandV = (currHeight != lastHeight ? true : false);

		

		event.Skip();
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
		pFrame = pRoot->first_node("Frame");
		if (nullptr == pFrame)
		{
			LOG_INFO << "xml file not found frame node";
			return;
		}


		//wxSize DIPSize = FromDIP(size);
		
		nWidth = AttributeAsInt(pFrame, "width", 1920);
		nHeight = AttributeAsInt(pFrame, "height", 1200);
		wxSize size = wxSize(1920, 1200);
		this->SetClientSize(size);

		SINGLETON_PTR(FrameManager)->SetMainFrame(this);

		/*SINGLETON_PTR(FrameManager)->OpenWindow("main_catalog_0_0");*/
	}
	void MainFrame::CreateMenu()
	{


		//Bind(wxEVT_MENU, &你的类::处理函数, this, 菜单ID);

	}

}