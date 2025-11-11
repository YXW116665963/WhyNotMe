#include "data_loader.h"

#include "text_util.h"
#include "file_util.h"
#include "data_center_type.h"
#include "data_center.h"
#include "logger.h"
namespace why
{
	namespace
	{
		const std::map<std::string, ValueType> g_valueTypeMap =
		{
			{"int32_t", ValueType::eInt},
			{"uint32_t", ValueType::eUInt},
			{"int64_t", ValueType::eBigInt},
			{"uint64_t", ValueType::eBigUInt},
			{"float", ValueType::eFloat},
			{"string", ValueType::eString},
			{"object", ValueType::eObject},
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
	}
	DataLoader::DataLoader(IDataCenter* pDataCenter)
		: m_pDataCenter(pDataCenter)
	{

	}

	DataLoader::~DataLoader()
	{

	}
	bool DataLoader::Load(DataList& dataList, const std::unordered_set<std::string>& limitSet)
	{
		uint32_t									uLineCount{ 0 };
		std::string									strDomain;
		std::string									strName;
		why::ValueType								eType{ ValueType::eInt };

		uLineCount = (uint32_t)dataList.size();
		for (uint32_t i = 1; i < uLineCount; i++)
		{
			std::vector<std::string>& lineInfo = dataList[i];
			uint32_t					uItemSize = (uint32_t)lineInfo.size();

			if (uItemSize < 6)
			{
				LOG_WARNING << "line:" << i << " has " << uItemSize << " param, smaller 6";
				continue;
			}

			if (lineInfo[3].empty())
				continue;

			if (!GetValueType(lineInfo[3], eType))
			{
				LOG_WARNING << "line:" << i << " has bad vale type: " << lineInfo[3];
				continue;
			}
			if (!lineInfo[0].empty() && limitSet.size() > 0 && limitSet.count(lineInfo[0]) != 0) {
				LOG_ERROR << "line:" << i << " has li domain: " << lineInfo[0];
				continue;
			}

			if (ValueType::eString != eType)
			{
				if (lineInfo[5].empty())
					lineInfo[5] = "0";
			}

			if (m_pObjectValue)
			{
				if ((lineInfo[0].empty() || lineInfo[1].empty()) && (!lineInfo[2].empty()))
				{
					AddChildData(lineInfo[2], eType, (uint32_t)atoi(lineInfo[4].c_str()), lineInfo[5]);
				}
				else
				{
					if (!lineInfo[0].empty() && !lineInfo[1].empty())
					{
						RegisterData(
							lineInfo[0],
							lineInfo[1],
							eType,
							(int32_t)std::stoi(lineInfo[4]),
							lineInfo[5],
							why::ICaseCompare(lineInfo[6], "true")
						);

						if (m_pObjectValue && (!lineInfo[2].empty()))
							AddChildData(lineInfo[2], eType, (uint32_t)atoi(lineInfo[4].c_str()), lineInfo[5]);
					}
				}
			}
			else
			{
				if (lineInfo[0].empty() || lineInfo[1].empty())
				{
					LOG_WARNING << "line;" << i << " has not domain or name";
					continue;
				}

				RegisterData(
					lineInfo[0],
					lineInfo[1],
					eType,
					(int32_t)std::stoi(lineInfo[4]),
					lineInfo[5],
					why::ICaseCompare(lineInfo[6], "true")
				);

				if (m_pObjectValue && (!lineInfo[2].empty()))
					AddChildData(lineInfo[2], eType, (uint32_t)atoi(lineInfo[4].c_str()), lineInfo[5]);
			}
		}

		return true;

	}

	bool DataLoader::Load(const std::string& strFileName)
	{
		LOG_INFO << "DataLoader Load file:" << strFileName << "";
		std::vector<std::vector<std::string>>		dataList;
		if (!LoadCSVFile(strFileName, dataList))
		{
			LOG_ERROR << "data file:" << strFileName << " load is failed";
			return false;
		}
		return Load(dataList, why::ProhibitImportDomains());
	}

