#include "event_data.h"
#include "logger.h"

namespace why
{
	namespace
	{
		uint32_t					m_uEventDataNextID{ 1 };

		template <class T>
		T GetEventDataAttributeValue(const std::string& strID, const std::map<std::string, std::any>& attributes, const std::string& strKey, T defaultValue)
		{
			auto		itFind = attributes.find(strKey);
			T			value = defaultValue;

			if (attributes.end() != itFind)
			{
				try
				{
					value = std::any_cast<T>(itFind->second);
				}
				catch (const std::bad_any_cast& e)
				{
					LOG_ERROR << "EventData[" << strID << "] get atrribute:" << strKey << " value type is bad:" << e.what();
				}
			}

			return value;
		}

		template <class T>
		void SetEventDataAttributeValue(const std::string& strID, std::map<std::string, std::any>& attributes, const std::string& strKey, T newValue)
		{
			auto			itFind = attributes.find(strKey);
			std::any		anyValue = newValue;

			if (attributes.end() != itFind)
			{
				if (anyValue.type() != itFind->second.type())
					LOG_ERROR << "EventData[" << strID << "] set atrribute :" << strKey << " value type is bad";
				else
					itFind->second = anyValue;
			}
			else
			{
				attributes.insert(std::make_pair(strKey, anyValue));
			}

			LOG_INFO << "EventData[" << strID << "] set atrribute :" << strKey;// << ",v: " << anyValue;
		}
	}

	EventData::EventData()
	{
		m_uEventDataNextID++;
		m_strSerialID = std::to_string(m_uEventDataNextID);
	}

	EventData::~EventData()
	{

	}
	 

	bool EventData::HasAttribute(const std::string& strKey) const
	{
		auto		itFind = m_attributes.find(strKey);

		return (itFind != m_attributes.end());
	}

	bool EventData::GetAttributeAsBool(const std::string& strKey) const
	{
		return GetEventDataAttributeValue(m_strSerialID, m_attributes, strKey, false);
	}

	int32_t EventData::GetAttributeAsInt(const std::string& strKey) const
	{
		return GetEventDataAttributeValue(m_strSerialID, m_attributes, strKey, (int32_t)0);
	}

	uint32_t EventData::GetAttributeAsUInt(const std::string& strKey) const
	{
		return GetEventDataAttributeValue(m_strSerialID, m_attributes, strKey, (uint32_t)0);
	}

	int64_t EventData::GetAttributeAsBigInt(const std::string& strKey) const
	{
		return GetEventDataAttributeValue(m_strSerialID, m_attributes, strKey, (int64_t)0);
	}

	uint64_t EventData::GetAttributeAsUBigInt(const std::string& strKey) const
	{
		return GetEventDataAttributeValue(m_strSerialID, m_attributes, strKey, (uint64_t)0);
	}

	float64_t EventData::GetAttributeAsFloat(const std::string& strKey) const
	{
		return GetEventDataAttributeValue(m_strSerialID, m_attributes, strKey, (float64_t)0.0);
	}

	std::string EventData::GetAttributeAsString(const std::string& strKey) const
	{
		return GetEventDataAttributeValue(m_strSerialID, m_attributes, strKey, std::string(""));
	}

	void EventData::SetAttributeAsBool(const std::string& strKey, bool bValue)
	{
		SetEventDataAttributeValue(m_strSerialID, m_attributes, strKey, bValue);
	}

	void EventData::SetAttributeAsInt(const std::string& strKey, int32_t nValue)
	{
		SetEventDataAttributeValue(m_strSerialID, m_attributes, strKey, nValue);
	}

	void EventData::SetAttributeAsUInt(const std::string& strKey, uint32_t uValue)
	{
		SetEventDataAttributeValue(m_strSerialID, m_attributes, strKey, uValue);
	}

	void EventData::SetAttributeAsBigInt(const std::string& strKey, int64_t nValue)
	{
		SetEventDataAttributeValue(m_strSerialID, m_attributes, strKey, nValue);
	}

	void EventData::SetAttributeAsUBigInt(const std::string& strKey, uint64_t uValue)
	{
		SetEventDataAttributeValue(m_strSerialID, m_attributes, strKey, uValue);
	}

	void EventData::SetAttributeAsFloat(const std::string& strKey, float32_t fValue)
	{
		SetEventDataAttributeValue(m_strSerialID, m_attributes, strKey, fValue);
	}

	void EventData::SetAttributeAsString(const std::string& strKey, const std::string& strValue)
	{
		SetEventDataAttributeValue(m_strSerialID, m_attributes, strKey, strValue);
	}
}