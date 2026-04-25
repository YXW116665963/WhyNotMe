#include "data_center_value_util.h"
#include "env_var_data_def.h"
#include "file_util.h"
#include "resource_loader.h"
#include "whynotview_app.h"
#include "wx/stdpaths.h"
#include <filesystem>
#include "file_util.h"
#include "data_center_interface.h"
#include "data_observer_helper.h"
#include "util.h"
#include "global_var.h"
#include "global_pointer.h"
namespace why
{
    namespace
    {
        std::unique_ptr<Logger> g_loggerPtr;
        int32_t g_nLogLevel = -1;
    }

    int WhyNotViewApp::OnExit()
    {
        LOG_INFO << "WhyNotViewApp OnExit";

        if (nullptr != g_globalPointer.m_timePtr.get()) {
            g_globalPointer.m_timePtr->Shutdown();
        }

        return 0;
    }

    bool WhyNotViewApp::LoadwhyBladeData()
    {
        std::string sBldFilePath = g_globalVar.m_strUserData_dirPath;
        sBldFilePath += std::filesystem::path::preferred_separator;
        sBldFilePath += "BladeData_metadata.csv";

        if (!ImportDataFromFile(g_globalPointer.m_dataCenterPtr.get(),
            sBldFilePath.c_str(),
            "bld_edit",
            ""))
        {
            return false;
        }


        return true;
    }


    // 应用程序初始化
    bool WhyNotViewApp::OnInit()
    {
        // 先初始化日志系统，因为后续的初始化过程中可能会有日志输出
        InitLogger();
        // 单独初始化数据库相关的env
        InitDataCenter();
        InitEnvVar();
        InitGlobalTimer();        
        LoadUIResource();

        m_pMainFrame = new MainFrame(NULL,
            wxID_ANY,
            wxT("WHY"),
            wxDefaultPosition,
            { 1280, 1024 },
            //wxMINIMIZE_BOX：右上最小化
            //wxCLOSE_BOX：右上关闭
			//wxFULL_REPAINT_ON_RESIZE：调整窗口大小时重绘整个窗口
            wxCAPTION | wxMINIMIZE_BOX | wxCLOSE_BOX | wxFULL_REPAINT_ON_RESIZE
        );

        m_pMainFrame->Show(true);

        return true;
    }

    // 程序入口点
    wxIMPLEMENT_APP(WhyNotViewApp);

    WhyNotViewApp::WhyNotViewApp()
    {
        g_globalVar.m_strAppName = "whynotview";
    }

    void WhyNotViewApp::InitEnvVar()
    {
        SetMainThread();
        PathAppender pathAppender;
        // 数据def文本文件
        std::string strDataDef_dirPath = pathAppender.SetSourcePath(GetStringValue(envVar::g_Domain, envVar::strUserConfig_dirPath))
			.AppendChildPath("data")
			.GetPath();

        g_globalPointer.m_dataCenterPtr->
            RegisterStringData(envVar::g_Domain, envVar::strDataDef_dirPath, strDataDef_dirPath.c_str(), DataStyle::ePersistence);

        if (!CreateDirectories(strDataDef_dirPath))
        {
            LOG_ERROR << "create " << strDataDef_dirPath << " failed!!!";
            return;
        }

        // glog日志
		std::string strLog_dirPath = pathAppender.SetSourcePath(GetStringValue(envVar::g_Domain, envVar::strUserConfig_dirPath))
			.AppendChildPath("log")
			.GetPath();

        g_globalPointer.m_dataCenterPtr->
            RegisterStringData(envVar::g_Domain, envVar::strLog_dirPath, strLog_dirPath.c_str(), DataStyle::ePersistence);

        // 部署路径：exe所在文件夹路径
        std::string strDeployment_dirPath = GetDirectory(wxStandardPaths::Get().GetExecutablePath().utf8_string());

        g_globalPointer.m_dataCenterPtr->
            RegisterStringData(envVar::g_Domain, envVar::strDeployment_dirPath, strDeployment_dirPath.c_str(), DataStyle::ePersistence);
        // UI XML
        std::string strUIXml_dirPath = pathAppender.SetSourcePath(strDeployment_dirPath)
            .AppendChildPath("Resource")
            .AppendChildPath("XMLFiles")
            .GetPath();
        g_globalPointer.m_dataCenterPtr->
            RegisterStringData(envVar::g_Domain, envVar::strUIXml_dirPath, strUIXml_dirPath.c_str(), DataStyle::ePersistence);
		// UI 图片
        std::string strUIPicture_dirPath = pathAppender.SetSourcePath(strDeployment_dirPath)
            .AppendChildPath("Resource")
            .AppendChildPath("Pictures")
            .GetPath();
        g_globalPointer.m_dataCenterPtr->
            RegisterStringData(envVar::g_Domain, envVar::strUIPicture_dirPath, strUIPicture_dirPath.c_str(), DataStyle::ePersistence);

        // UI Resource.xml:字体背景等资源
        std::string strUIResourceXml_filePath = pathAppender.SetSourcePath(strDeployment_dirPath)
            .AppendChildPath("Resource")
            .AppendChildPath("XMLFiles")
            .AppendChildPath("Resource.xml")
            .GetPath();
        g_globalPointer.m_dataCenterPtr->
            RegisterStringData(envVar::g_Domain, envVar::strUIResourceXml_filePath, strUIResourceXml_filePath.c_str(), DataStyle::ePersistence);
    }

