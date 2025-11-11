#include "interger_value.h"

namespace why
{
	IntergerValue::IntergerValue(IValueWatcher* pValueWatcher, ValueType eType, uint32_t uData)
		: m_pValueWatcher(pValueWatcher)
		, m_eType(eType)
		, m_uData(uData)
	{

	}

	IntergerValue::IntergerValue(const IntergerValue& other)
		: m_pValueWatcher(other.m_pValueWatcher)
		, m_eType(other.m_eType)
		, m_uData(other.m_uData)
	{

	}

	IntergerValue::~IntergerValue()
	{

	}

	IntergerValue& IntergerValue::operator=(const IntergerValue& other)
	{
		if (this != &other)
		{
			m_pValueWatcher = other.m_pValueWatcher;
			m_eType = other.m_eType;
			m_uData = other.m_uData;
		}

		return *this;
	}

	int32_t IntergerValue::GetIntValue() const
	{
		assert(ValueType::eInt == m_eType);
		int32_t					nData = 0;

		if (ValueType::eInt == m_eType)
		{
			ReadLockGuard		lockGuard(m_lock);

			nData = (int32_t)m_uData;
		}
		return nData;
	}

	uint32_t IntergerValue::GetUIntValue() const
	{
		assert(ValueType::eUInt == m_eType);
		uint32_t					uData = 0;

		if (ValueType::eUInt == m_eType)
		{
			ReadLockGuard		lockGuard(m_lock);

			uData = m_uData;
		}
		return uData;
	}

	bool IntergerValue::SetIntValue(int32_t nValue)
	{
		assert(ValueType::eInt == m_eType);
		bool				bChanged = false;

		if (ValueType::eInt != m_eType)
			return false;

		{
			WriteLockGuard			lockGuard(m_lock);

			if (m_uData != (uint32_t)nValue)
			{
				m_uData = (uint32_t)nValue;
				bChanged = true;
			}
		}

		if (bChanged && (nullptr != m_pValueWatcher))
			m_pValueWatcher->OnValueChanged(this);

		return bChanged;
	}

	bool IntergerValue::SetUIntValue(uint32_t uValue)
	{
		assert(ValueType::eUInt == m_eType);
		bool				bChanged = false;

		if (ValueType::eUInt != m_eType)
			return false;

		{
			WriteLockGuard			lockGuard(m_lock);

			if (m_uData != uValue)
			{
				m_uData = uValue;
				bChanged = true;
			}
		}

		// 数据订阅：值改变后，使用回调。
		if (bChanged && (nullptr != m_pValueWatcher))
			m_pValueWatcher->OnValueChanged(this);

		return bChanged;
	}

	bool IntergerValue::Assign(IDataValue* pOther, bool& changed)
	{
		if (m_eType != pOther->Type())
			return false;

		IntergerValue* pDataValue = dynamic_cast<IntergerValue*>(pOther);

		assert(pDataValue);
		changed = SetUIntValue(pDataValue->m_uData);

		return true;
	}
}