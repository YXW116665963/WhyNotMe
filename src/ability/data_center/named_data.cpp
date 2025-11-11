#include "named_data.h"

#include "util.h"
#include "text_util.h"

#include "interger_value.h"
#include "big_interger_value.h"
#include "float_value.h"
#include "string_value.h"
#include "list_value.h"
#include "object_value.h"
#include "value_util.h"

namespace why
{
	NamedData::NamedData(ObserverManager* pObserverMgr, const std::string& strDomain, const std::string& strName, DataStyle eStyle)
		: m_pObserverMgr(pObserverMgr)
		, m_strDomain(strDomain)
		, m_strName(strName)
		, m_eStyle(eStyle)
	{

	}

	NamedData::~NamedData()
	{

	}

	const char* NamedData::Domain() const
	{
		return m_strDomain.c_str();
	}

	const char* NamedData::Name() const
	{
		return m_strName.c_str();
	}

	IDataValue* NamedData::GetValue()
	{
		return m_value.get();
	}

	void NamedData::PauseNotify()
	{
		m_bPauseNotify = true;
	}

	void NamedData::ResumeNotify()
	{
		if (m_bPauseNotify)
		{
			m_bPauseNotify = false;
			if (m_bChanged)
			{
				if (m_pObserverMgr)
				{
					std::set<std::string> tempMembers;

					{
						WriteLockGuard		lockGuard(m_lock);
						for (auto member : m_members)
						{
							tempMembers.insert(member);
						}
						m_members.clear();
					}

					for (auto member : tempMembers)
					{
						m_pObserverMgr->OnValueChanged(m_strDomain, m_strName, member, this);
					}

					if (ValueType::eObject == m_value->Type())
						m_pObserverMgr->OnValueChanged(m_strDomain, m_strName, std::string(), this);
				}
				m_bChanged = false;
			}
		}
	}

	bool NamedData::CreateIntValue(int32_t nDefault)
	{
		IDataValue* pDataValue = new IntergerValue(this, ValueType::eInt, nDefault);
		DataValuePtr			valueDataPtr(pDataValue, InterfaceDeleter<IntergerValue, IDataValue>());

		m_value.swap(valueDataPtr);
		return true;
	}

	bool NamedData::CreateUIntValue(uint32_t uDefault)
	{
		IDataValue* pDataValue = new IntergerValue(this, ValueType::eUInt, uDefault);
		DataValuePtr			valueDataPtr(pDataValue, InterfaceDeleter<IntergerValue, IDataValue>());

		m_value.swap(valueDataPtr);
		return true;
	}

	bool NamedData::CreateBigIntValue(int64_t nDefault)
	{
		IDataValue* pDataValue = new BigIntergerValue(this, ValueType::eBigInt, nDefault);
		DataValuePtr			valueDataPtr(pDataValue, InterfaceDeleter<BigIntergerValue, IDataValue>());

		m_value.swap(valueDataPtr);
		return true;
	}

	bool NamedData::CreateBigUIntValue(uint64_t uDefault)
	{
		IDataValue* pDataValue = new BigIntergerValue(this, ValueType::eBigUInt, uDefault);
		DataValuePtr			valueDataPtr(pDataValue, InterfaceDeleter<BigIntergerValue, IDataValue>());

		m_value.swap(valueDataPtr);
		return true;
	}

	bool NamedData::CreateFloatValue(float64_t fDefault)
	{
		IDataValue* pDataValue = new FloatValue(this, fDefault);
		DataValuePtr			valueDataPtr(pDataValue, InterfaceDeleter<FloatValue, IDataValue>());

		m_value.swap(valueDataPtr);
		return true;
	}

	bool NamedData::CreateStringValue(const std::string& strDefault)
	{
		IDataValue* pDataValue = new StringValue(this, strDefault);
		DataValuePtr			valueDataPtr(pDataValue, InterfaceDeleter<StringValue, IDataValue>());

		m_value.swap(valueDataPtr);
		return true;
	}

	IListDataValue* NamedData::CreateListValue(ValueType eType, uint32_t uSize, const std::string& strDefault)
	{
		IListDataValue* pDataValue = nullptr;

		switch (eType)
		{
		case ValueType::eInt:
			pDataValue = CreateIntListValue(uSize, strDefault);
			break;
		case ValueType::eUInt:
			pDataValue = CreateUIntListValue(uSize, strDefault);
			break;
		case ValueType::eBigInt:
			pDataValue = CreateBigIntListValue(uSize, strDefault);
			break;
		case ValueType::eBigUInt:
			pDataValue = CreateBigUIntListValue(uSize, strDefault);
			break;
		case ValueType::eFloat:
			pDataValue = CreateFloatListValue(uSize, strDefault);
			break;
		case ValueType::eString:
			pDataValue = CreateStringListValue(uSize, strDefault);
			break;
		default:
			break;
		}

		return pDataValue;
	}

