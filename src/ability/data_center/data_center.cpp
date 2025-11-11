#include "data_center.h"

namespace why
{
	DataCenter::DataCenter()
		: m_observerMgr(&m_db)
		, m_dataMgr(&m_observerMgr, &m_db)
	{

	}

	DataCenter::~DataCenter()
	{

	}

	bool DataCenter::Initialize(const char* pFileName)
	{
		if ((nullptr == pFileName) || (0 == strlen(pFileName)))
			return false;

		return m_db.Startup(pFileName);
	}

	bool DataCenter::RegisterIntData(const char* pDomain, const char* pName, int32_t nDefault, DataStyle eStyle)
	{
		if ((nullptr == pDomain) || (0 == strlen(pDomain)) || (nullptr == pName) || (0 == strlen(pName)))
			return false;

		return m_dataMgr.RegisterIntData(pDomain, pName, nDefault, eStyle);
	}

	bool DataCenter::RegisterUIntData(const char* pDomain, const char* pName, uint32_t uDefault, DataStyle eStyle)
	{
		if ((nullptr == pDomain) || (0 == strlen(pDomain)) || (nullptr == pName) || (0 == strlen(pName)))
			return false;

		return m_dataMgr.RegisterUIntData(pDomain, pName, uDefault, eStyle);
	}

	bool DataCenter::RegisterBigIntData(const char* pDomain, const char* pName, int64_t nDefault, DataStyle eStyle)
	{
		if ((nullptr == pDomain) || (0 == strlen(pDomain)) || (nullptr == pName) || (0 == strlen(pName)))
			return false;

		return m_dataMgr.RegisterBigIntData(pDomain, pName, nDefault, eStyle);
	}

	bool DataCenter::RegisterBigUIntData(const char* pDomain, const char* pName, uint64_t uDefault, DataStyle eStyle)
	{
		if ((nullptr == pDomain) || (0 == strlen(pDomain)) || (nullptr == pName) || (0 == strlen(pName)))
			return false;

		return m_dataMgr.RegisterBigUIntData(pDomain, pName, uDefault, eStyle);
	}

	bool DataCenter::RegisterFloatData(const char* pDomain, const char* pName, float64_t fDefault, DataStyle eStyle)
	{
		if ((nullptr == pDomain) || (0 == strlen(pDomain)) || (nullptr == pName) || (0 == strlen(pName)))
			return false;

		return m_dataMgr.RegisterFloatData(pDomain, pName, fDefault, eStyle);
	}

	bool DataCenter::RegisterStringData(const char* pDomain, const char* pName, const char* pDefault, DataStyle eStyle)
	{
		if ((nullptr == pDomain) || (0 == strlen(pDomain)) || (nullptr == pName) || (0 == strlen(pName)))
			return false;

		std::string			strDefault = pDefault ? pDefault : "";

		return m_dataMgr.RegisterStringData(pDomain, pName, strDefault, eStyle);
	}

	IListDataValue* DataCenter::RegisterListData(const char* pDomain, const char* pName, ValueType eType, uint32_t uSize, const char* pDefault, DataStyle eStyle)
	{
		if ((nullptr == pDomain) || (0 == strlen(pDomain)) || (nullptr == pName) || (0 == strlen(pName)))
			return nullptr;

		std::string			strDefault = pDefault ? pDefault : "";

		return m_dataMgr.RegisterListData(pDomain, pName, eType, uSize, strDefault, eStyle);
	}

	IObjectDataValue* DataCenter::RegisterObjectData(const char* pDomain, const char* pName, DataStyle eStyle)
	{
		if ((nullptr == pDomain) || (0 == strlen(pDomain)) || (nullptr == pName) || (0 == strlen(pName)))
			return nullptr;

		return m_dataMgr.RegisterObjectData(pDomain, pName, eStyle);
	}

	void DataCenter::UnregisterData(const char* pDomain, const char* pName)
	{
		if ((nullptr == pDomain) || (0 == strlen(pDomain)) || (nullptr == pName) || (0 == strlen(pName)))
			return;

		m_dataMgr.UnregisterData(pDomain, pName);
	}

