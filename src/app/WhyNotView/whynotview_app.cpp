#include "whynotview_app.h"
#include "wx/stdpaths.h"
#include <filesystem>
#include "file_util.h"
#include "data_center_interface.h"
#include "data_observer_helper.h"
#include "global_def.h"
#include "util.h"



#define WHYNOTVIEW "whynotview"
namespace
{
    std::unique_ptr<why::Logger> g_loggerPtr;
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

bool WhyNotViewApp::LoadJcaBladeData()
{
    std::string sBldFilePath = m_strUserData_dirPath;
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
    InitAppFilePath();
    InitAppFile();
    why::SetMainThread();

    StartTimerThread();
    LoadJcaBladeData();
    

    return true;
}

// 程序入口点
wxIMPLEMENT_APP(WhyNotViewApp);

WhyNotViewApp::WhyNotViewApp()
{

}

void WhyNotViewApp::InitAppFilePath()
{
    //
    m_strUserConfig_dirPath = wxStandardPaths::Get().GetUserConfigDir().utf8_str();
    m_strUserConfig_dirPath += std::filesystem::path::preferred_separator;
    m_strUserConfig_dirPath += "why";
    m_strUserConfig_dirPath += std::filesystem::path::preferred_separator;
    m_strUserConfig_dirPath += WHYNOTVIEW;

    //
    m_strUserData_dirPath = m_strUserConfig_dirPath;
    m_strUserData_dirPath += std::filesystem::path::preferred_separator;
    m_strUserData_dirPath += "data";

    m_strDB_dirPath = m_strUserConfig_dirPath;
    m_strDB_dirPath += std::filesystem::path::preferred_separator;
    m_strDB_dirPath += "db";

    m_strDataCenterDB_filePath = m_strDB_dirPath;
    m_strDataCenterDB_filePath += std::filesystem::path::preferred_separator;
    m_strDataCenterDB_filePath += "data_center.db";

    //
    m_strLog_dirPath = m_strUserConfig_dirPath;
    m_strLog_dirPath += std::filesystem::path::preferred_separator;
    m_strLog_dirPath += "log";
}

bool WhyNotViewApp::InitAppFile()
{
    if (!why::CreateDirectories(m_strUserConfig_dirPath))
    {
        return false;
    }

    g_loggerPtr.reset(new why::Logger(m_strLog_dirPath, WHYNOTVIEW, g_nLogLevel));

    if (!why::CreateDirectories(m_strUserData_dirPath))
    {
        return false;
    }

    if (!why::CreateDirectories(m_strDB_dirPath))
    {
        return false;
    }

    if (!why::CreateDirectories(m_strLog_dirPath))
    {
        return false;
    }

    why::IDataCenter* pIDataCenter = CreateDataCenter(m_strDataCenterDB_filePath.c_str());

    if (nullptr == pIDataCenter)
    {
        LOG_ERROR << "WhyNotViewApp::InitDataBase() failed!";
        return false;
    }

    g_globalPointer.m_dataCenterPtr.reset(pIDataCenter);


    return true;
}

bool WhyNotViewApp::StartTimerThread()
{
    g_globalPointer.m_timePtr.reset(new why::Timer(false, "timer"));
    if (!g_globalPointer.m_timePtr->Startup())
    {
        LOG_ERROR << "start time thread failed";
        return false;
    }

    return true;
}
