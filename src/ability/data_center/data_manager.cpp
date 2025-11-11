#include "data_manager.h"
#include "data_center_interface.h"
#include "file_util.h"
#include "string_value.h"
#include "value_util.h"

namespace why
{
	DataManager::DataManager(ObserverManager* pObserverMgr, DataDB* pDataDB)
		: m_pObserverMgr(pObserverMgr)
		, m_pDataDB(pDataDB)
	{

	}

	DataManager::~DataManager()
	{

	}

	bool DataManager::RegisterIntData(const std::string& strDomain, const std::string& strName, int32_t nDefault, DataStyle eStyle)
	{
		NamedDataPtr			nameDataPtr(new NamedData(m_pObserverMgr, strDomain, strName, eStyle));

		if (!nameDataPtr->CreateIntValue(nDefault))
			return false;

		return Register(strDomain, strName, std::move(nameDataPtr));
	}

	bool DataManager::RegisterUIntData(const std::string& strDomain, const std::string& strName, uint32_t uDefault, DataStyle eStyle)
	{
		NamedDataPtr			nameDataPtr(new NamedData(m_pObserverMgr, strDomain, strName, eStyle));

		if (!nameDataPtr->CreateUIntValue(uDefault))
			return false;

		return Register(strDomain, strName, std::move(nameDataPtr));
	}

	bool DataManager::RegisterBigIntData(const std::string& strDomain, const std::string& strName, int64_t nDefault, DataStyle eStyle)
	{
		NamedDataPtr			nameDataPtr(new NamedData(m_pObserverMgr, strDomain, strName, eStyle));

		if (!nameDataPtr->CreateBigIntValue(nDefault))
			return false;

		return Register(strDomain, strName, std::move(nameDataPtr));
	}

	bool DataManager::RegisterBigUIntData(const std::string& strDomain, const std::string& strName, uint64_t uDefault, DataStyle eStyle)
	{
		NamedDataPtr			nameDataPtr(new NamedData(m_pObserverMgr, strDomain, strName, eStyle));

		if (!nameDataPtr->CreateBigUIntValue(uDefault))
			return false;

		return Register(strDomain, strName, std::move(nameDataPtr));
	}

	bool DataManager::RegisterFloatData(const std::string& strDomain, const std::string& strName, float64_t fDefault, DataStyle eStyle)
	{
		NamedDataPtr			nameDataPtr(new NamedData(m_pObserverMgr, strDomain, strName, eStyle));

		if (!nameDataPtr->CreateFloatValue(fDefault))
			return false;

		return Register(strDomain, strName, std::move(nameDataPtr));
	}

	bool DataManager::RegisterStringData(const std::string& strDomain, const std::string& strName, const std::string& strDefault, DataStyle eStyle)
	{
		NamedDataPtr			nameDataPtr(new NamedData(m_pObserverMgr, strDomain, strName, eStyle));

		if (!nameDataPtr->CreateStringValue(strDefault))
			return false;

		return Register(strDomain, strName, std::move(nameDataPtr));
	}

	IListDataValue* DataManager::RegisterListData(const std::string& strDomain, const std::string& strName, ValueType eType, uint32_t uSize, const std::string& strDefault, DataStyle eStyle)
	{
		NamedDataPtr			nameDataPtr(new NamedData(m_pObserverMgr, strDomain, strName, eStyle));
		IListDataValue* pListValue = nameDataPtr->CreateListValue(eType, uSize, strDefault);

		if (nullptr != pListValue)
		{
			if (Register(strDomain, strName, std::move(nameDataPtr)))
				return pListValue;
		}

		return nullptr;
	}

	IObjectDataValue* DataManager::RegisterObjectData(const std::string& strDomain, const std::string& strName, DataStyle eStyle)
	{
		NamedDataPtr			nameDataPtr(new NamedData(m_pObserverMgr, strDomain, strName, eStyle));
		IObjectDataValue* pObjectValue = nameDataPtr->CreateObjectValue();

		if (nullptr != pObjectValue)
		{
			if (Register(strDomain, strName, std::move(nameDataPtr)))
				return pObjectValue;
		}

		return nullptr;
	}

