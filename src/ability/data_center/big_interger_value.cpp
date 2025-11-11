#include "big_interger_value.h"

namespace why
{
	BigIntergerValue::BigIntergerValue(IValueWatcher* pValueWatcher, ValueType eType, uint64_t uData)
		: m_pValueWatcher(pValueWatcher)
		, m_eType(eType)
		, m_uData(uData)
	{

	}

	BigIntergerValue::BigIntergerValue(const BigIntergerValue& other)
		: m_pValueWatcher(other.m_pValueWatcher)
		, m_eType(other.m_eType)
		, m_uData(other.m_uData)
	{

	}

	BigIntergerValue::~BigIntergerValue()
	{

	}

	BigIntergerValue& BigIntergerValue::operator=(const BigIntergerValue& other)
	{
		if (this != &other)
		{
			m_pValueWatcher = other.m_pValueWatcher;
			m_eType = other.m_eType;
			m_uData = other.m_uData;
		}

		return *this;
	}

	int64_t BigIntergerValue::GetBigIntValue() const
	{
		assert(ValueType::eBigInt == m_eType);
		int64_t					nData = 0;

		if (ValueType::eBigInt == m_eType)
		{
			ReadLockGuard			lockGuard(m_lock);

			nData = (int64_t)m_uData;
		}
		return nData;
	}

	uint64_t BigIntergerValue::GetBigUIntValue() const
	{
		assert(ValueType::eBigUInt == m_eType);
		uint64_t					uData = 0;

		if (ValueType::eBigUInt == m_eType)
		{
			ReadLockGuard			lockGuard(m_lock);

			uData = m_uData;
		}
		return uData;
	}

	bool BigIntergerValue::SetBigIntValue(int64_t nValue)
	{
		bool				bChanged = false;

		if (ValueType::eBigInt != m_eType)
			return false;

		{
			WriteLockGuard			lockGuard(m_lock);

			if (m_uData != (uint64_t)nValue)
			{
				m_uData = (uint64_t)nValue;
				bChanged = true;
			}
		}

		if (bChanged && (nullptr != m_pValueWatcher))
			m_pValueWatcher->OnValueChanged(this);

		return bChanged;
	}

	bool BigIntergerValue::SetBigUIntValue(uint64_t uValue)
	{
		bool				bChanged = false;

		if (ValueType::eBigUInt != m_eType)
			return false;

		{
			WriteLockGuard			lockGuard(m_lock);

			if (m_uData != uValue)
			{
				m_uData = uValue;
				bChanged = true;
			}
		}

		if (bChanged && (nullptr != m_pValueWatcher))
			m_pValueWatcher->OnValueChanged(this);

		return bChanged;
	}

	bool BigIntergerValue::Assign(IDataValue* pOther, bool& changed)
	{
		if (m_eType != pOther->Type())
			return false;

		BigIntergerValue* pDataValue = dynamic_cast<BigIntergerValue*>(pOther);

		assert(pDataValue);
		changed = SetBigUIntValue(pDataValue->m_uData);

		return true;
	}
}