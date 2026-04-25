#pragma once

#include "standard.h"

#include <string>
#include <any>
#include <map>

namespace why
{
	class EventData
	{
	public:
		EventData();
		~EventData();

	public:

		bool HasAttribute(const std::string& strKey) const;
		bool GetAttributeAsBool(const std::string& strKey) const;
		int32_t GetAttributeAsInt(const std::string& strKey) const;
		uint32_t GetAttributeAsUInt(const std::string& strKey) const;
		int64_t GetAttributeAsBigInt(const std::string& strKey) const;
		uint64_t GetAttributeAsUBigInt(const std::string& strKey) const;
		float64_t GetAttributeAsFloat(const std::string& strKey) const;
		std::string GetAttributeAsString(const std::string& strKey) const;
		void SetAttributeAsBool(const std::string& strKey, bool bValue);
		void SetAttributeAsInt(const std::string& strKey, int32_t nValue);
		void SetAttributeAsUInt(const std::string& strKey, uint32_t uValue);
		void SetAttributeAsBigInt(const std::string& strKey, int64_t nValue);
		void SetAttributeAsUBigInt(const std::string& strKey, uint64_t uValue);
		void SetAttributeAsFloat(const std::string& strKey, float32_t fValue);
		void SetAttributeAsString(const std::string& strKey, const std::string& strValue);

	private:
		typedef std::map<std::string, std::any> AttributeMap;

	private:
		std::string				m_strSerialID;
		AttributeMap			m_attributes;
	};
}
