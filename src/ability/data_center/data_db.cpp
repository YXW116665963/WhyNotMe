#include "data_db.h"
#include "value_util.h"
#include "text_util.h"

namespace
{
	const char* g_lpCreateTable = "create table DataList(Domain char(255) NOT NULL, Name char(255) NOT NULL, Type smallint, "
		"Size smallint, IntValue interger DEFAULT'0', FloatValue float64_t DEFAULT'0.0', StrValue char(255) DEFAULT' ', ArrayValue Blob)";

	const char* g_lpFindData = "select * from DataList where Domain = '%s' and Name = '%s'";
	const char* g_lpFindDataEx = "select * from DataList where Domain = '%s' and Name = '%s' and Type='%d'";
	const char* g_lpUpdatDataItem = "update DataList set Type=%d, Size=%d where Domain='%s' and Name='%s'";
	const char* g_lpInsertDataItem = "insert into DataList(Domain, Name, Type, Size) values('%s', '%s', %d, %d)";
	const char* g_lpUpdatIntData = "update DataList set IntValue=%s where Domain='%s' and Name='%s'";
	const char* g_lpUpdatFloatData = "update DataList set FloatValue=%f where Domain='%s' and Name='%s'";
	const char* g_lpUpdatTextData = "update DataList set StrValue='%s' where Domain='%s' and Name='%s'";
	const char* g_lpUpdatBlobData = "update DataList set ArrayValue=? where Domain='%s' and Name='%s'";
}

namespace why
{
	DataDB::DataDB()
	{
		m_db = nullptr;
	}

	DataDB::~DataDB()
	{
		if (m_db)
		{
			sqlite3_close(m_db);
			m_db = nullptr;
		}
	}

	bool DataDB::Startup(const std::string& strFileName)
	{
		uint32_t			uResult;

		// 关闭之前打开的数据库
		if (m_db)
		{
			sqlite3_close(m_db);
			m_db = nullptr;
		}

		// 创建或打开数据库
		if (SQLITE_OK != sqlite3_open(strFileName.c_str(), &m_db))
		{
			m_db = nullptr;
			return false;
		}

		char* pError;

		if (SQLITE_OK != (uResult = sqlite3_exec(m_db, g_lpCreateTable, nullptr, nullptr, &pError)))
		{
			if (0 == strcmp(pError, "table DataList already exists"))
			{
				free(pError);
			}
			else
			{
				free(pError);
				sqlite3_close(m_db);
				m_db = nullptr;
				return false;
			}
		}

		return true;
	}

	void DataDB::Shutdown()
	{
		if (m_db)
		{
			sqlite3_close(m_db);
			m_db = nullptr;
		}
	}

	bool DataDB::InsertData(INamedData* pData)
	{
		assert(m_db);
		assert(pData);

		if ((nullptr == m_db) || (nullptr == pData))
			return false;

		std::string			strDomain = pData->Domain();
		std::string			strName = pData->Name();
		int32_t				nType = GetNameDataType(pData);
		uint32_t			uSize = GetNameDataSize(pData);
		char				acSql[512];

		if (Find(strDomain, strName))
			return false;

		snprintf(acSql, sizeof(acSql), g_lpInsertDataItem, strDomain.c_str(), strName.c_str(), nType, (int)uSize);
		return (SQLITE_OK == sqlite3_exec(m_db, acSql, nullptr, nullptr, nullptr));
	}

	bool DataDB::ReadData(INamedData* pData)
	{
		assert(m_db);
		assert(pData);

		if ((nullptr == m_db) || (nullptr == pData))
			return false;

		std::string			strDomain = pData->Domain();
		std::string			strName = pData->Name();
		int32_t				nType = GetNameDataType(pData);
		char				acSql[512];
		bool				bReturn = false;
		sqlite3_stmt* pStmt;

		snprintf(acSql, sizeof(acSql), g_lpFindDataEx, strDomain.c_str(), strName.c_str(), nType);
		if (SQLITE_OK != sqlite3_prepare(m_db, acSql, -1, &pStmt, nullptr))
			return false;

		if (SQLITE_ROW == sqlite3_step(pStmt))
		{
			LoadData(pData, pStmt);
			bReturn = true;
		}

		sqlite3_finalize(pStmt);
		return bReturn;
	}

