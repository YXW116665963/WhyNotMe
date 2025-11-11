#pragma once

#include "data_center_interface.h"

#include "observer_manager.h"
#include "data_manager.h"
#include "data_db.h"

namespace why
{
	class DataCenter : public IDataCenter
	{
	public:
		DataCenter();
		~DataCenter();

	public:
		// IDataCenter interface
		bool RegisterIntData(const char* pDomain, const char* pName, int32_t nDefault, DataStyle eStyle) override;
		bool RegisterUIntData(const char* pDomain, const char* pName, uint32_t uDefault, DataStyle eStyle) override;
		bool RegisterBigIntData(const char* pDomain, const char* pName, int64_t nDefault, DataStyle eStyle) override;
		bool RegisterBigUIntData(const char* pDomain, const char* pName, uint64_t uDefault, DataStyle eStyle) override;
		bool RegisterFloatData(const char* pDomain, const char* pName, float64_t fDefault, DataStyle eStyle) override;
		bool RegisterStringData(const char* pDomain, const char* pName, const char* pDefault, DataStyle eStyle) override;
		IListDataValue* RegisterListData(const char* pDomain, const char* pName, ValueType eType, uint32_t uSize, const char* pDefault, DataStyle eStyle) override;
		IObjectDataValue* RegisterObjectData(const char* pDomain, const char* pName, DataStyle eStyle) override;
		INamedData* FindData(const char* pDomain, const char* pName) override;
		bool HasDomain(const char* pDomain) override;
		IDataValue* GetDataValue(const char* pDomain, const char* pName) override;
		IListDataValue* GetListDataValue(const char* pDomain, const char* pName) override;
		IObjectDataValue* GetObjectDataValue(const char* pDomain, const char* pName) override;
		void UnregisterData(const char* pDomain, const char* pName) override;

		//<! 当pName=""时，表示全注册pDomain
		void Subscribe(const char* pDomain, const char* pName, IDataObserver* pObserver) override;
		void SubscribeEx(const char* pDomain, const char* pName, const char* pMember, IDataObserver* pObserver) override;
		void Unsubscribe(const char* pDomain, const char* pName, IDataObserver* pObserver) override;
		void UpdateData(const char* pDomain, const char* pName) override;
		void UpdateDataEx(const char* pDomain, const char* pName, const char* pMember) override;
		bool Clone(const char* pFromDomain, const char* pToDomain) override;
		void PrepareDestroy() override;
		void Destroy() override;

	public:
		bool Initialize(const char* pFileName);
		bool Export(const char* pDomain, why::DataList& dataList);

	private:
		DataDB					m_db;
		ObserverManager			m_observerMgr;
		DataManager				m_dataMgr;
	};
}