	INamedData* DataManager::FindData(const std::string& strDomain, const std::string& strName)
	{
		std::lock_guard<std::mutex>			lockGuard(m_lock);
		INamedData* pNamedData = nullptr;
		auto								itFindDomain = m_datas.find(strDomain);

		if (m_datas.end() != itFindDomain)
		{
			auto							itFindName = itFindDomain->second.find(strName);

			if (itFindDomain->second.end() != itFindName)
				pNamedData = itFindName->second.get();
		}

		return pNamedData;
	}

	void DataManager::UnregisterData(const std::string& strDomain, const std::string& strName)
	{
		std::lock_guard<std::mutex>			lockGuard(m_lock);
		auto								itFindDomain = m_datas.find(strDomain);

		if (m_datas.end() != itFindDomain)
			itFindDomain->second.erase(strName);
	}

	bool DataManager::HasDomain(const char* pDomain)
	{
		auto	itFindDomain = m_datas.find(pDomain);
		return m_datas.end() != itFindDomain;
	}

	bool DataManager::Register(const std::string& strDomain, const std::string& strName, NamedDataPtr&& namedDataPtr)
	{
		NamedData* pChangedData = nullptr;
		{
			std::lock_guard<std::mutex>			lockGuard(m_lock);
			auto								itFindDomain = m_datas.find(strDomain);

			if (m_datas.end() == itFindDomain)
			{
				m_datas.insert(std::make_pair(strDomain, NamedDataList()));
				itFindDomain = m_datas.find(strDomain);
			}

			auto itFindData = itFindDomain->second.find(strName);
			if (itFindDomain->second.end() == itFindData)
			{
				bool				bLoad = (DataStyle::ePersistence == namedDataPtr->GetStyle());

				if (bLoad && (nullptr != m_pDataDB) && m_pDataDB->IsStartup())
				{
					if (m_pDataDB->InsertData(namedDataPtr.get()))
						m_pDataDB->WriteData(namedDataPtr.get());
					else
						m_pDataDB->ReadData(namedDataPtr.get());
				}
				pChangedData = namedDataPtr.get();
				itFindDomain->second.insert(std::make_pair(strName, std::move(namedDataPtr)));
			}
			else
			{
				//ÖØ¸´×¢²á crash
				assert(itFindDomain->second.end() != itFindData);
				return false;
			}
		}
		if (nullptr != pChangedData)
			pChangedData->SetReady();
		return true;
	}

	bool DataManager::Export(const char* pDomain, why::DataList& dataList)
	{
		if ((nullptr == pDomain) || (0 == strlen(pDomain)))
			return false;

		auto								itFindDomain = m_datas.find(pDomain);

		if (m_datas.end() == itFindDomain)
		{
			return false;
		}
		NamedDataList& namedDataList = itFindDomain->second;

		std::vector<std::string>	lineInfo;

		lineInfo.push_back("domain");
		lineInfo.push_back("name");
		lineInfo.push_back("subname");
		lineInfo.push_back("type");
		lineInfo.push_back("count");
		lineInfo.push_back("value");
		lineInfo.push_back("db");
		lineInfo.push_back("des");
		dataList.push_back(lineInfo);

		std::string strName;
		std::string strDomain = pDomain;
		uint32_t uSize{ 1 };
		std::string strValue;
		std::string strType;
		bool bPersistence;
		IDataValue* pValue;
		for (DataIt it = namedDataList.begin(); it != namedDataList.end(); ++it)
		{
			strType.clear();
			uSize = 1;
			strValue.clear();
			NamedDataPtr& namedDataPtr = it->second;
			strName = namedDataPtr->Name();
			pValue = namedDataPtr->GetValue();
			bPersistence = namedDataPtr->GetStyle() == DataStyle::ePersistence;
			if (nullptr != pValue)
			{
				GetTypeValue(pValue, strType, strValue, uSize);

				lineInfo.clear();
				lineInfo.push_back(strDomain);
				lineInfo.push_back(strName);
				lineInfo.push_back("");
				lineInfo.push_back(strType);
				lineInfo.push_back(std::to_string(uSize));
				lineInfo.push_back(strValue);
				lineInfo.push_back(bPersistence ? "TRUE" : "FALSE");
				lineInfo.push_back("");
				dataList.push_back(lineInfo);


				if (why::ValueType::eObject == pValue->Type())
				{
					AddChildData(dynamic_cast<IObjectDataValue*>(pValue), strDomain, strName, lineInfo, dataList);
				}
			}
		}
		return true;
	}

