#pragma once

#include "standard.h"
#include "observer_manager.h"
#include "named_data.h"
#include "data_db.h"
#include "data_center_type.h"
#include <memory>
#include <map>
#include <string>
#include <mutex>

namespace why
{
	class DataManager
	{
	public:
		DataManager(ObserverManager* pObserverMgr, DataDB* pDataDB);
		~DataManager();

	public:
		bool RegisterIntData(const std::string& strDomain, const std::string& strName, int32_t nDefault, DataStyle eStyle);
		bool RegisterUIntData(const std::string& strDomain, const std::string& strName, uint32_t uDefault, DataStyle eStyle);
		bool RegisterBigIntData(const std::string& strDomain, const std::string& strName, int64_t nDefault, DataStyle eStyle);
		bool RegisterBigUIntData(const std::string& strDomain, const std::string& strName, uint64_t uDefault, DataStyle eStyle);
		bool RegisterFloatData(const std::string& strDomain, const std::string& strName, float64_t fDefault, DataStyle eStyle);
		bool RegisterStringData(const std::string& strDomain, const std::string& strName, const std::string& strDefault, DataStyle eStyle);
		IListDataValue* RegisterListData(const std::string& strDomain, const std::string& strName, ValueType eType, uint32_t uSize, const std::string& strDefault, DataStyle eStyle);
		IObjectDataValue* RegisterObjectData(const std::string& strDomain, const std::string& strName, DataStyle eStyle);
		INamedData* FindData(const std::string& strDomain, const std::string& strName);
		void UnregisterData(const std::string& strDomain, const std::string& strName);
		bool Export(const char* pDomain, why::DataList& dataList);
		bool HasDomain(const char* pDomain);
		bool Clone(const char* pFromDomain, const char* pToDomain);

	private:
		typedef std::unique_ptr<NamedData> NamedDataPtr;
		typedef std::map<std::string, NamedDataPtr> NamedDataList;
		typedef NamedDataList::iterator DataIt;
		typedef std::map<std::string, NamedDataList> NamedDataPool;
		typedef NamedDataPool::iterator DomainIt;

	private:
		bool Register(const std::string& strDomain, const std::string& strName, NamedDataPtr&& namedDataPtr);
		void GetTypeValue(IDataValue* pValue, std::string& strType, std::string& strValue, uint32_t& uSize);
		void AddChildData(IObjectDataValue* pObjectValue, const std::string& strDomain, const std::string& strName, std::vector<std::string>& lineInfo, why::DataList& dataList);
		bool Clone(NamedDataList& fromDatas, const std::string& strDomain, NamedDataList& toDatas);

	private:
		ObserverManager* m_pObserverMgr;
		DataDB* m_pDataDB;
		std::mutex				m_lock;
		NamedDataPool			m_datas;
	};
}