
#pragma once

#include "standard.h"

#include "data_value_interface.h"
#include <string>
#include "global_pointer.h"


//#include "running_log.h"

#include "logger.h"
#include "text_util.h"

namespace why
{
	inline void AssertNullPtr(IDataValue* pDataValue)
	{
#if defined(DEV_MODE) 
		assert(nullptr != pDataValue);
#endif
	}
	// string
	std::string GetStringValue(const char* pDomain, const char* pName);
	std::string GetListStringValueAt(const char* pDomain, const char* pName, uint32_t uIdnex);
	std::string GetObjectStringValue(const char* pDomain, const char* pName, const char* pChildName, std::string& strText);
	//
	bool SetStringValue(const char* pDomain, const char* pName, const char* val);
	bool SetObjectStringValue(const char* pDomain, const char* pName, const char* pChildName, const char* val);

	// int32_t
	int32_t GetIntValue(const char* pDomain, const char* pName);
	int32_t GetListIntValueAt(const char* pDomain, const char* pName, uint32_t uIdnex);
	int32_t GetObjectIntValue(const char* pDomain, const char* pName, const char* pChildName);
	//
	bool SetObjectIntValue(const char* pDomain, const char* pName, const char* pChildName, const int32_t val);
	bool SetIntValue(const char* pDomain, const char* pName, const int32_t val);

	//uint32_t
	uint32_t GetUIntValue(const char* pDomain, const char* pName);
	uint32_t GetListUIntValueAt(const char* pDomain, const char* pName, uint32_t uIdnex);
	uint32_t GetObjectUIntValue(const char* pDomain, const char* pName, const char* pChildName);
	//
	bool SetUIntValue(const char* pDomain, const char* pName, const uint32_t val);
	bool SetObjectUIntValue(const char* pDomain, const char* pName, const char* pChildName, const uint32_t val);

	//int64_t
	int64_t GetBigIntValue(const char* pDomain, const char* pName);
	int64_t GetListBigIntValueAt(const char* pDomain, const char* pName, uint32_t uIdnex);
	int64_t GetObjectBigIntValue(const char* pDomain, const char* pName, const char* pChildName);
	//
	bool SetBigIntValue(const char* pDomain, const char* pName, const int64_t val);
	bool SetObjectBigIntValue(const char* pDomain, const char* pName, const char* pChildName, const int64_t val);

	//uint64_t
	uint64_t GetBigUIntValue(const char* pDomain, const char* pName);
	uint64_t GetListBigUIntValueAt(const char* pDomain, const char* pName, uint32_t uIdnex);
	uint64_t GetObjectBigUIntValue(const char* pDomain, const char* pName, const char* pChildName);
	//
	bool SetBigUIntValue(const char* pDomain, const char* pName, const uint64_t val);
	bool SetObjectBigUIntValue(const char* pDomain, const char* pName, const char* pChildName, const uint64_t val);

	//float64_t
	float64_t GetFloatValue(const char* pDomain, const char* pName);
	float64_t GetListFloatValueAt(const char* pDomain, const char* pName, uint32_t uIdnex);
	float64_t GetObjectFloatValue(const char* pDomain, const char* pName, const char* pChildName);
	//
	bool SetFloatValue(const char* pDomain, const char* pName, const float64_t val);
	bool SetObjectFloatValue(const char* pDomain, const char* pName, const char* pChildName, const float64_t val);
}