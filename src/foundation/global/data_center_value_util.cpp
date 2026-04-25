#include "data_center_value_util.h"

namespace why
{
	bool SetStringValue(const char* pDomain, const char* pName, const char* val)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pDataValue = g_globalPointer.m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->SetStringValue(val);
		else
			LOG_ERROR << "SetStringValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return false;
	}

	std::string GetStringValue(const char* pDomain, const char* pName)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pDataValue = g_globalPointer.m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
		{
			int32_t		nLen = pDataValue->GetStringValue(nullptr, 0);
			std::string strText;
			strText.resize(nLen, 0);
			pDataValue->GetStringValue(strText.data(), nLen);
			return strText;
		}
		else
		{
			LOG_ERROR << "GetStringValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		}
		// AssertNullPtr(pDataValue);
		return "";
	}


	int32_t GetIntValue(const char* pDomain, const char* pName)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pDataValue = g_globalPointer.m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->GetIntValue();
		else
			LOG_ERROR << "GetIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return 0;
	}

	bool SetIntValue(const char* pDomain, const char* pName, const int32_t val)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pDataValue = g_globalPointer.m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->SetIntValue(val);
		else
			LOG_ERROR << "SetIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return false;
	}

	uint32_t GetUIntValue(const char* pDomain, const char* pName)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pDataValue = g_globalPointer.m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->GetUIntValue();
		else
			LOG_ERROR << "GetUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return 0;
	}

	bool SetUIntValue(const char* pDomain, const char* pName, const uint32_t val)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pDataValue = g_globalPointer.m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->SetUIntValue(val);
		else
			LOG_ERROR << "SetUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return false;
	}


	int32_t GetObjectIntValue(const char* pDomain, const char* pName, const char* pChildName)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pObjectDataValue = g_globalPointer.m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->GetAsInt(pChildName);
		else
			LOG_ERROR << "GetObjectIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return 0;
	}


	bool SetObjectIntValue(const char* pDomain, const char* pName, const char* pChildName, const int32_t val)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pObjectDataValue = g_globalPointer.m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->SetAsInt(pChildName, val);
		else
			LOG_ERROR << "SetObjectIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return false;
	}

	int32_t GetListIntValueAt(const char* pDomain, const char* pName, uint32_t uIdnex)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pListDataValue = g_globalPointer.m_dataCenterPtr->GetListDataValue(pDomain, pName);
		if (nullptr != pListDataValue)
			return pListDataValue->GetAtAsInt(uIdnex);
		else
			LOG_ERROR << "GetListIntValueAt pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pListDataValue);
		return 0;
	}



	uint32_t GetObjectUIntValue(const char* pDomain, const char* pName, const char* pChildName)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pObjectDataValue = g_globalPointer.m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->GetAsUInt(pChildName);
		else
			LOG_ERROR << "GetObjectUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return 0;
	}

	bool SetObjectUIntValue(const char* pDomain, const char* pName, const char* pChildName, const uint32_t val)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pObjectDataValue = g_globalPointer.m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->SetAsUInt(pChildName, val);
		else
			LOG_ERROR << "SetObjectUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return false;
	}

	uint32_t GetListUIntValueAt(const char* pDomain, const char* pName, uint32_t uIdnex)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pListDataValue = g_globalPointer.m_dataCenterPtr->GetListDataValue(pDomain, pName);
		if (nullptr != pListDataValue)
			return pListDataValue->GetAtAsUInt(uIdnex);
		else
			LOG_ERROR << "GetListUIntValueAt pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pListDataValue);
		return 0;
	}

	int64_t GetBigIntValue(const char* pDomain, const char* pName)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pDataValue = g_globalPointer.m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->GetBigIntValue();
		else
			LOG_ERROR << "GetBigIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return 0;
	}

	bool SetBigIntValue(const char* pDomain, const char* pName, const int64_t val)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pDataValue = g_globalPointer.m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->SetBigIntValue(val);
		else
			LOG_ERROR << "SetBigIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return false;
	}

	int64_t GetObjectBigIntValue(const char* pDomain, const char* pName, const char* pChildName)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pObjectDataValue = g_globalPointer.m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->GetAsBigInt(pChildName);
		else
			LOG_ERROR << "GetObjectBigIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return 0;
	}

	bool SetObjectBigIntValue(const char* pDomain, const char* pName, const char* pChildName, const int64_t val)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pObjectDataValue = g_globalPointer.m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->SetAsBigInt(pChildName, val);
		else
			LOG_ERROR << "SetObjectBigIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return false;
	}

	int64_t GetListBigIntValueAt(const char* pDomain, const char* pName, uint32_t uIdnex)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pListDataValue = g_globalPointer.m_dataCenterPtr->GetListDataValue(pDomain, pName);
		if (nullptr != pListDataValue)
			return pListDataValue->GetAtAsBigInt(uIdnex);
		else
			LOG_ERROR << "GetListBigIntValueAt pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pListDataValue);
		return 0;
	}

	uint64_t GetBigUIntValue(const char* pDomain, const char* pName)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pDataValue = g_globalPointer.m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->GetBigUIntValue();
		else
			LOG_ERROR << "GetBigUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return 0;
	}

	bool SetBigUIntValue(const char* pDomain, const char* pName, const uint64_t val)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pDataValue = g_globalPointer.m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->SetBigUIntValue(val);
		else
			LOG_ERROR << "SetBigUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return false;
	}

	uint64_t GetObjectBigUIntValue(const char* pDomain, const char* pName, const char* pChildName)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pObjectDataValue = g_globalPointer.m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->GetAsBigUInt(pChildName);
		else
			LOG_ERROR << "GetObjectBigUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return 0;
	}

	uint64_t GetObjectBigUIntValue(INamedData* pNameData, const char* pChildName)
	{
		assert(nullptr != pNameData);
		auto pObjectDataValue = dynamic_cast<IObjectDataValue*>(pNameData->GetValue());
		assert(nullptr != pObjectDataValue);
		if ((nullptr != pObjectDataValue) && (ValueType::eObject == pObjectDataValue->Type()))
			return pObjectDataValue->GetAsBigUInt(pChildName);
		return 0;
	}

	bool SetObjectBigUIntValue(const char* pDomain, const char* pName, const char* pChildName, const uint64_t val)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pObjectDataValue = g_globalPointer.m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->SetAsBigUInt(pChildName, val);
		else
			LOG_ERROR << "SetObjectBigUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return false;
	}

	uint64_t GetListBigUIntValueAt(const char* pDomain, const char* pName, uint32_t uIdnex)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pListDataValue = g_globalPointer.m_dataCenterPtr->GetListDataValue(pDomain, pName);
		if (nullptr != pListDataValue)
			return pListDataValue->GetAtAsBigUInt(uIdnex);
		else
			LOG_ERROR << "GetListBigUIntValueAt pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pListDataValue);
		return 0;
	}

	float64_t GetFloatValue(const char* pDomain, const char* pName)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pDataValue = g_globalPointer.m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->GetFloatValue();
		else
			LOG_ERROR << "GetFloatValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return 0;
	}

	bool SetFloatValue(const char* pDomain, const char* pName, const float64_t val)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pDataValue = g_globalPointer.m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->SetFloatValue(val);
		else
			LOG_ERROR << "SetFloatValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return false;
	}

	float64_t GetObjectFloatValue(const char* pDomain, const char* pName, const char* pChildName)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pObjectDataValue = g_globalPointer.m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->GetAsFloat(pChildName);
		else
			LOG_ERROR << "GetObjectFloatValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return 0;
	}

	bool SetObjectFloatValue(const char* pDomain, const char* pName, const char* pChildName, const float64_t val)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pObjectDataValue = g_globalPointer.m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->SetAsFloat(pChildName, val);
		else
			LOG_ERROR << "SetObjectFloatValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return false;
	}

	float64_t GetListFloatValueAt(const char* pDomain, const char* pName, uint32_t uIdnex)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pListDataValue = g_globalPointer.m_dataCenterPtr->GetListDataValue(pDomain, pName);
		if (nullptr != pListDataValue)
			return pListDataValue->GetAtAsFloat(uIdnex);
		else
			LOG_ERROR << "GetListFloatValueAt pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pListDataValue);
		return 0;
	}



	std::string GetObjectStringValue(const char* pDomain, const char* pName, const char* pChildName)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pObjectDataValue = g_globalPointer.m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
		{
			int32_t		nLen = pObjectDataValue->GetAsString(pChildName, nullptr, 0);
			std::string strText;
			strText.resize(nLen, 0);
			pObjectDataValue->GetAsString(pChildName, strText.data(), nLen);
			return strText;
		}
		else
		{
			LOG_ERROR << "GetObjectStringValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		}
		AssertNullPtr(pObjectDataValue);
		return "";
	}

	bool SetObjectStringValue(const char* pDomain, const char* pName, const char* pChildName, const char* val)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pObjectDataValue = g_globalPointer.m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
		{
			pObjectDataValue->SetAsString(pChildName, val);
			return true;
		}
		else
		{
			LOG_ERROR << "SetObjectStringValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		}
		AssertNullPtr(pObjectDataValue);
		return false;
	}



	std::string GetListStringValueAt(const char* pDomain, const char* pName, uint32_t uIdnex)
	{
		assert(nullptr != g_globalPointer.m_dataCenterPtr.get());
		auto pListDataValue = g_globalPointer.m_dataCenterPtr->GetListDataValue(pDomain, pName);
		if (nullptr != pListDataValue)
		{
			int32_t		nLen = pListDataValue->GetAtAsString(uIdnex, nullptr, 0);
			std::string strText;
			strText.resize(nLen, 0);
			return strText;
		}
		else
			LOG_ERROR << "GetListStringValueAt pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pListDataValue);
		return "";
	}
}