	void DataDB::WriteData(INamedData* pData)
	{
		assert(m_db);
		assert(pData);

		if ((nullptr == m_db) || (nullptr == pData))
			return;

		std::string			strDomain = pData->Domain();
		std::string			strName = pData->Name();
		auto				pValue = pData->GetValue();

		switch (pValue->Type())
		{
		case ValueType::eInt:
			UpdateData(strDomain, strName, (uint64_t)pValue->GetIntValue());
			break;
		case ValueType::eUInt:
			UpdateData(strDomain, strName, (uint64_t)pValue->GetUIntValue());
			break;
		case ValueType::eBigInt:
			UpdateData(strDomain, strName, (uint64_t)pValue->GetBigIntValue());
			break;
		case ValueType::eBigUInt:
			UpdateData(strDomain, strName, pValue->GetBigUIntValue());
			break;
		case ValueType::eFloat:
			UpdateData(strDomain, strName, pValue->GetFloatValue());
			break;
		case ValueType::eString:
		{
			std::string				strValue;

			if (GetStringValue(pValue, strValue))
				UpdateData(strDomain, strName, strValue);
		}
		break;
		case ValueType::eList:
		{
			std::string				strValue;

			if (GetListValue(dynamic_cast<const IListDataValue*>(pValue), strValue))
				UpdateData(strDomain, strName, (const int8_t*)strValue.c_str(), strValue.length());
		}
		break;
		case ValueType::eObject:
		{
			std::string				strValue;

			if (GetObjectValue(dynamic_cast<const IObjectDataValue*>(pValue), strValue))
				UpdateData(strDomain, strName, (const int8_t*)strValue.c_str(), strValue.length());
		}
		break;
		default:
			break;
		}
		return;
	}

	void DataDB::UpdateData(const std::string& strDomain, const std::string& strName, uint64_t uData)
	{
		std::string			strValue = std::to_string(uData);
		char				acSql[512];

		snprintf(acSql, sizeof(acSql), g_lpUpdatIntData, strValue.c_str(), strDomain.c_str(), strName.c_str());
		sqlite3_exec(m_db, acSql, nullptr, nullptr, nullptr);
		return;
	}

	void DataDB::UpdateData(const std::string& strDomain, const std::string& strName, float64_t fData)
	{
		char				acSql[512];

		snprintf(acSql, sizeof(acSql), g_lpUpdatFloatData, fData, strDomain.c_str(), strName.c_str());
		sqlite3_exec(m_db, acSql, nullptr, nullptr, nullptr);
		return;
	}

	void DataDB::UpdateData(const std::string& strDomain, const std::string& strName, const std::string& strText)
	{
		char				acSql[512];

		snprintf(acSql, sizeof(acSql), g_lpUpdatTextData, strText.c_str(), strDomain.c_str(), strName.c_str());
		sqlite3_exec(m_db, acSql, nullptr, nullptr, nullptr);
		return;
	}

	void DataDB::UpdateData(const std::string& strDomain, const std::string& strName, const int8_t* pData, uint32_t uLen)
	{
		char				acSql[512];
		sqlite3_stmt* pStmt;

		snprintf(acSql, sizeof(acSql), g_lpUpdatBlobData, strDomain.c_str(), strName.c_str());
		if (SQLITE_OK != sqlite3_prepare(m_db, acSql, -1, &pStmt, nullptr))
			return;

		if (SQLITE_OK == sqlite3_bind_blob(pStmt, 1, (const void*)pData, (int)uLen, nullptr))
			sqlite3_step(pStmt);

		sqlite3_finalize(pStmt);
		return;
	}

	bool DataDB::IsStartup()
	{
		return (nullptr != m_db);
	}

	void DataDB::LoadData(INamedData* pData, sqlite3_stmt* pStmt)
	{
		ValueType				eType;
		uint32_t				uSize;

		eType = (ValueType)sqlite3_column_int(pStmt, 2);
		uSize = (uint32_t)sqlite3_column_int(pStmt, 3);
		if (1 == uSize)
		{
			switch (eType)
			{
			case ValueType::eInt:
			case ValueType::eUInt:
			case ValueType::eBigInt:
			case ValueType::eBigUInt:
				LoadIntData(pData, eType, pStmt);
				break;
			case ValueType::eFloat:
				LoadFloatData(pData, pStmt);
				break;
			case ValueType::eString:
				LoadStringData(pData, pStmt);
				break;
			case ValueType::eObject:
				LoadObjectData(pData, pStmt);
				break;
			default:
				break;
			}
		}
		else
		{
			LoadArrayData(pData, eType, uSize, pStmt);
		}
	}

