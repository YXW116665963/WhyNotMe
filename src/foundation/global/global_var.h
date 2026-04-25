#include <iostream>

struct GlobalVar
{
    //UserConfigDir
    std::string m_strUserConfig_dirPath;            // C:...whynotview
    std::string m_strUserData_dirPath;              // C:...whynotview/data
    std::string m_strDB_dirPath;                    // C:...whynotview/db
    std::string m_strDataCenter_dbPath;             // C:...whynotview/db/data_center.db
    std::string m_strLog_dirPath;                   // C:...whynotview/log
    //

    //AppConfigDir
    std::string m_strAppConfig_dirPath;            // D:...whynotview
    //

    std::string m_strExecutable_exePath;

    // UI
    std::string m_strPictures_dirPath;
    std::string m_strXml_dirPath;
    std::string m_strResourceXml_filePath;


    // use
    std::string m_strAuthor{ "why" };
    std::string m_strAppName;
};

inline GlobalVar g_globalVar;

inline GlobalVar* const GetGlobalVar()
{
    return &g_globalVar;
}
