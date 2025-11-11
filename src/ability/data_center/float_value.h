#pragma once

#include "data_value_interface.h"
#include "value_watcher_interface.h"

#include <shared_mutex>

namespace why
{
	class FloatValue : public IDataValue
	{
	public:
		FloatValue(IValueWatcher* pValueWatcher, float64_t fValue);
		FloatValue(const FloatValue& other);
		~FloatValue();
		FloatValue& operator=(const FloatValue& other);

	public:
		// IDataValue interface
		ValueType Type() const override { return ValueType::eFloat; }
		int32_t GetIntValue() const override { assert(false); return 0; }
		bool SetIntValue(int32_t /*nValue*/) override { assert(false); return 0; }
		uint32_t GetUIntValue() const override { assert(false); return 0; }
		bool SetUIntValue(uint32_t /*uValue*/) override { assert(false); return 0; }
		int64_t GetBigIntValue() const override { assert(false); return 0; }
		bool SetBigIntValue(int64_t /*nValue*/) override { assert(false); return false; };
		uint64_t GetBigUIntValue() const override { assert(false); return 0; }
		bool SetBigUIntValue(uint64_t /*uValue*/) override { assert(false); return false; };
		float64_t GetFloatValue() const override
		{
			ReadLockGuard			lockGuard(m_lock);

			return m_fData;
		}
		bool SetFloatValue(float64_t fValue) override;
		uint32_t GetStringValue(char* /*pText*/, uint32_t /*uTextSize*/) const override { assert(false); return 0; }
		bool SetStringValue(const char* /*pValue*/) override { assert(false); return false; }
		bool Assign(IDataValue* pOther, bool& changed) override;

	public:
		void InitData(float64_t fValue) { m_fData = fValue; }

	private:
		typedef std::shared_lock<std::shared_mutex> ReadLockGuard;
		typedef std::unique_lock<std::shared_mutex> WriteLockGuard;

	private:
		mutable std::shared_mutex		m_lock;
		IValueWatcher* m_pValueWatcher;
		float64_t						m_fData;
	};
}