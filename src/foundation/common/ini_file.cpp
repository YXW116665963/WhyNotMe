#include "ini_file.h"

#include <fstream>
#include <sstream>
#include <algorithm>

namespace why
{
	IniFile::IniFile()
	{

	}

	IniFile::~IniFile()
	{

	}

	bool IniFile::Open(const char* lpFileName)
	{
		std::ifstream			inFile;

		m_strFileName = lpFileName;
		inFile.open(m_strFileName.c_str());
		if (!inFile.is_open())
			return false;

		std::vector<std::string>	stringArray;
		std::string					strLine;
		size_t						nFirst;

		while (!inFile.eof())
		{
			std::getline(inFile, strLine, '\n');
			if (!strLine.empty())
			{
				nFirst = strLine.find_first_not_of(" \t");
				if (';' != strLine[nFirst])
					stringArray.push_back(strLine);
			}
		}

		if (stringArray.empty())
			return false;

		m_itemMap.clear();
		std::for_each(stringArray.begin(), stringArray.end(), AnalyzeIni(this));
		return !m_itemMap.empty();
	}

	bool IniFile::Save()
	{
		if (m_itemMap.empty() || m_strFileName.empty())
			return false;

		return SaveFile(m_strFileName);
	}

	bool IniFile::SaveAs(const char* lpFileName)
	{
		if ((m_itemMap.empty()) || (nullptr == lpFileName) || (0 == strlen(lpFileName)))
			return false;

		return SaveFile(lpFileName);
	}

	const char* IniFile::ReadString(const char* lpDomain, const char* lpName, const char* lpDefault)
	{
		ItemMapIt			itFind;

		itFind = m_itemMap.find(std::string(lpDomain ? lpDomain : ""));
		if (itFind == m_itemMap.end())
			return lpDefault;

		for (ItemIt it = itFind->second.begin(); it != itFind->second.end(); it++)
		{
			if (it->m_strName == lpName)
				return it->m_strValue.c_str();
		}

		return lpDefault;
	}

	bool IniFile::WriteString(const char* lpDomain, const char* lpName, const char* lpValue)
	{
		ItemMapIt			itFind;
		IniItem				item;

		itFind = m_itemMap.find(std::string(lpDomain));
		if (itFind == m_itemMap.end())
		{
			ItemArray		itemArray;

			item.m_strName = lpName;
			item.m_strValue = lpValue;
			itemArray.push_back(item);
			m_itemMap[std::string(lpDomain)] = itemArray;
			return true;
		}
		else
		{
			for (ItemIt it = itFind->second.begin(); it != itFind->second.end(); it++)
			{
				if (it->m_strName == lpName)
				{
					it->m_strValue = lpValue;
					return true;
				}
			}

			item.m_strName = lpName;
			item.m_strValue = lpValue ? lpValue : "";
			itFind->second.push_back(item);
			return true;
		}
	}

	int32_t IniFile::ReadInt(const char* lpDomain, const char* lpName, int32_t nDefault)
	{
		ItemMapIt			itFind;

		itFind = m_itemMap.find(std::string(lpDomain ? lpDomain : ""));
		if (itFind == m_itemMap.end())
			return nDefault;

		for (ItemIt it = itFind->second.begin(); it != itFind->second.end(); it++)
		{
			if (it->m_strName == lpName)
				return atoi(it->m_strValue.c_str());
		}

		return nDefault;
	}

	bool IniFile::WriteInt(const char* lpDomain, const char* lpName, int32_t nValue)
	{
		std::stringstream       stringStream;

		stringStream << nValue;
		return WriteString(lpDomain, lpName, stringStream.str().c_str());
	}

	bool IniFile::SaveFile(const std::string &strFileName)
	{
		std::ofstream			outFile(strFileName.c_str());
		std::string				strDomain;
		std::string				strName;
		std::string				strValue;

		if (!outFile.is_open())
			return false;

		for (ItemMapIt itMap = m_itemMap.begin(); itMap != m_itemMap.end(); itMap++)
		{
			outFile << "[" << itMap->first << "]" << std::endl;
			for (ItemIt itItem = itMap->second.begin(); itItem != itMap->second.end(); itItem++)
				outFile << itItem->m_strName << " = " << itItem->m_strValue << std::endl;
		}

		return true;
	}
}