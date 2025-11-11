#include "object_value.h"

#include "util.h"
#include "text_util.h"
#include "value_util.h"

#include "interger_value.h"
#include "big_interger_value.h"
#include "float_value.h"
#include "string_value.h"

#include "list_value.h"
#include "value_watcher_guard.h"
namespace why
{
	ObjectValue::ObjectValue(IValueWatcher* pValueWatcher)
		: m_pValueWatcher(pValueWatcher)
	{
		assert(m_pValueWatcher);
	}

	ObjectValue::~ObjectValue()
	{

	}

	uint32_t ObjectValue::Size() const
	{
		ReadLockGuard			lockGuard(m_lock);

		return m_childs.size();
	}

	const char* ObjectValue::GetName(uint32_t uIndex)
	{
		ReadLockGuard			lockGuard(m_lock);

		if (uIndex >= m_childs.size())
			return nullptr;

		auto			itFind = m_childs.begin();

		std::advance(itFind, uIndex);
		return itFind->first.c_str();
	}

	IDataValue* ObjectValue::GetAt(uint32_t uIndex)
	{
		ReadLockGuard			lockGuard(m_lock);

		if (uIndex >= m_childs.size())
			return nullptr;

		auto			itFind = m_childs.begin();

		std::advance(itFind, uIndex);
		return itFind->second.get();
	}

	const IDataValue* ObjectValue::GetAt(uint32_t uIndex) const
	{
		ReadLockGuard			lockGuard(m_lock);

		if (uIndex >= m_childs.size())
			return nullptr;

		auto			itFind = m_childs.begin();

		std::advance(itFind, uIndex);
		return itFind->second.get();
	}

	IDataValue* ObjectValue::Get(const char* pName)
	{
		ReadLockGuard			lockGuard(m_lock);

		if ((nullptr == pName) || (0 == strlen(pName)))
			return nullptr;

		auto			itFind = m_childs.find(pName);

		if (itFind != m_childs.end())
			return itFind->second.get();
		else
			return nullptr;
	}

	const IDataValue* ObjectValue::Get(const char* pName) const
	{
		ReadLockGuard			lockGuard(m_lock);

		if ((nullptr == pName) || (0 == strlen(pName)))
			return nullptr;

		auto			itFind = m_childs.find(pName);

		if (itFind != m_childs.end())
			return itFind->second.get();
		else
			return nullptr;
	}

	int32_t ObjectValue::GetAsInt(const char* pName) const
	{
		ReadLockGuard			lockGuard(m_lock);

		if ((nullptr == pName) || (0 == strlen(pName)))
			return 0;

		auto			itFind = m_childs.find(pName);

		if (itFind != m_childs.end())
			return itFind->second->GetIntValue();
		else
			return 0;
	}

	uint32_t ObjectValue::GetAsUInt(const char* pName) const
	{
		ReadLockGuard			lockGuard(m_lock);

		if ((nullptr == pName) || (0 == strlen(pName)))
			return 0;

		auto			itFind = m_childs.find(pName);

		if (itFind != m_childs.end())
			return itFind->second->GetUIntValue();
		else
			return 0;
	}

	int64_t ObjectValue::GetAsBigInt(const char* pName) const
	{
		ReadLockGuard			lockGuard(m_lock);

		if ((nullptr == pName) || (0 == strlen(pName)))
			return 0;

		auto			itFind = m_childs.find(pName);

		if (itFind != m_childs.end())
			return itFind->second->GetBigIntValue();
		else
			return 0;
	}

	uint64_t ObjectValue::GetAsBigUInt(const char* pName) const
	{
		ReadLockGuard			lockGuard(m_lock);

		if ((nullptr == pName) || (0 == strlen(pName)))
			return 0;

		auto			itFind = m_childs.find(pName);

		if (itFind != m_childs.end())
			return itFind->second->GetBigUIntValue();
		else
			return 0;
	}

	float64_t ObjectValue::GetAsFloat(const char* pName) const
	{
		ReadLockGuard			lockGuard(m_lock);

		if ((nullptr == pName) || (0 == strlen(pName)))
			return 0.0;

		auto			itFind = m_childs.find(pName);

		if (itFind != m_childs.end())
			return itFind->second->GetFloatValue();
		else
			return 0.0;
	}