	void DataDB::LoadIntData(INamedData* pData, ValueType eType, sqlite3_stmt* pStmt)
	{
		sqlite_int64				nValue;
		auto						pDataValue = pData->GetValue();

		nValue = sqlite3_column_int64(pStmt, 4);
		switch (eType)
		{
		case ValueType::eInt:
			pDataValue->SetIntValue((int32_t)nValue);
			break;
		case ValueType::eUInt:
			pDataValue->SetUIntValue((uint32_t)nValue);
			break;
		case ValueType::eBigInt:
			pDataValue->SetBigIntValue((int64_t)nValue);
			break;
		case ValueType::eBigUInt:
			pDataValue->SetBigUIntValue((uint64_t)nValue);
			break;
		default:
			break;
		}
	}

	void DataDB::LoadFloatData(INamedData* pData, sqlite3_stmt* pStmt)
	{
		float64_t				fValue;
		auto					pDataValue = pData->GetValue();

		fValue = sqlite3_column_double(pStmt, 5);
		pDataValue->SetFloatValue(fValue);
	}

	void DataDB::LoadStringData(INamedData* pData, sqlite3_stmt* pStmt)
	{
		std::string				strValue;
		auto					pDataValue = pData->GetValue();

		strValue = (const char*)sqlite3_column_text(pStmt, 6);
		pDataValue->SetStringValue(strValue.c_str());
	}

	void DataDB::LoadObjectData(INamedData* pData, sqlite3_stmt* pStmt)
	{
		auto					pDataValue = pData->GetValue();
		auto					pObjectValue = dynamic_cast<IObjectDataValue*>(pDataValue);

		if (pObjectValue)
		{
			uint32_t			uCount;

			uCount = (uint32_t)sqlite3_column_bytes(pStmt, 7);
			if (uCount > 0)
			{
				const char* pBlob = (const char*)sqlite3_column_blob(pStmt, 7);
				std::string			strContent(pBlob, uCount);

				
				SetObjectValue(pObjectValue, strContent);
			}
		}
	}

	void DataDB::LoadArrayData(INamedData* pData, ValueType eType, uint32_t uSize, sqlite3_stmt* pStmt)
	{
		auto					pDataValue = pData->GetValue();
		auto					pListValue = dynamic_cast<IListDataValue*>(pDataValue);

		if (pListValue)
		{
			uint32_t			uCount;

			uCount = (uint32_t)sqlite3_column_bytes(pStmt, 7);
			if (uCount > 0)
			{
				const char* pBlob = (const char*)sqlite3_column_blob(pStmt, 7);
				std::string			strContent(pBlob, uCount);

				SetListValue(pListValue, eType, strContent);
			}
		}
	}

	bool DataDB::Find(const std::string& strDomain, const std::string& strName)
	{
		assert(m_db);

		sqlite3_stmt* pStmt;
		char				acSql[512];
		bool				bResult;

		snprintf(acSql, sizeof(acSql), g_lpFindData, strDomain.c_str(), strName.c_str());
		if (SQLITE_OK != sqlite3_prepare(m_db, acSql, -1, &pStmt, nullptr))
			return false;

		bResult = false;
		if (SQLITE_ROW == sqlite3_step(pStmt))
			bResult = true;

		sqlite3_finalize(pStmt);
		return bResult;
	}

	int32_t DataDB::GetNameDataType(INamedData* pData)
	{
		auto				pDataValue = pData->GetValue();
		auto				valueType = pDataValue->Type();

		if (ValueType::eList == valueType)
		{
			auto			pListValue = dynamic_cast<IListDataValue*>(pData);

			if (pListValue)
				valueType = pListValue->ChildType();
		}

		return static_cast<int32_t>(valueType);
	}

	uint32_t DataDB::GetNameDataSize(INamedData* pData)
	{
		auto				pDataValue = pData->GetValue();
		auto				valueType = pDataValue->Type();
		uint32_t			uCount = 1;

		if (ValueType::eList == valueType)
		{
			auto			pListValue = dynamic_cast<IListDataValue*>(pData);

			if (pListValue)
				uCount = pListValue->Size();
		}

		return uCount;
	}
}