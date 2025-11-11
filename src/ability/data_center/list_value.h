#pragma once

#include "data_center_def.h"
#include "data_value_interface.h"
#include "value_watcher_guard.h"
#include "text_util.h"

#include <shared_mutex>

namespace why
{
	template<class T>
	class ListValue : public IListDataValue
	{
	public:
		ListValue(IValueWatcher* pValueWatcher, ValueType eType, uint32_t uSize, const T& defaultValue) :
			m_eType(eType),
			m_uSize(uSize),
			m_defaultValue(defaultValue),
			m_pValueWatcher(pValueWatcher)
		{
			if (m_uSize != 0) {
				m_datas.resize(m_uSize, m_defaultValue);
			}
		}

		~ListValue()
		{

		}

	public:
		// IDataValue interface
		ValueType Type() const override { return ValueType::eList; }
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

		bool Assign(IDataValue* pOther, bool& changed)
		{
			if (ValueType::eList != pOther->Type())
				return false;

			IListDataValue* pListTest = dynamic_cast<IListDataValue*>(pOther);
			if (!pListTest || pListTest->ChildType() != m_eType)
				return false;

			ListValue<T>* pDataValue = dynamic_cast<ListValue<T> *>(pOther);

			assert(pDataValue);
			uint32_t			uSize = pDataValue->m_datas.size();

			if (m_datas.size() != uSize)
			{
				changed = true;
				m_datas.resize(uSize, m_defaultValue);
			}

			for (uint32_t i = 0; i < uSize; i++)
			{
				bool one_changed = false;
				m_datas[i].Assign(&(pDataValue->m_datas[i]), one_changed);
				if (one_changed) {
					changed = true;
				}
			}

			if (changed && (nullptr != m_pValueWatcher))
				m_pValueWatcher->OnValueChanged(this);

			return true;
		}

		// IListDataValue interface
		ValueType ChildType() const override { return m_eType; }
		uint32_t Size() const override
		{
			ReadLockGuard		lockGuard(m_lock);

			return m_uSize;
		}

		void Resize(uint32_t uSize) override
		{
			bool			bChanged = false;

			{
				WriteLockGuard		lockGuard(m_lock);

				if ((0 != uSize) && (uSize != m_uSize))
				{
					m_uSize = uSize;
					m_datas.resize(m_uSize, m_defaultValue);
					bChanged = true;
				}
				else if (0 == uSize && uSize != m_uSize)
				{
					m_uSize = uSize;
					bChanged = true;
				}
			}

			if (bChanged && (nullptr != m_pValueWatcher))
				m_pValueWatcher->OnValueChanged(this);
		}

		IDataValue* GetAt(uint32_t uIndex) override
		{
			ReadLockGuard		lockGuard(m_lock);

			return (uIndex < m_uSize) ? &m_datas[uIndex] : nullptr;
		}

		const IDataValue* GetAt(uint32_t uIndex) const override
		{
			ReadLockGuard		lockGuard(m_lock);

			return (uIndex < m_uSize) ? &m_datas[uIndex] : nullptr;
		}

		int32_t GetAtAsInt(uint32_t uIndex) const override
		{
			ReadLockGuard		lockGuard(m_lock);

			return (uIndex < m_uSize) ? m_datas[uIndex].GetIntValue() : 0;
		}

		uint32_t GetAtAsUInt(uint32_t uIndex) const override
		{
			ReadLockGuard		lockGuard(m_lock);

			return (uIndex < m_uSize) ? m_datas[uIndex].GetUIntValue() : 0;
		}

		int64_t GetAtAsBigInt(uint32_t uIndex) const override
		{
			ReadLockGuard		lockGuard(m_lock);

			return (uIndex < m_uSize) ? m_datas[uIndex].GetBigIntValue() : 0;
		}

		uint64_t GetAtAsBigUInt(uint32_t uIndex) const override
		{
			ReadLockGuard		lockGuard(m_lock);

			return (uIndex < m_uSize) ? m_datas[uIndex].GetBigUIntValue() : 0;
		}

		float64_t GetAtAsFloat(uint32_t uIndex) const override
		{
			ReadLockGuard		lockGuard(m_lock);

			return (uIndex < m_uSize) ? m_datas[uIndex].GetFloatValue() : 0;
		}

		uint32_t GetAtAsString(uint32_t uIndex, char* pText, uint32_t uTextSize) const override
		{
			ReadLockGuard		lockGuard(m_lock);

			return (uIndex < m_uSize) ? m_datas[uIndex].GetStringValue(pText, uTextSize) : 0;
		}

		bool SetAtAsInt(uint32_t uIndex, int32_t nValue) override
		{
			bool					bChanged = false;

			{
				ValueWatcherGuard	watchGuard(m_pValueWatcher);
				ReadLockGuard		lockGuard(m_lock);

				if (uIndex >= m_uSize)
					return false;

				bChanged = m_datas[uIndex].SetIntValue(nValue);
			}

			if (bChanged && (nullptr != m_pValueWatcher))
				m_pValueWatcher->OnValueChanged(this);

			return bChanged;
		}

