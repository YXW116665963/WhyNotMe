#include "string_value.h"

namespace why
{
	StringValue::StringValue(IValueWatcher* pValueWatcher, const std::string& strValue)
		: m_pValueWatcher(pValueWatcher)
		, m_strData(strValue)
	{

	}

	StringValue::StringValue(const StringValue& other)
		: m_pValueWatcher(other.m_pValueWatcher)
		, m_strData(other.m_strData)
	{

	}

	StringValue::~StringValue()
	{

	}

	StringValue& StringValue::operator=(const StringValue& other)
	{
		if (this != &other)
		{
			m_pValueWatcher = other.m_pValueWatcher;
			m_strData = other.m_strData;
		}

		return *this;
	}

	uint32_t StringValue::GetStringValue(char* pText, uint32_t uTextSize) const
	{
		ReadLockGuard				lockGuard(m_lock);
		uint32_t					uNeedSize = m_strData.length();

		if ((nullptr == pText) || (uTextSize < uNeedSize))
			return uNeedSize;

		strcpy_s(pText, uNeedSize + 1, m_strData.c_str());
		return uNeedSize;
	}

	bool StringValue::SetStringValue(const char* pValue)
	{
		bool				bChanged = false;
		std::string			strValue = pValue ? pValue : "";

		{
			WriteLockGuard			lockGuard(m_lock);

			if (m_strData != strValue)
			{
				m_strData = strValue;
				bChanged = true;
			}
		}

		if (bChanged && (nullptr != m_pValueWatcher))
			m_pValueWatcher->OnValueChanged(this);

		return bChanged;
	}

	bool StringValue::Assign(IDataValue* pOther, bool& changed)
	{
		if (ValueType::eString != pOther->Type())
			return false;

		StringValue* pDataValue = dynamic_cast<StringValue*>(pOther);

		assert(pDataValue);
		changed = SetStringValue(pDataValue->m_strData.c_str());

		return true;
	}
}