	IObjectDataValue* NamedData::CreateObjectValue()
	{
		ObjectValue* pDataValue = new ObjectValue(this);
		DataValuePtr			valueDataPtr(pDataValue, InterfaceDeleter<ObjectValue, IDataValue>());

		m_value.swap(valueDataPtr);
		return pDataValue;
	}

	void NamedData::SetReady()
	{
		if (!m_bReady)
		{
			m_bReady = true;
			if (m_pObserverMgr)
				m_pObserverMgr->OnValueReady(m_strDomain, m_strName, this);
		}
	}

	IListDataValue* NamedData::CreateIntListValue(uint32_t uSize, const std::string& strDefault)
	{
		IntergerValue			initData(nullptr, ValueType::eInt, 0);
		auto					pDataValue = new ListValue<IntergerValue>(this, ValueType::eInt, uSize, initData);
		DataValuePtr			valueDataPtr(pDataValue, InterfaceDeleter<ListValue<IntergerValue>, IDataValue>());

		m_value.swap(valueDataPtr);
		if (0 != strDefault.length())
		{
			std::vector<std::string>		defaultValues;
			uint32_t						uCount;
			auto& childDatas = pDataValue->Data();

			StringSplit(strDefault, ';', defaultValues);
			uCount = defaultValues.size();
			for (uint32_t i = 0; (i < uCount) && (i < uSize); i++)
				childDatas[i].InitData((int32_t)atoi(defaultValues[i].c_str()));
		}

		return pDataValue;
	}

	IListDataValue* NamedData::CreateUIntListValue(uint32_t uSize, const std::string& strDefault)
	{
		IntergerValue			initData(nullptr, ValueType::eUInt, 0);
		auto					pDataValue = new ListValue<IntergerValue>(this, ValueType::eUInt, uSize, initData);
		DataValuePtr			valueDataPtr(pDataValue, InterfaceDeleter<ListValue<IntergerValue>, IDataValue>());

		m_value.swap(valueDataPtr);
		if (0 != strDefault.length())
		{
			std::vector<std::string>		defaultValues;
			uint32_t						uCount;
			auto& childDatas = pDataValue->Data();

			StringSplit(strDefault, ';', defaultValues);
			uCount = defaultValues.size();
			for (uint32_t i = 0; (i < uCount) && (i < uSize); i++)
				childDatas[i].InitData((uint32_t)atoi(defaultValues[i].c_str()));
		}

		return pDataValue;
	}

	IListDataValue* NamedData::CreateBigIntListValue(uint32_t uSize, const std::string& strDefault)
	{
		BigIntergerValue		initData(nullptr, ValueType::eBigInt, 0);
		auto					pDataValue = new ListValue<BigIntergerValue>(this, ValueType::eBigInt, uSize, initData);
		DataValuePtr			valueDataPtr(pDataValue, InterfaceDeleter<ListValue<BigIntergerValue>, IDataValue>());

		m_value.swap(valueDataPtr);
		if (0 != strDefault.length())
		{
			std::vector<std::string>		defaultValues;
			uint32_t						uCount;
			auto& childDatas = pDataValue->Data();

			StringSplit(strDefault, ';', defaultValues);
			uCount = defaultValues.size();
			for (uint32_t i = 0; (i < uCount) && (i < uSize); i++)
				childDatas[i].InitData(_atoi64(defaultValues[i].c_str()));
		}

		return pDataValue;
	}

	IListDataValue* NamedData::CreateBigUIntListValue(uint32_t uSize, const std::string& strDefault)
	{
		BigIntergerValue		initData(nullptr, ValueType::eBigUInt, 0);
		auto					pDataValue = new ListValue<BigIntergerValue>(this, ValueType::eBigUInt, uSize, initData);
		DataValuePtr			valueDataPtr(pDataValue, InterfaceDeleter<ListValue<BigIntergerValue>, IDataValue>());

		m_value.swap(valueDataPtr);
		if (0 != strDefault.length())
		{
			std::vector<std::string>		defaultValues;
			uint32_t						uCount;
			auto& childDatas = pDataValue->Data();

			StringSplit(strDefault, ';', defaultValues);
			uCount = defaultValues.size();
			for (uint32_t i = 0; (i < uCount) && (i < uSize); i++)
				childDatas[i].InitData((uint64_t)_atoi64(defaultValues[i].c_str()));
		}

		return pDataValue;
	}

