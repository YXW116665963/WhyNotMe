#include "test_dataloader.h"
#include "wx/wx.h"
#include "wx/stdpaths.h"
#include <filesystem>
#include "text_util.h"
#include "file_util.h"

DataCenterPtr m_dataCenterPtr{ nullptr, [](why::IDataCenter* pDataCenter) { pDataCenter->Destroy(); } };

const std::map<std::string, why::ValueType> g_valueTypeMap =
{
	{"int32_t", why::ValueType::eInt},
	{"uint32_t", why::ValueType::eUInt},
	{"int64_t", why::ValueType::eBigInt},
	{"uint64_t", why::ValueType::eBigUInt},
	{"float", why::ValueType::eFloat},
	{"string", why::ValueType::eString},
	{"object", why::ValueType::eObject},
};

bool GetValueType(const std::string& strType, why::ValueType& eType)
{
	auto				itFind = g_valueTypeMap.find(strType);

	if (g_valueTypeMap.end() != itFind)
	{
		eType = itFind->second;
		return true;
	}
	else
	{
		return false;
	}
}

namespace test_
{



	test_DataLoader::test_DataLoader()
	{
		m_strConfigPath = wxStandardPaths::Get().GetUserConfigDir().utf8_str();
		m_strConfigPath += std::filesystem::path::preferred_separator;
		m_strConfigPath += "why";

		m_strTestDirAbsPath = m_strConfigPath;
		m_strTestDirAbsPath += std::filesystem::path::preferred_separator;
		m_strTestDirAbsPath += "test";

		m_strDBFileAbsPath = m_strTestDirAbsPath;
		m_strDBFileAbsPath += std::filesystem::path::preferred_separator;
		m_strDBFileAbsPath += "data_center.db";
	}

	bool test_DataLoader::InitDataCenter()
	{
		if (!why::CreateDirectories(m_strConfigPath))
		{
			return false;
		}

		if (!why::CreateDirectories(m_strTestDirAbsPath))
		{
			return false;
		}

		why::IDataCenter* pIDataCenter = CreateDataCenter(m_strDBFileAbsPath.c_str());
		if (nullptr == pIDataCenter)
		{
			return false;
		}

		m_dataCenterPtr.reset(pIDataCenter);
		m_dataCenterPtr->RegisterStringData("path", "m_strTestDirAbsPath", m_strTestDirAbsPath.c_str(), why::DataStyle::ePersistence);
		m_pDataCenter = m_dataCenterPtr.get();
		return true;
	}


	bool test_DataLoader::ImportDataFromFile(why::IDataCenter* pDataCenter, const char* pDataFile, const char* pDomain, const char* pDB)
	{
		LOG_INFO << "ImportDataFromFile file:" << pDataFile << ",to:" << pDomain;
		std::vector<std::vector<std::string>>		dataList;
		if (!why::LoadCSVFile(pDataFile, dataList))
		{
			LOG_ERROR << "data file:" << pDataFile << " load is failed";
			return false;
		}
		uint32_t uLineCount{ 0 };

		uLineCount = (uint32_t)dataList.size();
		for (uint32_t i = 1; i < uLineCount; i++)
		{
			std::vector<std::string>& lineInfo = dataList[i];

			if (lineInfo.size() < 6) {
				continue;
			}

			if (lineInfo[0].empty()) {
				lineInfo[0] = pDomain;
			}
			if (nullptr != pDB && (0 != strlen(pDB))) {
				lineInfo[6] = pDB;
			}
		}
		return LoadAllData(dataList);
	}

	bool test_DataLoader::LoadAllData(std::vector<std::vector<std::string>>& dataList)
	{
		return false;
	}


	bool test_DataLoader::LoadOneData(std::vector<std::string>& data)
	{
		uint32_t uItemSize = (uint32_t)data.size();
		why::ValueType eType{ why::ValueType::eInt };
		if (data[3].empty())
			return false;

		if (!GetValueType(data[2], eType))
		{
			LOG_WARNING << " has bad vale type: " << data[3];
			return false;
		}

		if (data[0].empty() || data[1].empty())
		{
			LOG_WARNING << " has not domain or name";
			return false;
		}

		RegisterData(
			data[0],
			data[1],
			eType,
			data[3],
			why::ICaseCompare(data[4], "true")
		);

		return true;
	}

