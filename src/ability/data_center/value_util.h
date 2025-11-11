#pragma once

#include "standard.h"

#include "data_value_interface.h"
#include "value_watcher_interface.h"

namespace why
{
	std::string ValueToString(const IDataValue* pDataValue);

	bool GetStringValue(const IDataValue* pDataValue, std::string& strValue);
	bool GetListValue(const IListDataValue* pListValue, std::string& strValue);
	bool GetObjectValue(const IObjectDataValue* pObjectValue, std::string& strValue);

	bool SetListValue(IListDataValue* pListValue, ValueType eType, const std::string& strValue);
	bool SetObjectValue(IObjectDataValue* pObjectValue, const std::string& strValue);

	IListDataValue* CloneListValue(IListDataValue* pDataValue, IValueWatcher* pValueWatcher);
	IDataValue* CloneValue(IDataValue* pDataValue, IValueWatcher* pValueWatcher);
}