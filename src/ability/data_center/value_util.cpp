#include "value_util.h"
#include "interger_value.h"
#include "big_interger_value.h"
#include "float_value.h"
#include "string_value.h"
#include "object_value.h"
#include "list_value.h"
#include "text_util.h"
#include "object_value.h"


namespace why
{
	std::string ValueToString(const IDataValue* pDataValue)
	{
		std::string			strValue;
		ValueType			eType = pDataValue->Type();

		strValue.clear();
		switch (eType)
		{
		case ValueType::eInt:
			strValue = std::to_string(pDataValue->GetIntValue());
			break;
		case ValueType::eUInt:
			strValue = std::to_string(pDataValue->GetUIntValue());
			break;
		case ValueType::eBigInt:
			strValue = std::to_string(pDataValue->GetBigIntValue());
			break;
		case ValueType::eBigUInt:
			strValue = std::to_string(pDataValue->GetBigUIntValue());
			break;
		case ValueType::eFloat:
			strValue = std::to_string(pDataValue->GetFloatValue());
			break;
		case ValueType::eString:
			GetStringValue(pDataValue, strValue);
			break;
		case ValueType::eList:
			GetListValue(dynamic_cast<const IListDataValue*>(pDataValue), strValue);
			break;
		default:
			break;
		}

		return strValue;
	}

	bool GetStringValue(const IDataValue* pDataValue, std::string& strValue)
	{
		auto pStringValue = dynamic_cast<const StringValue*>(pDataValue);

		if (pStringValue)
		{
			strValue = pStringValue->GetStringValue();
			return true;
		}

		return false;
	}

