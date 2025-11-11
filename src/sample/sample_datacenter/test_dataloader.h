#pragma once
#include <iostream>
#include <memory>
#include <functional>
#include "logger.h"


#include "data_center_interface.h"
#include "data_observer_helper.h"
typedef std::function<void(why::IDataCenter*)> DataCenterDeleter;
typedef std::unique_ptr<why::IDataCenter, DataCenterDeleter> DataCenterPtr;


//extern 关键字用于告诉编译器，某个变量是外部定义的，意味着它的定义在其他地方。extern 只应出现在头文件中，以便其他源文件能够使用该变量。
extern DataCenterPtr m_dataCenterPtr;
//通常情况下，在头文件中定义变量会导致多个源文件链接时的重复定义错误。
//在C++17中，引入了 inline 变量的概念，它允许在头文件中定义变量而不会造成重复定义的链接错误。
// 使用 inline 关键字定义的变量可以在多个翻译单元中共享，但它们只会在链接时合并为一个实例。

//constexpr 变量也可以在头文件中定义，因为它们在编译时是常量，并且每个翻译单元都会有自己的副本，因此不会导致重复定义问题。


//struct GlobalPointer
//{
//	DataCenterPtr m_dataCenterPtr{ nullptr, [](why::IDataCenter* pDataCenter) { pDataCenter->Destroy(); } };
//};
//
//extern GlobalPointer g_globalPointer;

namespace test_
{
	class test_DataLoader
	{
	public:
		test_DataLoader();
		~test_DataLoader() {};


		bool InitDataCenter();

		// 数据配置文件注册到DB
		bool ImportDataFromFile(why::IDataCenter* pDataCenter, const char* pDataFile, const char* pDomain, const char* pDB = "");
		bool LoadOneData(std::vector<std::string>& data);
		void RegisterData(const std::string& strDomain, const std::string& strName, why::ValueType eType, const std::string& strDefault, bool bPersistence);
		bool LoadAllData(std::vector<std::vector<std::string>>& dataList);
	private:
		std::string m_strConfigPath;			//用户数据缓存文件绝对路径
		std::string	m_strTestDirAbsPath;		//test数据文件夹绝对路径
		std::string	m_strDBFileAbsPath;			//.db文件绝对路径


		why::IDataCenter* m_pDataCenter{ nullptr };
		why::INamedData* m_pNamedData{ nullptr };
		why::IObjectDataValue* m_pObjectValue{ nullptr };
	};
}

namespace why
{
	class CallbackTest
	{
	public:
		CallbackTest(IDataCenter* pCenter);
		~CallbackTest() {};
	private:
		void OnWhyChanged(INamedData* pData);

	private:
		void SubscribeDatas(IDataCenter* pCenter)
		{
			if (nullptr == m_observerHelperPtr.get())
				m_observerHelperPtr.reset(new DataObserverHelper(pCenter));

			m_observerHelperPtr->Subscribe("people", "why", std::bind(&CallbackTest::OnWhyChanged, this, std::placeholders::_1));
		}
			
	private:
			std::unique_ptr<DataObserverHelper>		m_observerHelperPtr;

	//private:
	//	BEGIN_SUBSCRIBE_DATAS()
	//		ON_SUBSCRIBE_DATA("people", "why", CallbackTest::OnWhyChanged)
	//	END_SUBSCRIBE_DATAS()
	};
}



namespace why
{
	inline void AssertNullPtr(IDataValue* pDataValue)
	{
#if defined(DEV_MODE) 
		assert(nullptr != pDataValue);
#endif
	}

