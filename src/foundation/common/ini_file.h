#pragma once

#include "standard.h"

#include <vector>
#include <map>
#include <string>

namespace why
{
    struct IniItem;
    class IniFile
    {
    public:
        IniFile();
        ~IniFile();

        /**
        * @brief 打开配置文件
        * @param lpFileName 等待打开的文件名称
        * @return
        * @ - true 文件打开并且解析成功
        * @ - false 文件打开或解析失败
        */
        bool Open(const char* lpFileName);

        /**
        * @brief 将改动保存到文件中
        * @return void
        */
        bool Save();

        /**
        * @brief 将配置保存到另一个文件
        * @return void
        */
        bool SaveAs(const char* lpFileName);

        /**
        * @brief 读取配置项
        * @param lpDomain 配置分类名称
        * @param lpName 配置名称
        * @param nDefault 当配置不存在时的默认配置值
        * @return 配置的值，如果配置项不存在，返回的值总是等于nDefault
        */
        int32_t ReadInt(const char* lpDomain, const char* lpName, int32_t nDefault);

        /**
        * @brief 读取配置项
        * @param lpDomain 配置分类名称
        * @param lpName 配置名称
        * @param lpDefault 当配置不存在时的默认配置值
        * @return 配置的值，如果配置项不存在，返回的值总是等于nDefault
        */
        const char* ReadString(const char* lpDomain, const char* lpName, const char* lpDefault);

        /**
        * @brief 写入配置项
        * @param lpDomain 配置分类名称
        * @param lpName 配置名称
        * @param nValue 配置项新值
        * @return
        *  - true 写入成功
        *  - false 写入失败
        * @warning 如果配置项已经存在，使用新值取代旧值，如果配置项不存在，则使用新值新建一个配置项
        */
        bool WriteInt(const char* lpDomain, const char* lpName, int32_t nValue);

        /**
        * @brief 写入配置项
        * @param lpDomain 配置分类名称
        * @param lpName 配置名称
        * @param lpValue 配置项新值
        * @return
        *  - true 写入成功
        *  - false 写入失败
        * @warning 如果配置项已经存在，使用新值取代旧值，如果配置项不存在，则使用新值新建一个配置项
        */
        bool WriteString(const char* lpDomain, const char* lpName, const char* lpValue);

    public:
        typedef std::vector<IniItem> ItemArray;
        typedef ItemArray::iterator ItemIt;
        typedef std::map<std::string, ItemArray> ItemMap;
        typedef ItemMap::iterator ItemMapIt;

    private:
        bool SaveFile(const std::string &strFileName);

    private:
        std::string			m_strFileName;
        ItemMap				m_itemMap;
    };

    struct  IniItem
    {
        std::string		m_strName;
        std::string		m_strValue;

        IniItem() {}
        IniItem(const IniItem &other)
        {
            m_strName = other.m_strName;
            m_strValue = other.m_strValue;
        }

        IniItem& operator=(const IniItem &other)
        {
            if (this != &other)
            {
                m_strName = other.m_strName;
                m_strValue = other.m_strValue;
            }
            return *this;
        }
    };

    class AnalyzeIni
    {
    public:
        AnalyzeIni(IniFile *pIniFile)
        {
            m_pIniFile = pIniFile;
        }

        void operator()(const std::string &strLine)
        {
            size_t			    nFirst = strLine.find('[');
            size_t			    nLast = strLine.rfind(']');
            size_t			    nSpaceTab = strLine.find_first_not_of(" \t");
            std::string		    strDomain;
            std::string		    strLeftPart;
            std::string		    strRightPart;
            std::string		    strName;
            std::string			strValue;

            if ((std::string::npos != nFirst) && (std::string::npos != nLast) && (nFirst != (nLast + 1)) && (nFirst == nSpaceTab))
            {
                m_strDomain = strLine.substr(nFirst + 1, nLast - nFirst - 1);
                return;
            }

            if (m_strDomain.empty())
                return;

            nFirst = strLine.find('=');
            if (std::string::npos == nFirst)
                return;

            strLeftPart = strLine.substr(0, nFirst);
            strRightPart = strLine.substr(nFirst + 1, std::string::npos);

            nFirst = strLeftPart.find_first_not_of(" \t");
            nLast = strLeftPart.find_last_not_of(" \t");
            if ((std::string::npos == nFirst) || (std::string::npos == nLast))
                return;
            strName = strLeftPart.substr(nFirst, nLast - nFirst + 1);

            nFirst = strRightPart.find_first_not_of(" \t");
            if
                    (((nLast = strRightPart.find("\t#", nFirst)) != std::string::npos) ||
                     ((nLast = strRightPart.find(" #", nFirst)) != std::string::npos) ||
                     ((nLast = strRightPart.find("\t//", nFirst)) != std::string::npos) ||
                     ((nLast = strRightPart.find(" //", nFirst)) != std::string::npos))
            {
                strRightPart = strRightPart.substr(0, nLast - nFirst);
            }
            nLast = strRightPart.find_last_not_of(" \t");
            if (nFirst == std::string::npos || nLast == std::string::npos)
            {
                strRightPart = "";
                strValue = "";
            }
            else
            {
                strValue = strRightPart.substr(nFirst, nLast - nFirst + 1);
            }

            m_pIniFile->WriteString(m_strDomain.c_str(), strName.c_str(), strValue.c_str());
            return;
        }

    public:
        std::string			m_strDomain;
        IniFile				*m_pIniFile;
    };
}
