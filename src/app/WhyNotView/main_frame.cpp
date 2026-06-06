
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
		//测试界面功能
		ID_TEST_REPAINT_PAGE = wxID_HIGHEST + 1,

		//File
		ID_FILE_OPEN = wxID_HIGHEST + 100,
		ID_FILE_SAVE = wxID_HIGHEST + 101,



		ID_DRAW_,

		ID_CONTROL_,

		ID_SETTING_,

		ID_HELP_,
	};


	void MainFrame::InitMenu()
	{		
		wxMenuBar* menuBar = new wxMenuBar;		
		//File
		wxMenu* fileMenu = new wxMenu;
		
		fileMenu->Append(ID_FILE_OPEN, "打开文件");
		Bind(wxEVT_MENU, &MainFrame::OnMenuOpenFile, this, ID_FILE_OPEN);

		fileMenu->Append(ID_FILE_SAVE, "保存文件");
		Bind(wxEVT_MENU, &MainFrame::OnMenuSaveFile, this, ID_FILE_SAVE);
		//File-格式
		wxMenu* formatMenu = new wxMenu();
		formatMenu->Append(wxID_HIGHEST + 4, "格式化 JSON");
		formatMenu->Append(wxID_HIGHEST + 5, "格式化 XML");
		fileMenu->AppendSubMenu(formatMenu, "格式");
		menuBar->Append(fileMenu, "File");

		wxMenu* drawMenu = new wxMenu;
		menuBar->Append(drawMenu, "Draw");

		wxMenu* controlMenu = new wxMenu;
		menuBar->Append(controlMenu, "Control");

		wxMenu* settingMenu = new wxMenu;
		menuBar->Append(settingMenu, "Setting");

		wxMenu* helpMenu = new wxMenu;
		menuBar->Append(helpMenu, "Help");

		//Test
		wxMenu* testMenu = new wxMenu;

		testMenu->Append(ID_TEST_REPAINT_PAGE, "界面重绘");
		Bind(wxEVT_MENU, &MainFrame::OnMenuRepaintPage, this, ID_TEST_REPAINT_PAGE);

		menuBar->Append(testMenu, "Test");


		SetMenuBar(menuBar);
	}



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


		InitMenu();


		LoadMainFrame();
	}

	void MainFrame::OnMenuOpenFile(wxCommandEvent& event)
	{
		wxMessageBox("OnMenuOpenFile！");
	}

	void MainFrame::OnMenuSaveFile(wxCommandEvent& event)
	{
		wxMessageBox("OnMenuSaveFile！");
	}

	void MainFrame::OnMenuRepaintPage(wxCommandEvent& event)
	{
		SINGLETON_PTR(FrameManager)->RepaintCurWindow();
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
}