    bool WhyNotViewApp::InitGlobalTimer()
    {
        g_globalPointer.m_timePtr.reset(new Timer(false, "timer"));
        if (!g_globalPointer.m_timePtr->Startup())
        {
            LOG_ERROR << "start time thread failed";
            return false;
        }

        return true;
    }

    bool WhyNotViewApp::InitLogger()
    {
        PathAppender pathAppender;
        std::string strLog_dirPath = pathAppender.SetSourcePath(wxStandardPaths::Get().GetUserConfigDir().utf8_string())
            .AppendChildPath(g_globalVar.m_strAuthor)
            .AppendChildPath(g_globalVar.m_strAppName)
            .AppendChildPath("log")
            .GetPath();

        g_loggerPtr.reset(new Logger(strLog_dirPath, g_globalVar.m_strAppName, g_nLogLevel));
        if (!CreateDirectories(strLog_dirPath))
        {
            LOG_ERROR << "create " << strLog_dirPath << " failed!!!";
            return false;
        }

        return true;
    }

    bool WhyNotViewApp::InitDataCenter()
    {		
        PathAppender pathAppender;
		// exe和数据中心路径
        std::string strUserConfig_dirPath = pathAppender.SetSourcePath(wxStandardPaths::Get().GetUserConfigDir().utf8_string())
            .AppendChildPath(g_globalVar.m_strAuthor)
            .AppendChildPath(g_globalVar.m_strAppName)
            .GetPath();

        // 数据库目录和数据中心路径
        std::string strDataBase_dirPath = pathAppender.SetSourcePath(strUserConfig_dirPath)
			.AppendChildPath("db")
            .GetPath();        

        std::string strDataCenter_filePath = pathAppender.SetSourcePath(strDataBase_dirPath)
            .AppendChildPath("data_center.db")
            .GetPath();

		// 创建数据中心数据库目录和数据中心文件
        if (!CreateDirectories(strDataBase_dirPath))
        {
            LOG_ERROR << "create " << strDataBase_dirPath << " failed!!!";
            return false;
        }
        IDataCenter* pIDataCenter = CreateDataCenter(strDataCenter_filePath.c_str());
        if (nullptr == pIDataCenter)
        {
            LOG_ERROR << "create " << strDataCenter_filePath << " failed!!!";
            return false;
        }
        g_globalPointer.m_dataCenterPtr.reset(pIDataCenter);

        // 注册数据库文件夹路径
        g_globalPointer.m_dataCenterPtr->
            RegisterStringData(envVar::g_Domain, envVar::strUserConfig_dirPath, strUserConfig_dirPath.c_str(), DataStyle::ePersistence);

        // 注册数据库文件夹路径
        g_globalPointer.m_dataCenterPtr->
            RegisterStringData(envVar::g_Domain, envVar::strDataBase_dirPath, strDataBase_dirPath.c_str(), DataStyle::ePersistence);

        // 注册数据中心路径
        g_globalPointer.m_dataCenterPtr->
            RegisterStringData(envVar::g_Domain, envVar::strDataCenter_filePath, strDataCenter_filePath.c_str(), DataStyle::ePersistence);

        return true;
    }

    bool WhyNotViewApp::LoadUIResource()
    {
        std::string strUIResourceXml_filePath = GetStringValue(envVar::g_Domain, envVar::strUIResourceXml_filePath);

        if (!LoadResource(strUIResourceXml_filePath.c_str()))
        {
            LOG_ERROR << "load ui resource failed";
            return false;
        }
        
        return true;
    }
}