	void test_DataLoader::RegisterData(const std::string& strDomain, const std::string& strName, why::ValueType eType
		, const std::string& strDefault, bool bPersistence)
	{
		why::DataStyle eStyle = bPersistence ? why::DataStyle::ePersistence : why::DataStyle::eNormal;
		switch (eType)
		{
		case why::ValueType::eInt:
			if (!m_pDataCenter->RegisterIntData(strDomain.c_str(), strName.c_str(), (int32_t)std::stoi(strDefault), eStyle))
			{
				why::IDataValue* pValue = m_pDataCenter->GetDataValue(strDomain.c_str(), strName.c_str());
				if (pValue) {
					pValue->SetIntValue((int32_t)std::stoi(strDefault));
				}
			}
			break;
		case why::ValueType::eUInt:
			if (!m_pDataCenter->RegisterUIntData(strDomain.c_str(), strName.c_str(), (uint32_t)std::stoi(strDefault), eStyle))
			{
				why::IDataValue* pValue = m_pDataCenter->GetDataValue(strDomain.c_str(), strName.c_str());
				if (pValue) {
					pValue->SetUIntValue((uint32_t)std::stoi(strDefault));
				}
			}
			break;
		case why::ValueType::eBigInt:
			if (!m_pDataCenter->RegisterBigIntData(strDomain.c_str(), strName.c_str(), (int64_t)std::stoll(strDefault), eStyle))
			{
				why::IDataValue* pValue = m_pDataCenter->GetDataValue(strDomain.c_str(), strName.c_str());
				if (pValue) {
					pValue->SetBigIntValue((int64_t)std::stoll(strDefault));
				}
			}
			break;
		case why::ValueType::eBigUInt:
			if (!m_pDataCenter->RegisterBigUIntData(strDomain.c_str(), strName.c_str(), (uint64_t)std::stoul(strDefault), eStyle))
			{
				why::IDataValue* pValue = m_pDataCenter->GetDataValue(strDomain.c_str(), strName.c_str());
				if (pValue) {
					pValue->SetBigUIntValue((uint64_t)std::stoul(strDefault));
				}
			}
			break;
		case why::ValueType::eFloat:
			if (!m_pDataCenter->RegisterFloatData(strDomain.c_str(), strName.c_str(), std::stod(strDefault), eStyle))
			{
				why::IDataValue* pValue = m_pDataCenter->GetDataValue(strDomain.c_str(), strName.c_str());
				if (pValue) {
					pValue->SetFloatValue(std::stod(strDefault));
				}
			}
			break;
		case why::ValueType::eString:
			if (!m_pDataCenter->RegisterStringData(strDomain.c_str(), strName.c_str(), strDefault.c_str(), eStyle))
			{
				why::IDataValue* pValue = m_pDataCenter->GetDataValue(strDomain.c_str(), strName.c_str());
				if (pValue) {
					pValue->SetStringValue(strDefault.c_str());
				}
			}
			break;
		case why::ValueType::eList:
			break;
		case why::ValueType::eObject:
			m_pObjectValue = m_pDataCenter->RegisterObjectData(strDomain.c_str(), strName.c_str(), eStyle);
			if (nullptr == m_pObjectValue)
				m_pObjectValue = m_pDataCenter->GetObjectDataValue(strDomain.c_str(), strName.c_str());
			m_pNamedData = m_pDataCenter->FindData(strDomain.c_str(), strName.c_str());
			break;
		default:
			break;
		}
	}

}

namespace why
{
	CallbackTest::CallbackTest(IDataCenter* pCenter)
	{
		//INITIALIZE_OBSERVERS(pCenter);

		SubscribeDatas(pCenter);
	}

	void CallbackTest::OnWhyChanged(INamedData* pData)
	{
		std::cout << "CallbackTest::OnWhyChanged";
	}


}

