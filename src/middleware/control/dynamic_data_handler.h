#pragma once

#include <vector>
#include <tuple>
#include <memory>
#include "data_observer_helper.h"
#include "text_util.h"
#include "gui_import.h"

namespace why
{
	using DataNameInfo = std::tuple<std::string, std::string>;
	class DataExchange;
	struct BaseBindDataInfo;

	class DynamicDataHandler
	{
	public:
		DynamicDataHandler(IDataCenter *pDataCenter, DataExchange* pDataExchange);
		~DynamicDataHandler() {};

		void Bind(BaseBindDataInfo& baseDataInfo, const std::vector<DataNameInfo>& arraySubscribeInfo);
	
	private:
		struct WndInfo
		{
			wxWindow*  m_pWindow{nullptr};
			std::string m_strChildName{""};
			std::string m_strFormat{""};
			std::string m_strDisplay{""};
		};

		struct BindDataFormatInfo
		{
			std::string m_strDomain;
			std::string m_strNameFormat;
			std::string m_strCurName;
			std::vector<DataNameInfo> m_arraySubscribeInfo;
			std::vector<WndInfo>    m_arrayWindowInfo;
		};

		using arrayBindFormatInfo = std::vector<BindDataFormatInfo>;
		using arrayFormatInfoIt = arrayBindFormatInfo::iterator;

	private:
		std::string FormatNewName(const std::string& strDoMain, const std::string& strNameFormat, const std::vector<DataNameInfo>& arraySubscribeInfo);
		void ChangeBind(why::INamedData* pNameData, const std::string strDoMain, const std::string strNameFormat);
		arrayFormatInfoIt FindFormatInfo(const DataNameInfo dataInfo);
		
	private:
		IDataCenter*            m_pDataCenter;
		DataExchange*           m_pDataExchange;
		std::unique_ptr<DataObserverHelper> m_observerHelperPtr;
		std::vector<BindDataFormatInfo> m_arrayFormatInfo;
	};
}