	void DataLoader::RegisterData(const std::string& strDomain, const std::string& strName, ValueType eType, uint32_t uSize, const std::string& strDefault, bool bPersistence)
	{
		DataStyle					eStyle = bPersistence ? DataStyle::ePersistence : DataStyle::eNormal;

		m_pObjectValue = nullptr;
		if (1 == uSize)
		{
			switch (eType)
			{
			case why::ValueType::eInt:
				if (!m_pDataCenter->RegisterIntData(strDomain.c_str(), strName.c_str(), (int32_t)std::stoi(strDefault), eStyle))
				{
					IDataValue* pValue = m_pDataCenter->GetDataValue(strDomain.c_str(), strName.c_str());
					if (pValue) {
						pValue->SetIntValue((int32_t)std::stoi(strDefault));
					}
				}
				break;
			case why::ValueType::eUInt:
				if (!m_pDataCenter->RegisterUIntData(strDomain.c_str(), strName.c_str(), (uint32_t)std::stoi(strDefault), eStyle))
				{
					IDataValue* pValue = m_pDataCenter->GetDataValue(strDomain.c_str(), strName.c_str());
					if (pValue) {
						pValue->SetUIntValue((uint32_t)std::stoi(strDefault));
					}
				}
				break;
			case why::ValueType::eBigInt:
				if (!m_pDataCenter->RegisterBigIntData(strDomain.c_str(), strName.c_str(), (int64_t)std::stoll(strDefault), eStyle))
				{
					IDataValue* pValue = m_pDataCenter->GetDataValue(strDomain.c_str(), strName.c_str());
					if (pValue) {
						pValue->SetBigIntValue((int64_t)std::stoll(strDefault));
					}
				}
				break;
			case why::ValueType::eBigUInt:
				if (!m_pDataCenter->RegisterBigUIntData(strDomain.c_str(), strName.c_str(), (uint64_t)std::stoul(strDefault), eStyle))
				{
					IDataValue* pValue = m_pDataCenter->GetDataValue(strDomain.c_str(), strName.c_str());
					if (pValue) {
						pValue->SetBigUIntValue((uint64_t)std::stoul(strDefault));
					}
				}
				break;
			case why::ValueType::eFloat:
				if (!m_pDataCenter->RegisterFloatData(strDomain.c_str(), strName.c_str(), std::stod(strDefault), eStyle))
				{
					IDataValue* pValue = m_pDataCenter->GetDataValue(strDomain.c_str(), strName.c_str());
					if (pValue) {
						pValue->SetFloatValue(std::stod(strDefault));
					}
				}
				break;
			case why::ValueType::eString:
				if (!m_pDataCenter->RegisterStringData(strDomain.c_str(), strName.c_str(), strDefault.c_str(), eStyle))
				{
					IDataValue* pValue = m_pDataCenter->GetDataValue(strDomain.c_str(), strName.c_str());
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
		else
		{
			IListDataValue* pListValue = m_pDataCenter->RegisterListData(strDomain.c_str(), strName.c_str(), eType,
				uSize, strDefault.c_str(), eStyle);

			if (nullptr == pListValue)
			{
				pListValue = m_pDataCenter->GetListDataValue(strDomain.c_str(), strName.c_str());
				pListValue->Resize(uSize);

				if (0 != uSize) {
					pListValue->SetValue(eType, strDefault, false);
				}
			}
		}
	}

	void DataLoader::AddChildData(const std::string& strName, ValueType eType, uint32_t uSize, const std::string& strDefault)
	{
		if (m_pObjectValue)
		{
			if (DataStyle::ePersistence == m_pNamedData->GetStyle())
			{
				if (nullptr != m_pObjectValue->Get(strName.c_str()))
					return;
			}

			if (uSize <= 1)
			{
				switch (eType)
				{
				case why::ValueType::eInt:
					m_pObjectValue->SetAsInt(strName.c_str(), (int32_t)std::stoi(strDefault));
					break;
				case why::ValueType::eUInt:
					m_pObjectValue->SetAsUInt(strName.c_str(), (uint32_t)std::stoi(strDefault));
					break;
				case why::ValueType::eBigInt:
					m_pObjectValue->SetAsBigInt(strName.c_str(), (int64_t)std::stoll(strDefault));
					break;
				case why::ValueType::eBigUInt:
					m_pObjectValue->SetAsBigUInt(strName.c_str(), (uint64_t)std::stoul(strDefault));
					break;
				case why::ValueType::eFloat:
					m_pObjectValue->SetAsFloat(strName.c_str(), std::stod(strDefault));
					break;
				case why::ValueType::eString:
					m_pObjectValue->SetAsString(strName.c_str(), strDefault.c_str());
					break;
				default:
					break;
				}
			}
			else
			{
				switch (eType)
				{
				case why::ValueType::eFloat:
					m_pObjectValue->SetListFloatValue(strName.c_str(), strDefault.c_str());
					break;
				default:
					break;
				}
			}
		}
	}

	bool DataLoader::Clone(const char* pFromDomain, const char* pToDomain)
	{
		LOG_INFO << "DataLoader Clone from " << pFromDomain << ",to:" << pToDomain;
		auto pDataCenter = dynamic_cast<DataCenter*>(m_pDataCenter);
		if (nullptr == pDataCenter)
			return false;
		why::DataList dataList;
		if (!pDataCenter->Export(pFromDomain, dataList))
			return false;

		std::unordered_set<std::string> limitSet;
		uint32_t									uLineCount{ 0 };

		uLineCount = (uint32_t)dataList.size();
		for (uint32_t i = 1; i < uLineCount; i++)
		{
			std::vector<std::string>& lineInfo = dataList[i];

			if (!lineInfo[0].empty()) {
				lineInfo[0] = pToDomain;
			}
		}
		return Load(dataList, limitSet);
	}

	bool DataLoader::Export(const char* pDomain, const char* pDataFile)
	{
		LOG_INFO << "DataLoader Export from" << pDomain << ",to:" << pDataFile;

		auto pDataCenter = dynamic_cast<DataCenter*>(m_pDataCenter);
		if (nullptr == pDataCenter)
			return false;

		DataList dataList;
		if (!pDataCenter->Export(pDomain, dataList))
			return false;

		return WriteCSVFile(dataList, pDataFile);
	}
}

bool RegisterDataFromFile(why::IDataCenter* pDataCenter, const char* pDataFile)
{
	why::DataLoader			dataLoader(pDataCenter);

	return dataLoader.Load(pDataFile);
}

bool CloneData(why::IDataCenter* pDataCenter, const char* pFromDomain, const char* pToDomain)
{
	why::DataLoader			dataLoader(pDataCenter);

	return dataLoader.Clone(pFromDomain, pToDomain);
}

bool ImportDataFromFile(why::IDataCenter* pDataCenter, const char* pDataFile, const char* pDomain, const char* pDB)
{
	LOG_INFO << "ImportDataFromFile file:" << pDataFile << ",to:" << pDomain;

	why::DataLoader			dataLoader(pDataCenter);

	std::unordered_set<std::string> limitSet;
	std::vector<std::vector<std::string>>		dataList;
	if (!why::LoadCSVFile(pDataFile, dataList))
	{
		LOG_ERROR << "data file:" << pDataFile << " load is failed";
		return false;
	}
	uint32_t									uLineCount{ 0 };

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
	return dataLoader.Load(dataList, limitSet);
}

bool ExportDataToFile(why::IDataCenter* pDataCenter, const char* pDomain, const char* pDataFile)
{
	LOG_INFO << "ExportDataToFile from:" << pDomain << ",to:" << pDataFile;

	why::DataLoader			dataLoader(pDataCenter);

	return dataLoader.Export(pDomain, pDataFile);
}