#pragma once

#include "data_value_interface.h"
#include "value_watcher_interface.h"

#include <shared_mutex>

namespace why
{
	class BigIntergerValue : public IDataValue
	{
	public:
		BigIntergerValue(IValueWatcher* pValueWatcher, ValueType eType, uint64_t uData);
		BigIntergerValue(const BigIntergerValue& other);
		~BigIntergerValue();
		BigIntergerValue& operator=(const BigIntergerValue& other);

	public:
		// IDataValue interface
		ValueType Type() const override { return m_eType; }
		int32_t GetIntValue() const override { assert(false); return 0; }
		bool SetIntValue(int32_t /*nValue*/) override { assert(false); return false; }
		uint32_t GetUIntValue() const override { assert(false); return 0; }
		bool SetUIntValue(uint32_t /*uValue*/) override { assert(false); return false; }
		int64_t GetBigIntValue() const override;
		bool SetBigIntValue(int64_t nValue) override;
		uint64_t GetBigUIntValue() const override;
		bool SetBigUIntValue(uint64_t uValue) override;
		float64_t GetFloatValue() const override { assert(false); return 0.0; }
		bool SetFloatValue(float64_t /*fValue*/) override { assert(false); return false; };
		//bool SetFloatValueNotify(float64_t /*fValue*/) override { assert(false); return false; };
		uint32_t GetStringValue(char* /*pText*/, uint32_t /*uTextSize*/) const override { assert(false); return 0; }
		bool SetStringValue(const char* /*pValue*/) override { assert(false); return false; };
		bool Assign(IDataValue* pOther, bool& changed) override;

	public:
		void InitData(uint64_t uData) { m_uData = uData; }

	private:
		typedef std::shared_lock<std::shared_mutex> ReadLockGuard;
		typedef std::unique_lock<std::shared_mutex> WriteLockGuard;

	private:
		mutable std::shared_mutex		m_lock;
		IValueWatcher* m_pValueWatcher;
		ValueType						m_eType;
		uint64_t						m_uData;
	};
}