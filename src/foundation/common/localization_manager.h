#pragma once
#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include "csv.h"
#include "text_util.h"
#include "file_util.h"

#define  ENGLISH     1		///英文
#define	 S_CHINESE   2		///简体中文
#define  JAPANESE    3		///日语
#define  T_CHINESE   4		///繁体中文

namespace why 
{
	class LocalizationManager
	{
	public:
		static LocalizationManager& GetInstance()
		{
			static LocalizationManager instance;
			return instance;
		}

		LocalizationManager(const LocalizationManager&) = delete;
		LocalizationManager& operator=(const LocalizationManager&) = delete;

	protected:

		LocalizationManager()
		{

		}
		~LocalizationManager()
		{

		}
	private:
		template<typename T, typename... Args>
		void ReplaceArgs(std::string& str, int index, T first, Args... rest)
		{

			std::string token = "{" + std::to_string(index) + "}";
			size_t pos = 0;
			pos = str.find(token, pos);
			while (pos != std::string::npos)
			{
				std::ostringstream replaceValue;
				replaceValue << first;
				str.replace(pos, token.length(), replaceValue.str());
				pos += replaceValue.str().length();

				pos = str.find(token, pos);
			}
			ReplaceArgs(str, index + 1, rest...);
		}

		void ReplaceArgs(std::string& key, int index) {
		}


	public:

		template<typename... Args>
		std::string ToLocalString(const std::string& key, Args... args)
		{
			std::string	result = key;
			auto	itFind = m_datas.find(key);
			if (m_datas.end() != itFind) {
				result = itFind->second;
			}
			ReplaceArgs(result, 1, std::forward<Args>(args)...);
			return result;
		}

		bool InitFromCSV(const std::string&	csvFileName, int nLanguage)
		{
			m_CurrentLanguage = nLanguage;
			m_datas.clear();
			try
			{
				/*
				  备注：在 GBK 编码中，要读取编码正确，需要把 csv 的格式转换为 GB2312
				  如何转换：
					   金山在线文档导出的 默认为 utf8-bom ， cp 出一份 ，使用notepad++ 转  ANSI ，后，再 cp 内容到 该文件中 localization.csv



					#define  ENGLISH     1		///英文
					#define	 S_CHINESE   2		///简体中文
					#define  JAPANESE    3		///日语
					#define  T_CHINESE   4		///繁体中文
				*/
				std::string		strLocalFileName = UTF8ToLocal(csvFileName);
				io::CSVReader<4, io::trim_chars<>, io::double_quote_escape<',', '\"'>> reader(strLocalFileName);
				reader.read_header(io::ignore_extra_column, "zh", "tw", "en", "jp");
				std::string zh, tw, en, jp;
				int index = nLanguage - 1;
				if (index < 0 || index > 3)
				{
					index = 0;
				}
				while (reader.read_row(zh, tw, en, jp))
				{
					std::string key = UTF8Encode(Ascii2Unicode(zh));
					std::string col[4] = { en,zh,jp,tw };
					//进行一次GB2312转码utf8 
 					m_datas[key] = UTF8Encode(Ascii2Unicode(col[index]));

				}
			}
			catch (const std::exception& /*e*/)
			{
				return false;
			}
			return true;
		}

		int GetCurrentLanguage()
		{
			return m_CurrentLanguage;
		}

	private:
		typedef std::map<std::string, std::string> LocalDataMap;

	private:
		LocalDataMap			m_datas;
		int						m_CurrentLanguage;

	};
}

/**
* @brief 本地化翻译
* @param key 中文字符串
* @param agrs 格式化需要的参数 值，
* @return 本地化内容
	如：单品种全自动切割(2)
	多语言key：单品种全自动切割({1})
	方法调用: ToLocalString("单品种全自动切割({1})","2")
	实际字符替换为：单品种全自动切割(
	\
	如：单品种全自动切割(2)SET-1
	多语言key：单品种全自动切割({1}))SET-{2}
	方法调用: ToLocalString("单品种全自动切割({1})","2","1")
	实际字符替换为：单品种全自动切割(2)SET-1
*/
template<typename... Args>
inline std::string ToLocalString(const std::string& key, Args... args)
{
	return why::LocalizationManager::GetInstance().ToLocalString(key, std::forward<Args>(args)...);
};