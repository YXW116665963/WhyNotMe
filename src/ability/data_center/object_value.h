#pragma once

#include "standard.h"

#include "data_center_def.h"
#include "data_value_interface.h"
#include "value_watcher_interface.h"

#include <shared_mutex>
#include <memory>
#include <functional>
#include <string>
#include <map>

namespace why
{
	class ObjectValue : public IObjectDataValue
	{
	public:
		ObjectValue(IValueWatcher* pValueWatcher);
		~ObjectValue();

	public:
		// IDataValue interface
		ValueType Type() const override { return ValueType::eObject; }
		int32_t GetIntValue() const override { assert(false); return 0; }
		bool SetIntValue(int32_t /*nValue*/) override { assert(false); return false; }
		uint32_t GetUIntValue() const override { assert(false); return 0; }
		bool SetUIntValue(uint32_t /*uValue*/) override { assert(false); return false; }
		int64_t GetBigIntValue() const override { assert(false); return 0; }
		bool SetBigIntValue(int64_t /*nValue*/) override { assert(false); return false; }
		uint64_t GetBigUIntValue() const override { assert(false); return 0; }
		bool SetBigUIntValue(uint64_t /*uValue*/) override { assert(false); return false; }
		float64_t GetFloatValue() const override { assert(false); return 0.0; }
		bool SetFloatValue(float64_t /*fValue*/) override { assert(false); return false; }
		uint32_t GetStringValue(char* /*pText*/, uint32_t /*uTextSize*/) const override { assert(false); return 0; }
		bool SetStringValue(const char* /*pValue*/) override { assert(false); return false; }
		bool Assign(IDataValue* pOther, bool& changed) override;

		// IObjectDataValue interface
		uint32_t Size() const override;
		const char* GetName(uint32_t uIndex) override;
		IDataValue* GetAt(uint32_t uIndex) override;
		const IDataValue* GetAt(uint32_t uIndex) const override;
		IDataValue* Get(const char* pName) override;
		const IDataValue* Get(const char* pName) const override;
		int32_t GetAsInt(const char* pName) const override;
		uint32_t GetAsUInt(const char* pName) const override;
		int64_t GetAsBigInt(const char* pName) const override;
		uint64_t GetAsBigUInt(const char* pName) const override;
		float64_t GetAsFloat(const char* pName) const override;
		uint32_t GetAsString(const char* pName, char* pText, uint32_t uTextSize) const override;
		//const std::string& GetAsString(const char* pName) const override;

		bool SetAsInt(const char* pName, int32_t nValue) override;
		bool SetAsUInt(const char* pName, uint32_t uValue) override;
		bool SetAsBigInt(const char* pName, int64_t nValue) override;
		bool SetAsBigUInt(const char* pName, uint64_t uValue) override;
		bool SetAsFloat(const char* pName, float64_t fValue) override;
		bool SetAsString(const char* pName, const char* pValue) override;

		bool SetListFloatValue(const char* pName, const char* pValue) override;
		bool SetListFloatOneValue(const char* pName, uint32_t uIndex, float64_t fValue) override;
		float64_t GetFloatOneValue(const char* pName, uint32_t uIndex) override;

	public:
		std::string GetValues(char separator) const;
		std::string GetMemberName(IDataValue* pDataValue) const;

	private:
		typedef std::shared_lock<std::shared_mutex> ReadLockGuard;
		typedef std::unique_lock<std::shared_mutex> WriteLockGuard;
		typedef std::function<void(IDataValue*)> DataValueDeleter;
		typedef std::unique_ptr<IDataValue, DataValueDeleter> DataValuePtr;
		typedef std::map<std::string, DataValuePtr> ChildDatas;

	private:
		mutable std::shared_mutex		m_lock;
		IValueWatcher* m_pValueWatcher;
		ChildDatas						m_childs;
	};
}