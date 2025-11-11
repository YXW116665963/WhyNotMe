#pragma once

#include "standard.h"
#include <functional>
#include <map>
#include <set>
#include <unordered_set>

#include "data_center_def.h"
#include "data_center_interface.h"

namespace why
{
	class DataLoader
	{
	public:
		DataLoader(IDataCenter* pDataCenter);
		~DataLoader();

	public:
		typedef std::vector<std::vector<std::string>> DataList;
		bool Load(DataList& dataList, const std::unordered_set<std::string>& limitSet);
		bool Load(const std::string& strFileName);

		bool Clone(const char* pFromDomain, const char* pToDomain);
		bool Export(const char* pDomain, const char* pDataFile);

	private:
		//<! 此处是导入文件时的注册。由于允许导入不同文件，第二次改变值即可
		void RegisterData(const std::string& strDomain, const std::string& strName, ValueType eType, uint32_t uSize, const std::string& strDefault, bool bPersistence);
		void AddChildData(const std::string& strName, ValueType eType, uint32_t uSize, const std::string& strDefault);

	private:
		IDataCenter* m_pDataCenter{ nullptr };
		INamedData* m_pNamedData{ nullptr };
		IObjectDataValue* m_pObjectValue{ nullptr };
	};
}