	IListDataValue* NamedData::CreateFloatListValue(uint32_t uSize, const std::string& strDefault)
	{
		FloatValue				initData(nullptr, 0.0);
		auto					pDataValue = new ListValue<FloatValue>(this, ValueType::eFloat, uSize, initData);
		DataValuePtr			valueDataPtr(pDataValue, InterfaceDeleter<ListValue<FloatValue>, IDataValue>());

		m_value.swap(valueDataPtr);
		if (0 != strDefault.length())
		{
			std::vector<std::string>		defaultValues;
			uint32_t						uCount;
			auto& childDatas = pDataValue->Data();

			StringSplit(strDefault, ';', defaultValues);
			uCount = defaultValues.size();
			for (uint32_t i = 0; (i < uCount) && (i < uSize); i++)
				childDatas[i].InitData(atof(defaultValues[i].c_str()));
		}

		return pDataValue;
	}

	IListDataValue* NamedData::CreateStringListValue(uint32_t uSize, const std::string& strDefault)
	{
		std::string				strEmpty = "";
		StringValue				initData(nullptr, strEmpty);
		auto					pDataValue = new ListValue<StringValue>(this, ValueType::eString, uSize, initData);
		DataValuePtr			valueDataPtr(pDataValue, InterfaceDeleter<ListValue<StringValue>, IDataValue>());

		m_value.swap(valueDataPtr);
		if (0 != strDefault.length())
		{
			std::vector<std::string>		defaultValues;
			uint32_t						uCount;
			auto& childDatas = pDataValue->Data();

			StringSplit(strDefault, ';', defaultValues);
			uCount = defaultValues.size();
			for (uint32_t i = 0; (i < uCount) && (i < uSize); i++)
				childDatas[i].InitData(defaultValues[i]);
		}

		return pDataValue;
	}

	void NamedData::Pause()
	{
		m_uWatchPausRef++;
	}

	void NamedData::Resume()
	{
		m_uWatchPausRef--;
	}

	void NamedData::OnValueChanged(IDataValue* pDataValue)
	{
		if (m_bReady && (0 == m_uWatchPausRef))
		{
			std::string			strMember = GetMemberName(pDataValue);

			if (m_bPauseNotify)
			{
				{
					WriteLockGuard		lockGuard(m_lock);
					m_bChanged = true;
					m_members.insert(strMember);
				}
			}
			else
			{
				if (m_pObserverMgr)
				{
					m_pObserverMgr->OnValueChanged(m_strDomain, m_strName, strMember, this);
					if (!strMember.empty())
						m_pObserverMgr->OnValueChanged(m_strDomain, m_strName, std::string(), this);
				}
			}
		}
	}

	bool NamedData::CreateValue(NamedData* pOther)
	{
		ValueWatcherGuard		watchGuard(this);
		IDataValue* pDataValue = CloneValue(pOther->m_value.get(), this);
		DataValuePtr			valueDataPtr(pDataValue, pOther->m_value.get_deleter());

		m_value.swap(valueDataPtr);
		return (nullptr != pDataValue);
	}

	bool NamedData::Assign(INamedData* pOther, bool& changed)
	{
		NamedData* pOtherData = dynamic_cast<NamedData*>(pOther);

		if (nullptr == pOtherData)
			return false;

		m_eStyle = pOtherData->m_eStyle;
		m_bReady = pOtherData->m_bReady;
		m_bPauseNotify = pOtherData->m_bPauseNotify;
		m_bChanged = pOtherData->m_bChanged;

		if (m_value->Type() != pOtherData->m_value->Type())
			return false;

		m_value->Assign(pOtherData->m_value.get(), changed);
		return true;
	}

	std::string NamedData::GetMemberName(IDataValue* pDataValue)
	{
		if (ValueType::eObject == m_value->Type())
		{
			ObjectValue* pObjectValue = dynamic_cast<ObjectValue*>(m_value.get());

			assert(nullptr != pObjectValue);
			return pObjectValue->GetMemberName(pDataValue);
		}
		else
		{
			return std::string();
		}
	}
}