	uint32_t ObjectValue::GetAsString(const char* pName, char* pText, uint32_t uTextSize) const
	{
		ReadLockGuard			lockGuard(m_lock);

		if ((nullptr == pName) || (0 == strlen(pName)))
			return 0;

		auto			itFind = m_childs.find(pName);

		if (itFind != m_childs.end())
			return itFind->second->GetStringValue(pText, uTextSize);
		else
			return 0;
	}


	bool ObjectValue::SetAsInt(const char* pName, int32_t nValue)
	{
		if ((nullptr == pName) || (0 == strlen(pName)))
			return false;

		bool					bChanged = false;
		IDataValue* pDataValue = nullptr;

		{
			ValueWatcherGuard		watchGuard(m_pValueWatcher);
			WriteLockGuard			lockGuard(m_lock);
			auto					itFind = m_childs.find(pName);

			if (itFind != m_childs.end())
			{
				bChanged = itFind->second->SetIntValue(nValue);
				pDataValue = itFind->second.get();
			}
			else
			{
				DataValuePtr			valueDataPtr(new IntergerValue(m_pValueWatcher, ValueType::eInt, nValue), InterfaceDeleter<IntergerValue, IDataValue>());

				pDataValue = valueDataPtr.get();
				m_childs.insert(std::make_pair(pName, std::move(valueDataPtr)));
				bChanged = true;
			}
		}

		if (bChanged && (nullptr != m_pValueWatcher))
			m_pValueWatcher->OnValueChanged(pDataValue);

		return bChanged;
	}

	bool ObjectValue::SetAsUInt(const char* pName, uint32_t uValue)
	{
		if ((nullptr == pName) || (0 == strlen(pName)))
			return false;

		bool					bChanged = false;
		IDataValue* pDataValue = nullptr;

		{
			ValueWatcherGuard		watchGuard(m_pValueWatcher);
			WriteLockGuard			lockGuard(m_lock);
			auto					itFind = m_childs.find(pName);

			if (itFind != m_childs.end())
			{
				pDataValue = itFind->second.get();
				bChanged = itFind->second->SetUIntValue(uValue);
			}
			else
			{

				DataValuePtr			valueDataPtr(new IntergerValue(m_pValueWatcher, ValueType::eUInt, uValue), InterfaceDeleter<IntergerValue, IDataValue>());

				pDataValue = valueDataPtr.get();
				m_childs.insert(std::make_pair(pName, std::move(valueDataPtr)));
				bChanged = true;
			}
		}

		if (bChanged && (nullptr != m_pValueWatcher))
			m_pValueWatcher->OnValueChanged(pDataValue);

		return true;
	}

	bool ObjectValue::SetAsBigInt(const char* pName, int64_t nValue)
	{
		if ((nullptr == pName) || (0 == strlen(pName)))
			return false;

		bool					bChanged = false;
		IDataValue* pDataValue = nullptr;

		{
			ValueWatcherGuard		watchGuard(m_pValueWatcher);
			WriteLockGuard			lockGuard(m_lock);
			auto					itFind = m_childs.find(pName);

			if (itFind != m_childs.end())
			{
				bChanged = itFind->second->SetBigIntValue(nValue);
				pDataValue = itFind->second.get();
			}
			else
			{
				DataValuePtr			valueDataPtr(new BigIntergerValue(m_pValueWatcher, ValueType::eBigInt, nValue), InterfaceDeleter<BigIntergerValue, IDataValue>());

				pDataValue = valueDataPtr.get();
				m_childs.insert(std::make_pair(pName, std::move(valueDataPtr)));
				bChanged = true;
			}
		}

		if (bChanged && (nullptr != m_pValueWatcher))
			m_pValueWatcher->OnValueChanged(pDataValue);

		return true;
	}

	bool ObjectValue::SetAsBigUInt(const char* pName, uint64_t uValue)
	{
		if ((nullptr == pName) || (0 == strlen(pName)))
			return false;

		bool					bChanged = false;
		IDataValue* pDataValue = nullptr;

		{
			ValueWatcherGuard		watchGuard(m_pValueWatcher);
			WriteLockGuard			lockGuard(m_lock);
			auto					itFind = m_childs.find(pName);

			if (itFind != m_childs.end())
			{
				bChanged = itFind->second->SetBigIntValue(uValue);
				pDataValue = itFind->second.get();
			}
			else
			{
				DataValuePtr			valueDataPtr(new BigIntergerValue(m_pValueWatcher, ValueType::eBigUInt, uValue), InterfaceDeleter<BigIntergerValue, IDataValue>());

				pDataValue = valueDataPtr.get();
				m_childs.insert(std::make_pair(pName, std::move(valueDataPtr)));
				bChanged = true;
			}
		}

		if (bChanged && (nullptr != m_pValueWatcher))
			m_pValueWatcher->OnValueChanged(pDataValue);

		return true;
	}