	void DataManager::GetTypeValue(IDataValue* pValue, std::string& strType, std::string& strValue, uint32_t& uSize)
	{
		uSize = 1;
		switch (pValue->Type())
		{
		case why::ValueType::eInt:
			strType = "int32_t";
			strValue = std::to_string(pValue->GetIntValue());
			break;
		case why::ValueType::eUInt:
			strType = "uint32_t";
			strValue = std::to_string(pValue->GetUIntValue());
			break;
		case why::ValueType::eBigInt:
			strType = "int64_t";
			strValue = std::to_string(pValue->GetBigIntValue());
			break;
		case why::ValueType::eBigUInt:
			strType = "uint64_t";
			strValue = std::to_string(pValue->GetBigUIntValue());
			break;
		case why::ValueType::eFloat:
			strType = "float";
			strValue = std::to_string(pValue->GetFloatValue());
			break;
		case why::ValueType::eString:
			strType = "string";
			GetStringValue(pValue, strValue);
			break;
		case why::ValueType::eObject:
			strType = "object";
			strValue = "";
			break;
		case why::ValueType::eList:
			strType = "list";
			uSize = dynamic_cast<IListDataValue*>(pValue)->Size();
			GetListValue(dynamic_cast<IListDataValue*>(pValue), strValue);
			break;
		default:
			break;
		}
	}

	void DataManager::AddChildData(IObjectDataValue* pObjectValue, const std::string& strDomain, const std::string& strName, std::vector<std::string>& lineInfo, why::DataList& dataList)
	{

		std::string strSubName;
		uint32_t uSize{ 1 };
		std::string strValue;
		std::string strType;
		IDataValue* pObjectItemValue;

		uint32_t objectItemCount = pObjectValue->Size();
		for (uint32_t i = 0; i < objectItemCount; i++)
		{
			pObjectItemValue = pObjectValue->GetAt(i);
			if (nullptr == pObjectItemValue)
				continue;

			strSubName = pObjectValue->GetName(i);
			GetTypeValue(pObjectItemValue, strType, strValue, uSize);

			lineInfo.clear();
			lineInfo.push_back(strDomain);
			lineInfo.push_back(strName);
			lineInfo.push_back(strSubName);
			lineInfo.push_back(strType);
			lineInfo.push_back(std::to_string(uSize));
			lineInfo.push_back(strValue);
			lineInfo.push_back("");
			lineInfo.push_back("");
			dataList.push_back(lineInfo);
		}
	}

	bool DataManager::Clone(const char* pFromDomain, const char* pToDomain)
	{
		if ((nullptr == pFromDomain) || (0 == strlen(pFromDomain)))
			return false;

		if ((nullptr == pToDomain) || (0 == strlen(pToDomain)))
			return false;

		auto					itFromDomain = m_datas.find(pFromDomain);
		auto					itToDomain = m_datas.find(pToDomain);

		if (m_datas.end() == itFromDomain)
			return false;

		if (m_datas.end() == itToDomain)
		{
			m_datas.insert(std::make_pair(pToDomain, NamedDataList()));
			itToDomain = m_datas.find(pToDomain);
		}

		return Clone(itFromDomain->second, pToDomain, itToDomain->second);
	}

	bool DataManager::Clone(NamedDataList& fromDatas, const std::string& strDomain, NamedDataList& toDatas)
	{
		DataIt				itFind;
		bool				bAssign = false;
		bool                one_changed = false;

		for (auto itCur = fromDatas.begin(); itCur != fromDatas.end(); itCur++)
		{
			bAssign = false;
			itFind = toDatas.find(itCur->first);
			if (itFind != toDatas.end())
			{
				if (itFind->second->Assign(itCur->second.get(), one_changed))
					bAssign = true;
				else
					toDatas.erase(itFind);
			}

			if (!bAssign)
			{
				DataStyle				eDataStyle = itCur->second->GetStyle();
				NamedDataPtr			nameDataPtr(new NamedData(m_pObserverMgr, strDomain, itCur->first, eDataStyle));

				if (nameDataPtr->CreateValue(itCur->second.get()))
					Register(strDomain, itCur->first, std::move(nameDataPtr));
			}
		}

		return true;
	}
}