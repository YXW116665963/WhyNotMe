#include "float_value.h"

namespace why
{
	namespace
	{
		float64_t g_epsinon = 0.00000001;
	}

	FloatValue::FloatValue(IValueWatcher* pValueWatcher, float64_t fValue)
		: m_pValueWatcher(pValueWatcher)
		, m_fData(fValue)
	{

	}

	FloatValue::FloatValue(const FloatValue& other)
		: m_pValueWatcher(other.m_pValueWatcher)
		, m_fData(other.m_fData)
	{

	}

	FloatValue::~FloatValue()
	{

	}

	FloatValue& FloatValue::operator=(const FloatValue& other)
	{
		if (this != &other)
		{
			m_pValueWatcher = other.m_pValueWatcher;
			m_fData = other.m_fData;
		}

		return *this;
	}

	bool FloatValue::SetFloatValue(float64_t fValue)
	{
		bool				bChanged = false;

		{
			WriteLockGuard			lockGuard(m_lock);

			if (abs(m_fData - fValue) > g_epsinon)
			{
				m_fData = fValue;
				bChanged = true;
			}
		}

		if (bChanged && (nullptr != m_pValueWatcher))
			m_pValueWatcher->OnValueChanged(this);

		return bChanged;
	}

	bool FloatValue::Assign(IDataValue* pOther, bool& changed)
	{
		if (ValueType::eFloat != pOther->Type())
			return false;

		FloatValue* pDataValue = dynamic_cast<FloatValue*>(pOther);

		assert(pDataValue);
		changed = SetFloatValue(pDataValue->m_fData);

		return true;
	}
}