	bool ObjectValue::SetAsFloat(const char* pName, float64_t fValue)
	{
		if ((nullptr == pName) || (0 == strlen(pName)))
			return false;

		bool					bChanged = false;
		IDataValue* pDataValue = nullptr;

		{
			ValueWatcherGuard		watchGuard(m_pValueWatcher);
			WriteLockGuard			lockGuard(m_lock);
			auto					itFind = m_childs.find(pName);

			if (itFind != m_childs.end())
			{
				bChanged = itFind->second->SetFloatValue(fValue);
				pDataValue = itFind->second.get();
			}
			else
			{
				DataValuePtr			valueDataPtr(new FloatValue(m_pValueWatcher, fValue), InterfaceDeleter<FloatValue, IDataValue>());

				pDataValue = valueDataPtr.get();
				m_childs.insert(std::make_pair(pName, std::move(valueDataPtr)));
				bChanged = true;
			}
		}

		if (bChanged && (nullptr != m_pValueWatcher))
			m_pValueWatcher->OnValueChanged(pDataValue);

		return true;
	}

	bool ObjectValue::SetAsString(const char* pName, const char* pValue)
	{
		if ((nullptr == pName) || (0 == strlen(pName)))
			return false;

		bool					bChanged = false;
		IDataValue* pDataValue = nullptr;

		{
			ValueWatcherGuard		watchGuard(m_pValueWatcher);
			WriteLockGuard			lockGuard(m_lock);
			auto					itFind = m_childs.find(pName);

			if (itFind != m_childs.end())
			{
				bChanged = itFind->second->SetStringValue(pValue);
				pDataValue = itFind->second.get();
			}
			else
			{
				DataValuePtr			valueDataPtr(new StringValue(m_pValueWatcher, pValue), InterfaceDeleter<StringValue, IDataValue>());

				pDataValue = valueDataPtr.get();
				m_childs.insert(std::make_pair(pName, std::move(valueDataPtr)));
				bChanged = true;
			}
		}

		if (bChanged && (nullptr != m_pValueWatcher))
			m_pValueWatcher->OnValueChanged(pDataValue);

		return true;
	}

	bool ObjectValue::SetListFloatValue(const char* pName, const char* pValue)
	{
		if ((nullptr == pName) || (0 == strlen(pName)))
			return false;

		bool					bChanged = false;
		IDataValue* pDataValue = nullptr;

		{
			ValueWatcherGuard		watchGuard(m_pValueWatcher);
			WriteLockGuard			lockGuard(m_lock);
			auto					itFind = m_childs.find(pName);

			if (itFind != m_childs.end())
			{
				auto pList = dynamic_cast<IListDataValue*>(itFind->second.get());

				if (pList)
				{
					pList->SetValue(ValueType::eFloat, pValue);
					pDataValue = pList;
					bChanged = true;
				}
			}
			else
			{
				FloatValue				initData(nullptr, 0.0);
				auto				    pNewDataValue = new ListValue<FloatValue>(m_pValueWatcher, ValueType::eFloat, 0, initData);
				DataValuePtr			valueDataPtr(pNewDataValue, InterfaceDeleter<ListValue<FloatValue>, IDataValue>());

				pNewDataValue->SetValue(ValueType::eFloat, pValue);
				pDataValue = pNewDataValue;
				m_childs.insert(std::make_pair(pName, std::move(valueDataPtr)));
				bChanged = true;
			}
		}

		if (bChanged && (nullptr != m_pValueWatcher))
			m_pValueWatcher->OnValueChanged(pDataValue);

		return true;
	}