	inline int32_t GetIntValue(const char* pDomain, const char* pName)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pDataValue = m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->GetIntValue();
		else
			LOG_ERROR << "GetIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return 0;
	}

	inline int32_t GetIntValue(INamedData* pNameData)
	{
		assert(nullptr != pNameData);
		auto pDataValue = pNameData->GetValue();
		assert(nullptr != pDataValue);
		if ((nullptr != pDataValue) && (ValueType::eInt == pDataValue->Type()))
			return pDataValue->GetIntValue();
		return 0;
	}

	inline bool SetIntValue(const char* pDomain, const char* pName, const int32_t val)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pDataValue = m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->SetIntValue(val);
		else
			LOG_ERROR << "SetIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return false;
	}

	inline int32_t GetObjectIntValue(const char* pDomain, const char* pName, const char* pChildName)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pObjectDataValue = m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->GetAsInt(pChildName);
		else
			LOG_ERROR << "GetObjectIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return 0;
	}

	inline int32_t GetObjectIntValue(INamedData* pNameData, const char* pChildName)
	{
		assert(nullptr != pNameData);
		auto pObjectDataValue = dynamic_cast<IObjectDataValue*>(pNameData->GetValue());
		assert(nullptr != pObjectDataValue);
		if ((nullptr != pObjectDataValue) && (ValueType::eObject == pObjectDataValue->Type()))
			return pObjectDataValue->GetAsInt(pChildName);
		return 0;
	}

	inline bool SetObjectIntValue(const char* pDomain, const char* pName, const char* pChildName, const int32_t val)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pObjectDataValue = m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->SetAsInt(pChildName, val);
		else
			LOG_ERROR << "SetObjectIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return false;
	}

	inline int32_t GetListIntValueAt(const char* pDomain, const char* pName, uint32_t uIdnex)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pListDataValue = m_dataCenterPtr->GetListDataValue(pDomain, pName);
		if (nullptr != pListDataValue)
			return pListDataValue->GetAtAsInt(uIdnex);
		else
			LOG_ERROR << "GetListIntValueAt pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pListDataValue);
		return 0;
	}

	inline uint32_t GetUIntValue(const char* pDomain, const char* pName)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pDataValue = m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->GetUIntValue();
		else
			LOG_ERROR << "GetUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return 0;
	}

	inline uint32_t GetUIntValue(INamedData* pNameData)
	{
		assert(nullptr != pNameData);
		auto pDataValue = pNameData->GetValue();
		assert(nullptr != pDataValue);
		if ((nullptr != pDataValue) && (ValueType::eUInt == pDataValue->Type()))
			return pDataValue->GetUIntValue();
		return 0;
	}

	inline bool SetUIntValue(const char* pDomain, const char* pName, const uint32_t val)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pDataValue = m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->SetUIntValue(val);
		else
			LOG_ERROR << "SetUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return false;
	}

	inline uint32_t GetObjectUIntValue(const char* pDomain, const char* pName, const char* pChildName)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pObjectDataValue = m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->GetAsUInt(pChildName);
		else
			LOG_ERROR << "GetObjectUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return 0;
	}

	inline uint32_t GetObjectUIntValue(INamedData* pNameData, const char* pChildName)
	{
		assert(nullptr != pNameData);
		auto pObjectDataValue = dynamic_cast<IObjectDataValue*>(pNameData->GetValue());
		assert(nullptr != pObjectDataValue);
		if ((nullptr != pObjectDataValue) && (ValueType::eObject == pObjectDataValue->Type()))
			return pObjectDataValue->GetAsUInt(pChildName);
		return 0;
	}

	inline bool SetObjectUIntValue(const char* pDomain, const char* pName, const char* pChildName, const uint32_t val)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pObjectDataValue = m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->SetAsUInt(pChildName, val);
		else
			LOG_ERROR << "SetObjectUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return false;
	}

	inline uint32_t GetListUIntValueAt(const char* pDomain, const char* pName, uint32_t uIdnex)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pListDataValue = m_dataCenterPtr->GetListDataValue(pDomain, pName);
		if (nullptr != pListDataValue)
			return pListDataValue->GetAtAsUInt(uIdnex);
		else
			LOG_ERROR << "GetListUIntValueAt pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pListDataValue);
		return 0;
	}

	inline int64_t GetBigIntValue(const char* pDomain, const char* pName)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pDataValue = m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->GetBigIntValue();
		else
			LOG_ERROR << "GetBigIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return 0;
	}

	inline int64_t GetBigIntValue(INamedData* pNameData)
	{
		assert(nullptr != pNameData);
		auto pDataValue = pNameData->GetValue();
		assert(nullptr != pDataValue);
		if ((nullptr != pDataValue) && (ValueType::eBigInt == pDataValue->Type()))
			return pDataValue->GetBigIntValue();
		return 0;
	}

	inline bool SetBigIntValue(const char* pDomain, const char* pName, const int64_t val)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pDataValue = m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->SetBigIntValue(val);
		else
			LOG_ERROR << "SetBigIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return false;
	}

	inline bool GetObjectValue(const char* pDomain, const char* pName, IObjectDataValue* pObjectDataValue)
	{
		assert(nullptr != m_dataCenterPtr.get());
		pObjectDataValue = m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return true;
		else
			LOG_ERROR << "GetObjectBigIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pObjectDataValue);
		return false;
	}

	inline int64_t GetObjectBigIntValue(const char* pDomain, const char* pName, const char* pChildName)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pObjectDataValue = m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->GetAsBigInt(pChildName);
		else
			LOG_ERROR << "GetObjectBigIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return 0;
	}

	inline int64_t GetObjectBigIntValue(INamedData* pNameData, const char* pChildName)
	{
		assert(nullptr != pNameData);
		auto pObjectDataValue = dynamic_cast<IObjectDataValue*>(pNameData->GetValue());
		assert(nullptr != pObjectDataValue);
		if ((nullptr != pObjectDataValue) && (ValueType::eObject == pObjectDataValue->Type()))
			return pObjectDataValue->GetAsBigInt(pChildName);
		return 0;
	}

	inline bool SetObjectBigIntValue(const char* pDomain, const char* pName, const char* pChildName, const int64_t val)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pObjectDataValue = m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->SetAsBigInt(pChildName, val);
		else
			LOG_ERROR << "SetObjectBigIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return false;
	}

	inline int64_t GetListBigIntValueAt(const char* pDomain, const char* pName, uint32_t uIdnex)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pListDataValue = m_dataCenterPtr->GetListDataValue(pDomain, pName);
		if (nullptr != pListDataValue)
			return pListDataValue->GetAtAsBigInt(uIdnex);
		else
			LOG_ERROR << "GetListBigIntValueAt pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pListDataValue);
		return 0;
	}

	inline uint64_t GetBigUIntValue(const char* pDomain, const char* pName)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pDataValue = m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->GetBigUIntValue();
		else
			LOG_ERROR << "GetBigUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return 0;
	}

	inline uint64_t GetBigUIntValue(INamedData* pNameData)
	{
		assert(nullptr != pNameData);
		auto pDataValue = pNameData->GetValue();
		assert(nullptr != pDataValue);
		if ((nullptr != pDataValue) && (ValueType::eBigUInt == pDataValue->Type()))
			return pDataValue->GetBigUIntValue();
		return 0;
	}

	inline bool SetBigUIntValue(const char* pDomain, const char* pName, const uint64_t val)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pDataValue = m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->SetBigUIntValue(val);
		else
			LOG_ERROR << "SetBigUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return false;
	}

	inline uint64_t GetObjectBigUIntValue(const char* pDomain, const char* pName, const char* pChildName)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pObjectDataValue = m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->GetAsBigUInt(pChildName);
		else
			LOG_ERROR << "GetObjectBigUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return 0;
	}

	inline uint64_t GetObjectBigUIntValue(INamedData* pNameData, const char* pChildName)
	{
		assert(nullptr != pNameData);
		auto pObjectDataValue = dynamic_cast<IObjectDataValue*>(pNameData->GetValue());
		assert(nullptr != pObjectDataValue);
		if ((nullptr != pObjectDataValue) && (ValueType::eObject == pObjectDataValue->Type()))
			return pObjectDataValue->GetAsBigUInt(pChildName);
		return 0;
	}

	inline bool SetObjectBigUIntValue(const char* pDomain, const char* pName, const char* pChildName, const uint64_t val)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pObjectDataValue = m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->SetAsBigUInt(pChildName, val);
		else
			LOG_ERROR << "SetObjectBigUIntValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return false;
	}

	inline uint64_t GetListBigUIntValueAt(const char* pDomain, const char* pName, uint32_t uIdnex)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pListDataValue = m_dataCenterPtr->GetListDataValue(pDomain, pName);
		if (nullptr != pListDataValue)
			return pListDataValue->GetAtAsBigUInt(uIdnex);
		else
			LOG_ERROR << "GetListBigUIntValueAt pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pListDataValue);
		return 0;
	}

	inline float64_t GetFloatValue(const char* pDomain, const char* pName)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pDataValue = m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->GetFloatValue();
		else
			LOG_ERROR << "GetFloatValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return 0;
	}

	inline float64_t GetFloatValue(INamedData* pNameData)
	{
		assert(nullptr != pNameData);
		auto pDataValue = pNameData->GetValue();
		assert(nullptr != pDataValue);
		if ((nullptr != pDataValue) && (ValueType::eFloat == pDataValue->Type()))
			return pDataValue->GetFloatValue();
		return 0;
	}

	inline bool SetFloatValue(const char* pDomain, const char* pName, const float64_t val)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pDataValue = m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->SetFloatValue(val);
		else
			LOG_ERROR << "SetFloatValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return false;
	}

	inline float64_t GetObjectFloatValue(const char* pDomain, const char* pName, const char* pChildName)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pObjectDataValue = m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->GetAsFloat(pChildName);
		else
			LOG_ERROR << "GetObjectFloatValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return 0;
	}

	inline float64_t GetObjectFloatValue(INamedData* pNameData, const char* pChildName)
	{
		assert(nullptr != pNameData);
		auto pObjectDataValue = dynamic_cast<IObjectDataValue*>(pNameData->GetValue());
		assert(nullptr != pObjectDataValue);
		if ((nullptr != pObjectDataValue) && (ValueType::eObject == pObjectDataValue->Type()))
			return pObjectDataValue->GetAsFloat(pChildName);
		return 0;
	}

	inline bool SetObjectFloatValue(const char* pDomain, const char* pName, const char* pChildName, const float64_t val)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pObjectDataValue = m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
			return pObjectDataValue->SetAsFloat(pChildName, val);
		else
			LOG_ERROR << "SetObjectFloatValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		AssertNullPtr(pObjectDataValue);
		return false;
	}

	inline float64_t GetListFloatValueAt(const char* pDomain, const char* pName, uint32_t uIdnex)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pListDataValue = m_dataCenterPtr->GetListDataValue(pDomain, pName);
		if (nullptr != pListDataValue)
			return pListDataValue->GetAtAsFloat(uIdnex);
		else
			LOG_ERROR << "GetListFloatValueAt pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pListDataValue);
		return 0;
	}

	inline bool GetStringValue(const char* pDomain, const char* pName, std::string& strText)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pDataValue = m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
		{
			int32_t		nLen = pDataValue->GetStringValue(nullptr, 0);
			strText.resize(nLen, 0);
			pDataValue->GetStringValue(strText.data(), nLen);
			return true;
		}
		else
		{
			LOG_ERROR << "GetStringValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		}
		// AssertNullPtr(pDataValue);
		return false;
	}

	inline bool GetStringValue(INamedData* pNameData, std::string& strText)
	{
		assert(nullptr != pNameData);
		auto pDataValue = pNameData->GetValue();
		if ((nullptr != pDataValue) && (ValueType::eString == pDataValue->Type()))
		{
			int32_t		nLen = pDataValue->GetStringValue(nullptr, 0);

			strText.resize(nLen, 0);
			pDataValue->GetStringValue(strText.data(), nLen);
			return true;
		}
		else
		{
			LOG_ERROR << "GetStringValue pDataValue is nullptr,domain:" << pNameData->Domain() << ",name:" << pNameData->Name();
		}
		AssertNullPtr(pDataValue);
		return false;
	}

	inline bool GetObjectStringValue(const char* pDomain, const char* pName, const char* pChildName, std::string& strText)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pObjectDataValue = m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
		{
			int32_t		nLen = pObjectDataValue->GetAsString(pChildName, nullptr, 0);

			strText.resize(nLen, 0);
			pObjectDataValue->GetAsString(pChildName, strText.data(), nLen);
			return true;
		}
		else
		{
			LOG_ERROR << "GetObjectStringValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		}
		AssertNullPtr(pObjectDataValue);
		return 0;
	}

	inline bool SetObjectStringValue(const char* pDomain, const char* pName, const char* pChildName, const char* val)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pObjectDataValue = m_dataCenterPtr->GetObjectDataValue(pDomain, pName);
		if (nullptr != pObjectDataValue)
		{
			pObjectDataValue->SetAsString(pChildName, val);
			return true;
		}
		else
		{
			LOG_ERROR << "SetObjectStringValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName << ",childName:" << pChildName;
		}
		AssertNullPtr(pObjectDataValue);
		return false;
	}

	inline bool SetStringValue(const char* pDomain, const char* pName, const char* val)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pDataValue = m_dataCenterPtr->GetDataValue(pDomain, pName);
		if (nullptr != pDataValue)
			return pDataValue->SetStringValue(val);
		else
			LOG_ERROR << "SetStringValue pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pDataValue);
		return false;
	}

	inline bool GetListStringValueAt(const char* pDomain, const char* pName, uint32_t uIdnex, std::string& strText)
	{
		assert(nullptr != m_dataCenterPtr.get());
		auto pListDataValue = m_dataCenterPtr->GetListDataValue(pDomain, pName);
		if (nullptr != pListDataValue)
		{
			int32_t		nLen = pListDataValue->GetAtAsString(uIdnex, nullptr, 0);

			strText.resize(nLen, 0);
			return pListDataValue->GetAtAsString(uIdnex, strText.data(), uIdnex);
		}
		else
			LOG_ERROR << "GetListStringValueAt pDataValue is nullptr,domain:" << pDomain << ",name:" << pName;
		AssertNullPtr(pListDataValue);
		return false;
	}
}

