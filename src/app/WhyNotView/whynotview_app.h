#include "wx/wx.h"
#include "logger.h"


// 应用程序类
class WhyNotViewApp : public wxApp
{
public:
    WhyNotViewApp();
    ~WhyNotViewApp() {};
    virtual bool OnInit() wxOVERRIDE;
    int OnExit() wxOVERRIDE;

private:
    bool LoadJcaBladeData();//test
private:
    void InitAppFilePath();
    bool InitAppFile();

    bool StartTimerThread();

private:
    std::string m_strUserConfig_dirPath;

    std::string m_strUserData_dirPath;//data
    std::string m_strDB_dirPath;//db
    std::string m_strLog_dirPath;//log
    
    std::string m_strDataCenterDB_filePath;//data_center.db


};