	bool ObjectValue::SetListFloatOneValue(const char* pName, uint32_t uIndex, float64_t fValue)
	{
		if ((nullptr == pName) || (0 == strlen(pName)))
			return false;

		bool					bChanged = false;
		IDataValue* pDataValue = nullptr;

		{
			ValueWatcherGuard		watchGuard(m_pValueWatcher);
			WriteLockGuard			lockGuard(m_lock);
			auto					itFind = m_childs.find(pName);

			if (itFind != m_childs.end())
			{
				auto pList = dynamic_cast<IListDataValue*>(itFind->second.get());

				if (pList && pList->Size() > uIndex && uIndex >= 0)
				{
					bChanged = pList->SetAtAsFloat(uIndex, fValue);
					pDataValue = pList;
				}
			}
			else
			{
				return false;
			}
		}

		if (bChanged && (nullptr != m_pValueWatcher))
			m_pValueWatcher->OnValueChanged(pDataValue);

		return true;
	}

	float64_t ObjectValue::GetFloatOneValue(const char* pName, uint32_t uIndex)
	{
		if ((nullptr == pName) || (0 == strlen(pName)))
			return 0.0f;

		{
			WriteLockGuard			lockGuard(m_lock);
			auto					itFind = m_childs.find(pName);

			if (itFind != m_childs.end())
			{
				auto pList = dynamic_cast<IListDataValue*>(itFind->second.get());

				if (pList && pList->Size() > uIndex && uIndex >= 0) {
					return pList->GetAtAsFloat(uIndex);
				}
			}
		}

		return 0.0f;
	}

	std::string ObjectValue::GetValues(char separator) const
	{
		std::string			strValues;

		for (auto it = m_childs.begin(); it != m_childs.end(); it++)
		{
			strValues += std::to_string((uint32_t)it->second->Type());
			strValues += separator;
			strValues += it->first;
			strValues += separator;
			strValues += ValueToString(it->second.get());
			strValues += separator;
		}

		return strValues;
	}

	std::string ObjectValue::GetMemberName(IDataValue* pDataValue) const
	{
		std::string			strMember;

		if (this == pDataValue)
			return strMember;

		for (auto it = m_childs.begin(); it != m_childs.end(); it++)
		{
			if (pDataValue == it->second.get())
			{
				strMember = it->first;
				break;
			}
			else
			{
				if (ValueType::eList == it->second->Type())
				{
					IListDataValue* pListValue = dynamic_cast<IListDataValue*>(it->second.get());
					uint32_t			uCount = pListValue->Size();
					bool				bFind = false;

					for (uint32_t i = 0; i < uCount; i++)
					{
						if (pDataValue == pListValue->GetAt(i))
						{
							bFind = true;
							break;
						}
					}

					if (bFind)
					{
						strMember = it->first;
						break;
					}
				}
			}
		}

		return strMember;
	}

	bool ObjectValue::Assign(IDataValue* pOther, bool& changed)
	{
		if (Type() != pOther->Type())
			return false;

		ObjectValue* pDataValue = dynamic_cast<ObjectValue*>(pOther);
		std::vector<IDataValue*>	updateMembers;

		assert(pDataValue);
		for (auto it = pDataValue->m_childs.begin(); it != pDataValue->m_childs.end(); it++)
		{
			auto			itFind = m_childs.find(it->first);

			if (itFind == m_childs.end())
			{
				DataValuePtr			valueDataPtr(CloneValue(it->second.get(), m_pValueWatcher), it->second.get_deleter());

				updateMembers.push_back(valueDataPtr.get());
				m_childs.insert(std::make_pair(it->first, std::move(valueDataPtr)));
				changed = true;
			}
			else
			{
				bool one_change = false;
				if (!itFind->second->Assign(it->second.get(), one_change))
				{
					DataValuePtr			valueDataPtr(CloneValue(it->second.get(), m_pValueWatcher), it->second.get_deleter());

					updateMembers.push_back(valueDataPtr.get());
					itFind->second.swap(valueDataPtr);
					changed = true;
				}
				else
				{
					if (one_change)
					{
						updateMembers.push_back(itFind->second.get());
						changed = true;
					}
				}
			}
		}

		if (changed && (nullptr != m_pValueWatcher))
		{
			uint32_t			uSize = (uint32_t)updateMembers.size();

			for (uint32_t i = 0; i < uSize; i++)
				m_pValueWatcher->OnValueChanged(updateMembers[i]);
		}

		return true;
	}
}