		bool SetAtAsUInt(uint32_t uIndex, uint32_t uValue) override
		{
			bool					bChanged = false;

			{
				ValueWatcherGuard	watchGuard(m_pValueWatcher);
				ReadLockGuard		lockGuard(m_lock);

				if (uIndex >= m_uSize)
					return false;

				bChanged = m_datas[uIndex].SetUIntValue(uValue);
			}

			if (bChanged && (nullptr != m_pValueWatcher))
				m_pValueWatcher->OnValueChanged(this);

			return bChanged;
		}

		bool SetAtAsBigInt(uint32_t uIndex, int64_t nValue) override
		{
			bool					bChanged = false;

			{
				ValueWatcherGuard	watchGuard(m_pValueWatcher);
				ReadLockGuard		lockGuard(m_lock);

				if (uIndex >= m_uSize)
					return false;

				bChanged = m_datas[uIndex].SetBigIntValue(nValue);
			}

			if (bChanged && (nullptr != m_pValueWatcher))
				m_pValueWatcher->OnValueChanged(this);

			return bChanged;
		}

		bool SetAtAsBigUInt(uint32_t uIndex, uint64_t uValue) override
		{
			bool					bChanged = false;

			{
				ValueWatcherGuard	watchGuard(m_pValueWatcher);
				ReadLockGuard		lockGuard(m_lock);

				if (uIndex >= m_uSize)
					return false;

				bChanged = m_datas[uIndex].SetBigUIntValue(uValue);
			}

			if (bChanged && (nullptr != m_pValueWatcher))
				m_pValueWatcher->OnValueChanged(this);

			return bChanged;
		}

		bool SetAtAsFloat(uint32_t uIndex, float64_t fValue) override
		{
			bool					bChanged = false;

			{
				ValueWatcherGuard	watchGuard(m_pValueWatcher);
				ReadLockGuard		lockGuard(m_lock);

				if (uIndex >= m_uSize)
					return false;

				bChanged = m_datas[uIndex].SetFloatValue(fValue);
			}

			if (bChanged && (nullptr != m_pValueWatcher))
				m_pValueWatcher->OnValueChanged(this);

			return bChanged;
		}

		bool SetAtAsString(uint32_t uIndex, const char* pValue) override
		{
			bool					bChanged = false;

			{
				ValueWatcherGuard	watchGuard(m_pValueWatcher);
				ReadLockGuard		lockGuard(m_lock);

				if (uIndex >= m_uSize)
					return false;

				bChanged = m_datas[uIndex].SetStringValue(pValue);
			}

			if (bChanged && (nullptr != m_pValueWatcher))
				m_pValueWatcher->OnValueChanged(this);

			return bChanged;
		}

	public:
		std::vector<T>& Data() { return m_datas; }

		bool SetValue(ValueType eType, const std::string& strValue, bool unknowSize = true)
		{
			if ((m_eType != eType) || (0 == strValue.length()))
			{
				return false;
			}
			else
			{
				std::vector<std::string>		strValues;
				uint32_t						uCount;

				StringSplit(strValue, ';', strValues);
				uCount = strValues.size();

				if (unknowSize)
				{
					if (uCount > m_uSize)
						Resize(uCount);
				}
				else
				{
					uCount = min(uCount, m_uSize);
				}

				switch (m_eType)
				{
				case ValueType::eInt:
					for (uint32_t i = 0; i < uCount; i++)
						m_datas[i].SetIntValue((int32_t)atoi(strValues[i].c_str()));
					break;
				case ValueType::eUInt:
					for (uint32_t i = 0; i < uCount; i++)
						m_datas[i].SetUIntValue((uint32_t)atoi(strValues[i].c_str()));
					break;
				case ValueType::eBigInt:
					for (uint32_t i = 0; i < uCount; i++)
						m_datas[i].SetBigIntValue((int64_t)_atoi64(strValues[i].c_str()));
					break;
				case ValueType::eBigUInt:
					for (uint32_t i = 0; i < uCount; i++)
						m_datas[i].SetBigUIntValue((uint64_t)_atoi64(strValues[i].c_str()));
					break;
				case ValueType::eFloat:
					for (uint32_t i = 0; i < uCount; i++)
						m_datas[i].SetFloatValue(atof(strValues[i].c_str()));
					break;
				case ValueType::eString:
					for (uint32_t i = 0; i < uCount; i++)
						m_datas[i].SetStringValue(strValues[i].c_str());
					break;
				default:
					break;
				}

				return true;
			}
		}

	private:
		typedef std::shared_lock<std::shared_mutex> ReadLockGuard;
		typedef std::unique_lock<std::shared_mutex> WriteLockGuard;

	private:
		IValueWatcher* m_pValueWatcher{ nullptr };
		mutable std::shared_mutex		m_lock;
		ValueType						m_eType;			//!< 此处的类型为数组成员的类型
		std::vector<T>					m_datas;
		uint32_t						m_uSize{ 0 };
		T                               m_defaultValue;
	};
}