	bool GetListValue(const IListDataValue* pListValue, std::string& strValue)
	{
		if (pListValue)
		{
			uint32_t		uCount = pListValue->Size();
			ValueType		eType = pListValue->ChildType();

			strValue.clear();
			switch (eType)
			{
			case ValueType::eInt:
				for (uint32_t i = 0; i < uCount; i++)
				{
					strValue += std::to_string(pListValue->GetAtAsInt(i));
					strValue += ";";
				}
				break;
			case ValueType::eUInt:
				for (uint32_t i = 0; i < uCount; i++)
				{
					strValue += std::to_string(pListValue->GetAtAsUInt(i));
					strValue += ";";
				}
				break;
			case ValueType::eBigInt:
				for (uint32_t i = 0; i < uCount; i++)
				{
					strValue += std::to_string(pListValue->GetAtAsBigInt(i));
					strValue += ";";
				}
				break;
			case ValueType::eBigUInt:
				for (uint32_t i = 0; i < uCount; i++)
				{
					strValue += std::to_string(pListValue->GetAtAsBigUInt(i));
					strValue += ";";
				}
				break;
			case ValueType::eFloat:
				for (uint32_t i = 0; i < uCount; i++)
				{
					strValue += std::to_string(pListValue->GetAtAsFloat(i));
					strValue += ";";
				}
				break;
			case ValueType::eString:
				for (uint32_t i = 0; i < uCount; i++)
				{
					auto pStringValue = dynamic_cast<const StringValue*>(pListValue->GetAt(i));

					if (pStringValue)
					{
						strValue += pStringValue->GetStringValue();
						strValue += ";";
					}
				}
				break;
			default:
				break;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool SetListValue(IListDataValue* pListValue, ValueType eType, const std::string& strValue)
	{
		if (pListValue)
		{
			std::vector<std::string>		strValues;
			uint32_t						uCount;

			StringSplit(strValue, ';', strValues);
			uCount = strValues.size();
			if (uCount > pListValue->Size())
				pListValue->Resize(uCount);

			switch (eType)
			{
			case ValueType::eInt:
				for (uint32_t i = 0; i < uCount; i++)
					pListValue->SetIntValue((int32_t)atoi(strValues[i].c_str()));
				break;
			case ValueType::eUInt:
				for (uint32_t i = 0; i < uCount; i++)
					pListValue->SetUIntValue((uint32_t)atoi(strValues[i].c_str()));
				break;
			case ValueType::eBigInt:
				for (uint32_t i = 0; i < uCount; i++)
					pListValue->SetBigIntValue((int64_t)_atoi64(strValues[i].c_str()));
				break;
			case ValueType::eBigUInt:
				for (uint32_t i = 0; i < uCount; i++)
					pListValue->SetBigUIntValue((uint64_t)_atoi64(strValues[i].c_str()));
				break;
			case ValueType::eFloat:
				for (uint32_t i = 0; i < uCount; i++)
					pListValue->SetFloatValue(atof(strValues[i].c_str()));
				break;
			case ValueType::eString:
				for (uint32_t i = 0; i < uCount; i++)
					pListValue->SetStringValue(strValues[i].c_str());
				break;
			default:
				break;
			}

			return true;
		}
		else
		{
			return false;
		}
	}

	bool GetObjectValue(const IObjectDataValue* pObjectValue, std::string& strValue)
	{
		if (pObjectValue)
		{
			//why 对象类型实际存储值
			auto			pValue = dynamic_cast<const ObjectValue*>(pObjectValue);

			if (pValue)
			{
				strValue = pValue->GetValues(':');
				return true;
			}
		}

		return false;
	}

	//why 加载数据库中字符串为obj对象的函数
	bool SetObjectValue(IObjectDataValue* pObjectValue, const std::string& strValue)
	{
		if (pObjectValue)
		{
			std::vector<std::string>		strValues;
			uint32_t						uCount = 0;
			ValueType						eType{ ValueType::eInt };

			StringSplit(strValue, ':', strValues);
			uCount = strValues.size() / 3;

			for (uint32_t i = 0, uIndex = 0; i < uCount; i++, uIndex += 3)
			{
				eType = static_cast<ValueType>(atoi(strValues[uIndex].c_str()));
				switch (eType)
				{
				case ValueType::eInt:
					pObjectValue->SetAsInt(strValues[uIndex + 1].c_str(), atoi(strValues[uIndex + 2].c_str()));
					break;
				case ValueType::eUInt:
					pObjectValue->SetAsUInt(strValues[uIndex + 1].c_str(), (uint32_t)atoi(strValues[uIndex + 2].c_str()));
					break;
				case ValueType::eBigInt:
					pObjectValue->SetAsBigInt(strValues[uIndex + 1].c_str(), _atoi64(strValues[uIndex + 2].c_str()));
					break;
				case ValueType::eBigUInt:
					pObjectValue->SetAsBigUInt(strValues[uIndex + 1].c_str(), (uint64_t)_atoi64(strValues[uIndex + 2].c_str()));
					break;
				case ValueType::eFloat:
					pObjectValue->SetAsFloat(strValues[uIndex + 1].c_str(), atof(strValues[uIndex + 2].c_str()));
					break;
				case ValueType::eString:
					pObjectValue->SetAsString(strValues[uIndex + 1].c_str(), strValues[uIndex + 2].c_str());
					break;
				default:
					break;
				}
			}

			return true;
		}
		else
		{
			return false;
		}
	}

	IListDataValue* CloneListValue(IListDataValue* pListValue, IValueWatcher* pValueWatcher)
	{
		ValueType				eValueType = pListValue->ChildType();
		uint32_t				uCount = pListValue->Size();
		IListDataValue* pNewValue = nullptr;

		assert(uCount > 0);
		switch (eValueType)
		{
		case ValueType::eInt:
		case ValueType::eUInt:
		{
			IntergerValue* pMember = dynamic_cast<IntergerValue*>(pListValue->GetAt(0));
			pNewValue = new ListValue<IntergerValue>(pValueWatcher, eValueType, uCount, *pMember);
		}
		break;
		case ValueType::eBigInt:
		case ValueType::eBigUInt:
		{
			BigIntergerValue* pMember = dynamic_cast<BigIntergerValue*>(pListValue->GetAt(0));
			pNewValue = new ListValue<BigIntergerValue>(pValueWatcher, eValueType, uCount, *pMember);
		}
		break;
		case ValueType::eFloat:
		{
			FloatValue* pMember = dynamic_cast<FloatValue*>(pListValue->GetAt(0));
			pNewValue = new ListValue<FloatValue>(pValueWatcher, eValueType, uCount, *pMember);
		}
		break;
		case ValueType::eString:
		{
			StringValue* pMember = dynamic_cast<StringValue*>(pListValue->GetAt(0));
			pNewValue = new ListValue<StringValue>(pValueWatcher, eValueType, uCount, *pMember);
		}
		break;
		case ValueType::eList:
			assert(false);
			break;
		case ValueType::eObject:
			assert(false);
			break;
		default:
			break;
		}

		return pNewValue;
	}

	IDataValue* CloneValue(IDataValue* pDataValue, IValueWatcher* pValueWatcher)
	{
		ValueType			eValueType = pDataValue->Type();
		IDataValue* pNewValue = nullptr;
		bool				bChanged = false;

		switch (eValueType)
		{
		case ValueType::eInt:
			pNewValue = new IntergerValue(pValueWatcher, eValueType, pDataValue->GetIntValue());
			break;
		case ValueType::eUInt:
			pNewValue = new IntergerValue(pValueWatcher, eValueType, pDataValue->GetUIntValue());
			break;
		case ValueType::eBigInt:
			pNewValue = new BigIntergerValue(pValueWatcher, eValueType, pDataValue->GetBigIntValue());
			break;
		case ValueType::eBigUInt:
			pNewValue = new BigIntergerValue(pValueWatcher, eValueType, pDataValue->GetBigUIntValue());
			break;
		case ValueType::eFloat:
			pNewValue = new FloatValue(pValueWatcher, pDataValue->GetFloatValue());
			break;
		case ValueType::eString:
			pNewValue = new StringValue(pValueWatcher, std::string());
			pNewValue->Assign(pDataValue, bChanged);
			break;
		case ValueType::eList:
			pNewValue = CloneListValue(dynamic_cast<IListDataValue*>(pDataValue), pValueWatcher);
			pNewValue->Assign(pDataValue, bChanged);
			break;
		case ValueType::eObject:
			pNewValue = new ObjectValue(pValueWatcher);
			pNewValue->Assign(pDataValue, bChanged);
			break;
		default:
			break;
		}

		return pNewValue;
	}
}