	INamedData* DataCenter::FindData(const char* pDomain, const char* pName)
	{
		if ((nullptr == pDomain) || (0 == strlen(pDomain)) || (nullptr == pName) || (0 == strlen(pName)))
			return nullptr;

		return m_dataMgr.FindData(pDomain, pName);
	}

	IDataValue* DataCenter::GetDataValue(const char* pDomain, const char* pName)
	{
		auto* pFind = FindData(pDomain, pName);

		if (nullptr == pFind)
			return nullptr;

		return pFind->GetValue();
	}

	IListDataValue* DataCenter::GetListDataValue(const char* pDomain, const char* pName)
	{
		auto* pFind = FindData(pDomain, pName);

		if (nullptr == pFind)
			return nullptr;

		if (ValueType::eList != pFind->GetValue()->Type())
			return nullptr;

		return dynamic_cast<IListDataValue*>(pFind->GetValue());
	}

	IObjectDataValue* DataCenter::GetObjectDataValue(const char* pDomain, const char* pName)
	{
		auto* pFind = FindData(pDomain, pName);

		if (nullptr == pFind)
			return nullptr;

		if (ValueType::eObject != pFind->GetValue()->Type())
			return nullptr;

		return dynamic_cast<IObjectDataValue*>(pFind->GetValue());
	}

	void DataCenter::Subscribe(const char* pDomain, const char* pName, IDataObserver* pObserver)
	{
		SubscribeEx(pDomain, pName, nullptr, pObserver);
	}

	void DataCenter::SubscribeEx(const char* pDomain, const char* pName, const char* pMember, IDataObserver* pObserver)
	{
		if ((nullptr == pDomain) || (0 == strlen(pDomain)) || (nullptr == pName))
			return;

		if (nullptr == pObserver)
			return;

		std::string			strMember = pMember ? std::string(pMember) : std::string();

		m_observerMgr.Subscribe(pDomain, pName, strMember, pObserver);

		auto		pNamedData = m_dataMgr.FindData(pDomain, pName);

		if (pNamedData)
			m_observerMgr.OnValueChangedEx(pDomain, pName, strMember, pNamedData, pObserver);
	}

	void DataCenter::Unsubscribe(const char* pDomain, const char* pName, IDataObserver* pObserver)
	{
		if ((nullptr == pDomain) || (0 == strlen(pDomain)) || (nullptr == pName))
			return;

		if (nullptr == pObserver)
			return;

		m_observerMgr.Unsubscribe(pDomain, pName, pObserver);
	}

	void DataCenter::UpdateData(const char* pDomain, const char* pName)
	{
		UpdateDataEx(pDomain, pName, nullptr);
	}

	void DataCenter::UpdateDataEx(const char* pDomain, const char* pName, const char* pMember)
	{
		auto				pNameData = FindData(pDomain, pName);
		std::string			strMember = pMember ? std::string(pMember) : std::string();

		if (pNameData)
			m_observerMgr.OnValueChanged(pDomain, pName, strMember, pNameData);
	}

	bool DataCenter::Export(const char* pDomain, why::DataList& dataList)
	{
		return m_dataMgr.Export(pDomain, dataList);
	}

	void DataCenter::PrepareDestroy()
	{
		m_observerMgr.Stop();
	}

	bool DataCenter::Clone(const char* pFromDomain, const char* pToDomain)
	{
		return m_dataMgr.Clone(pFromDomain, pToDomain);
	}

	void DataCenter::Destroy()
	{
		delete this;
	}

	bool DataCenter::HasDomain(const char* pDomain)
	{
		if ((nullptr == pDomain) || (0 == strlen(pDomain)))
			return false;

		return m_dataMgr.HasDomain(pDomain);
	}

}

why::IDataCenter* CreateDataCenter(const char* pFileName)
{
	auto* pDataCenter = new why::DataCenter();

	if (!pDataCenter->Initialize(pFileName))
	{
		pDataCenter->Destroy();
		pDataCenter = nullptr;
	}

	return pDataCenter;
}