#pragma once

#include "data_value_interface.h"
#include "value_watcher_interface.h"

#include <shared_mutex>

namespace why
{
	class StringValue : public IDataValue
	{
	public:
		StringValue(IValueWatcher* pValueWatcher, const std::string& strValue);
		StringValue(const StringValue& other);
		~StringValue();
		StringValue& operator=(const StringValue& other);

	public:
		// IDataValue
		ValueType Type() const override { return ValueType::eString; }
		int32_t GetIntValue() const override { assert(false); return 0; }
		bool SetIntValue(int32_t /*nValue*/) override { assert(false); return 0; }
		uint32_t GetUIntValue() const override { assert(false); return 0; }
		bool SetUIntValue(uint32_t /*uValue*/) override { assert(false); return 0; }
		int64_t GetBigIntValue() const override { assert(false); return 0; }
		bool SetBigIntValue(int64_t /*nValue*/) override { assert(false); return false; };
		uint64_t GetBigUIntValue() const override { assert(false); return 0; }
		bool SetBigUIntValue(uint64_t /*uValue*/) override { assert(false); return false; };
		float64_t GetFloatValue() const override { assert(false); return 0.0; }
		bool SetFloatValue(float64_t /*fValue*/) override { assert(false); return false; };
		uint32_t GetStringValue(char* pText, uint32_t uTextSize) const override;
		bool SetStringValue(const char* pValue) override;
		bool Assign(IDataValue* pOther, bool& changed) override;

	public:
		void InitData(const std::string& strValue) { m_strData = strValue; }
		const std::string& GetStringValue() const { return m_strData; }

	private:
		typedef std::shared_lock<std::shared_mutex> ReadLockGuard;
		typedef std::unique_lock<std::shared_mutex> WriteLockGuard;

	private:
		mutable std::shared_mutex		m_lock;
		IValueWatcher* m_pValueWatcher;
		std::string						m_strData;
	};
}