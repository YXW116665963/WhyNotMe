#include "dynamic_data_handler.h"
#include "data_exchange.h"

namespace why
{
	DynamicDataHandler::DynamicDataHandler(IDataCenter *pDataCenter, DataExchange* pDataExchange):
		m_pDataCenter(pDataCenter),
		m_pDataExchange(pDataExchange)
	{
		m_observerHelperPtr.reset(new why::DataObserverHelper(pDataCenter));
	}

	void DynamicDataHandler::Bind(BaseBindDataInfo& baseDataInfo, const std::vector<DataNameInfo>& arraySubscribeInfo)
	{
		if (!m_pDataExchange)
			return;

		WndInfo wndInfo;
		wndInfo.m_pWindow = baseDataInfo.m_pWindow;
		wndInfo.m_strChildName = baseDataInfo.m_strChildName;
		wndInfo.m_strDisplay = baseDataInfo.m_strDisplay;
		wndInfo.m_strFormat = baseDataInfo.m_strFormat;

		// get new name
		std::string strNewName = FormatNewName(
			baseDataInfo.m_strDomain,
			baseDataInfo.m_strName,
			arraySubscribeInfo);

		auto itData = FindFormatInfo({ baseDataInfo .m_strDomain, baseDataInfo .m_strName});
		if (itData == m_arrayFormatInfo.end())
		{
			BindDataFormatInfo    dataFormatInfo;
			dataFormatInfo.m_strDomain = baseDataInfo.m_strDomain;
			dataFormatInfo.m_strNameFormat = baseDataInfo.m_strName;
			dataFormatInfo.m_strCurName = strNewName;
			dataFormatInfo.m_arraySubscribeInfo = arraySubscribeInfo;
			dataFormatInfo.m_arrayWindowInfo.push_back(wndInfo);
			m_arrayFormatInfo.push_back(dataFormatInfo);

			for (const auto& item : arraySubscribeInfo)
			{
				m_observerHelperPtr->Subscribe(
					std::get<0>(item),
					std::get<1>(item),
					std::bind(
						&DynamicDataHandler::ChangeBind, 
						this, 
						std::placeholders::_1,
						dataFormatInfo.m_strDomain, 
						dataFormatInfo.m_strNameFormat));
			}
		}
		else
		{
			itData->m_arrayWindowInfo.push_back(wndInfo);
		}

		baseDataInfo.m_strName = strNewName;
		m_pDataExchange->Bind(baseDataInfo);
	}

	std::string DynamicDataHandler::FormatNewName(const std::string& strDoMain, const std::string& strNameFormat, const std::vector<DataNameInfo>& arraySubscribeInfo)
	{
		// get value
		std::vector<uint32_t> arrayValue;
		for (size_t i = 0; i < arraySubscribeInfo.size(); ++i)
		{
			auto value = m_pDataCenter->GetDataValue(std::get<0>(arraySubscribeInfo[i]).c_str(),
				std::get<1>(arraySubscribeInfo[i]).c_str());
			if (value)
				arrayValue.push_back(value->GetUIntValue());
		}

		// formatname
		size_t pos = 0;
		size_t count = 0;
		std::string result = strNameFormat;
		for (; pos != std::string::npos && count < arrayValue.size();)
		{
			pos = result.find("%u", pos);
			if (pos != std::string::npos)
			{
				result.replace(pos, 2, std::to_string(arrayValue[count]));
				pos += std::to_string(arrayValue[count++]).length();
			}
		}

		return result;
	}

	void DynamicDataHandler::ChangeBind(why::INamedData* pNameData, const std::string strDoMain, const std::string strNameFormat)
	{
		auto itData = FindFormatInfo({ strDoMain, strNameFormat });
		if (!m_pDataExchange || itData == m_arrayFormatInfo.end())
			return;

		auto strNewName = FormatNewName(strDoMain, strNameFormat, itData->m_arraySubscribeInfo);
		if(itData->m_strCurName == strNewName)
			return;

		m_pDataExchange->ChangeBind(strDoMain, itData->m_strCurName, strDoMain, strNewName);
		itData->m_strCurName = strNewName;
	}

	DynamicDataHandler::arrayFormatInfoIt DynamicDataHandler::FindFormatInfo(const DataNameInfo dataInfo)
	{
		auto itFind = m_arrayFormatInfo.end();
		for (auto item = m_arrayFormatInfo.begin(); item != m_arrayFormatInfo.end(); ++ item)
		{
			if (item->m_strDomain == std::get<0>(dataInfo) 
				&& item->m_strNameFormat == std::get<1>(dataInfo))
			{
				return item;
			}
		}
		return itFind;
	}
}