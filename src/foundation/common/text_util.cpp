#include "text_util.h"

#include <locale>
#include <codecvt>
#include <vector>
#include <algorithm>

#include "util.h"
#include "logger.h"
namespace why
{
	std::string UTF8Encode(const std::wstring &strInput)
	{
#if defined(WIN32) || defined(WIN64)
		std::string strOutput;
		LPSTR pstrRes = nullptr;
		
		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, strInput.c_str(), strInput.size(), 0, 0, nullptr, nullptr);
		
		if (nLen > 0)
		{
			pstrRes = new CHAR[nLen + 1];
			memset(pstrRes, 0, nLen + 1);
		}
		else
		{
			return "";
		}

		::WideCharToMultiByte(CP_UTF8, 0, strInput.c_str(), strInput.size(), pstrRes, nLen, nullptr, nullptr);
		pstrRes[nLen] = 0;
		strOutput = pstrRes;
		delete[]pstrRes;

		return strOutput;
#else
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		std::string										 strOutput = conv.to_bytes(strInput).c_str();

		return strOutput;
#endif
	}

	std::wstring UTF8Decode(const std::string &strInput)
	{
#if defined(WIN32) || defined(WIN64)
		std::wstring strOutput;
		wchar_t *pstrRes = nullptr;

		int nLen = ::MultiByteToWideChar(CP_UTF8, 0, strInput.c_str(), strInput.size(), nullptr, 0);
		if (nLen > 0)
		{
			pstrRes = new wchar_t[nLen + 1];
			memset(pstrRes, 0, nLen + 1);
		}
		else
		{
			return L"";
	}

		::MultiByteToWideChar(CP_UTF8, 0, strInput.c_str(), strInput.size(), pstrRes, nLen);
		pstrRes[nLen] = 0;
		strOutput = pstrRes;
		delete[]pstrRes;

		return strOutput;
#else
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::wstring    strOutput = L"";
        
        try {
            strOutput = conv.from_bytes(strInput);
        }
        catch (const std::range_error&) {
            strOutput = conv.from_bytes(strInput.substr(0, conv.converted()));
        }
        
		return strOutput;
#endif
	}


	std::string URLDecode(const std::string &src) {
		std::string result;
		char tmp[3];
		for (unsigned int i = 0; i < src.length(); ++i) {
			if (src[i] == '%' && i + 2 < src.length()) {
				tmp[0] = src[++i];
				tmp[1] = src[++i];
				tmp[2] = '\0';

				result += static_cast<char>(strtol(tmp, 0, 16));
			} else if (src[i] == '+') {
				result += ' ';
			} else {
				result += src[i];
			}
		}

		return result;
	}

	std::string URLEncode(const std::string &src) {
		std::string escaped;
		int max = src.length();

		for (int i = 0; i < max; i++) {
			if ((48 <= src[i] && src[i] <= 57) ||//0-9
				(65 <= src[i] && src[i] <= 90) ||//ABC...XYZ
				(97 <= src[i] && src[i] <= 122) || //abc...xyz
				(src[i] == '~' || src[i] == '-' || src[i] == '_' || src[i] == '.')
				) {
				escaped.append(&src[i], 1);
			} else {
				escaped.append("%");
				escaped.append(ToHex(src[i]));//converts char 255 to string "FF"
			}
		}
		return escaped;
	}

	void StringSplit(const std::string &str, std::string::value_type separator, std::vector<std::string> &strList)
	{
		std::string::const_iterator		it;
		std::string::const_iterator		itBegin;

		strList.clear();

		itBegin = str.begin(),
			it = itBegin;

		for (; it != str.end(); it++)
		{
			if (*it == separator)
			{
				strList.push_back(std::string(itBegin, it));
				itBegin = it + 1;
			}
		}

		if (itBegin != it)
			strList.push_back(std::string(itBegin, it));
	}

	void StringSplit(const std::wstring &str, std::wstring::value_type separator, std::vector<std::wstring> &strList)
	{
		std::wstring::const_iterator		it;
		std::wstring::const_iterator		itBegin;

		strList.clear();

		itBegin = str.begin(),
			it = itBegin;

		for (; it != str.end(); it++)
		{
			if (*it == separator)
			{
				strList.push_back(std::wstring(itBegin, it));
				itBegin = it + 1;
			}
		}

		if (itBegin != it)
			strList.push_back(std::wstring(itBegin, it));
	}

	void StringSplit(const std::string &str, const std::string &separator, std::vector<std::string> &strList)
	{
		std::string::size_type			nPos;
		std::string::size_type			nFind;
		std::string						temp;

		strList.clear();

		nPos = str.find_first_not_of(separator);
		while (std::string::npos != nPos)
		{
			nFind = str.find_first_of(separator, nPos);
			if (nFind == std::string::npos)
			{
				temp = str.substr(nPos, str.size() - nPos);
				strList.push_back(temp);
				break;
			}
			else
			{
				temp = str.substr(nPos, nFind - nPos);
				strList.push_back(temp);
				nPos = str.find_first_not_of(separator, nFind);
			}
		}
	}

	void StringSplit(const std::wstring &str, const std::wstring &separator, std::vector<std::wstring> &strList)
	{
		std::wstring::size_type			nPos;
		std::wstring::size_type			nFind;
		std::wstring					temp;

		strList.clear();

		nPos = str.find_first_not_of(separator);
		while (std::wstring::npos != nPos)
		{
			nFind = str.find_first_of(separator, nPos);
			if (nFind == std::wstring::npos)
			{
				temp = str.substr(nPos, str.size() - nPos);
				strList.push_back(temp);
				break;
			}
			else
			{
				temp = str.substr(nPos, nFind - nPos);
				strList.push_back(temp);
				nPos = str.find_first_not_of(separator, nFind);
			}
		}
	}

	void Split(const std::string& s, const std::string& delim, std::vector<std::string>& ret) 
	{
		if (s.size() > 0)
		{
			size_t last = 0;
			size_t index = s.find(delim, last);
			while (index != std::string::npos)
			{
				ret.push_back(s.substr(last, index - last));
				last = index + delim.size();
				index = s.find(delim, last);
			}
			if (index - last > 0)
			{
				ret.push_back(s.substr(last, index - last));
			}
		}
	}
	
	void Split(const std::wstring& s, const std::wstring& delim, std::vector<std::wstring>& ret) 
	{
		if (s.size() > 0)
		{
			size_t last = 0;
			size_t index = s.find(delim, last);
			while (index != std::wstring::npos)
			{
				ret.push_back(s.substr(last, index - last));
				last = index + delim.size();
				index = s.find(delim, last);
			}
			if (index - last > 0)
			{
				ret.push_back(s.substr(last, index - last));
			}
		}
	}

	void StringSplit2DataVec(const std::string & str, const std::string & separator, std::vector<uint32_t>& userList)
	{
		std::string::size_type			nPos;
		std::string::size_type			nFind;
		std::string						temp;

		userList.clear();

		nPos = str.find_first_not_of(separator);
		while (std::string::npos != nPos)
		{
			nFind = str.find_first_of(separator, nPos);
			if (nFind == std::string::npos)
			{
				temp = str.substr(nPos, str.size() - nPos);
				userList.push_back(atoi(temp.c_str()));
				break;
			}
			else
			{
				temp = str.substr(nPos, nFind - nPos);
				userList.push_back(atoi(temp.c_str()));
				nPos = str.find_first_not_of(separator, nFind);
			}
		}
	}

	void PickupString(const std::string &str, const std::string &strSub, std::vector<std::string> &strList)
	{
		std::string::size_type			nPos;
		std::string::size_type			nFind;
		std::string						temp;

		strList.clear();

		nPos = str.find_first_of(strSub);
		while (std::string::npos != nPos)
		{
			nFind = str.find_first_not_of(strSub, nPos);
			if (nFind == std::string::npos)
			{
				temp = str.substr(nPos, str.size() - nPos);
				strList.push_back(temp);
				break;
			}
			else
			{
				temp = str.substr(nPos, nFind - nPos);
				strList.push_back(temp);
				nPos = str.find_first_of(strSub, nFind);
			}
		}
	}

	void PickupString(const std::wstring &str, const std::wstring &strSub, std::vector<std::wstring> &strList)
	{
		std::wstring::size_type			nPos;
		std::wstring::size_type			nFind;
		std::wstring					temp;

		strList.clear();

		nPos = str.find_first_of(strSub);
		while (std::wstring::npos != nPos)
		{
			nFind = str.find_first_not_of(strSub, nPos);
			if (nFind == std::wstring::npos)
			{
				temp = str.substr(nPos, str.size() - nPos);
				strList.push_back(temp);
				break;
			}
			else
			{
				temp = str.substr(nPos, nFind - nPos);
				strList.push_back(temp);
				nPos = str.find_first_of(strSub, nFind);
			}
		}
	}

	void ReplaceString(std::string &str, const std::string &strFind, const std::string &strReplaced)
	{
		std::string					strOut;
		std::string::size_type		nPos = 0;
		std::string::size_type		nFind = std::string::npos;
		std::string					temp;


		nFind = str.find(strFind, nPos);
		while (std::string::npos != nFind)
		{
			strOut += str.substr(nPos, nFind - nPos);
			strOut += strReplaced;

			nPos = nFind + strFind.length();
			nFind = str.find(strFind, nPos);
		}

		strOut += str.substr(nPos, str.length() - nPos);
		str.swap(strOut);
	}

	void ReplaceString(std::wstring &str, const std::wstring &strFind, const std::wstring &strReplaced)
	{
		std::wstring					strOut;
		std::wstring::size_type			nPos = 0;
		std::wstring::size_type			nFind = std::wstring::npos;
		std::wstring					temp;

		nFind = str.find(strFind, nPos);
		while (std::wstring::npos != nFind)
		{
			strOut += str.substr(nPos, nFind - nPos);
			strOut += strReplaced;

			nPos = nFind + strFind.length();
			nFind = str.find(strFind, nPos);
		}

		strOut += str.substr(nPos, str.length() - nPos);
		str.swap(strOut);
	}

	void GetSubString(const std::string &strFrom, std::string &strSub, const std::string &strHeader, const std::string &strTail)
	{
		size_t			uHeader;
		size_t			uTail;

		strSub.clear();
		if (strHeader.empty())
		{
			uHeader = 0;
		}
		else
		{
			uHeader = strFrom.find(strHeader);
			if (std::string::npos == uHeader)
				return;
			uHeader += strHeader.length();
		}

		if (strTail.empty())
		{ 
			strSub = strFrom.substr(uHeader);
		}
		else
		{
			uTail = strFrom.find(strTail, uHeader);
			if (std::string::npos == uTail)
				strSub = strFrom.substr(uHeader);
			else
				strSub = strFrom.substr(uHeader, uTail - uHeader);
		}
	}

	void GetSubString(const std::wstring &strFrom, std::wstring &strSub, const std::wstring &strHeader, const std::wstring &strTail)
	{
		size_t			uHeader;
		size_t			uTail;

		strSub.clear();
		uHeader = strFrom.find(strHeader);
		if (std::wstring::npos == uHeader)
			return;

		uHeader += strHeader.length();
		uTail = strFrom.find(strTail, uHeader);
		if (std::wstring::npos == uTail)
			strSub = strFrom.substr(uHeader);
		else
			strSub = strFrom.substr(uHeader, uTail - uHeader);
	}

	void RemoveHeader(std::string &str, char cHeader)
	{
		auto		nFind = str.find_first_not_of(cHeader);

		if ((std::string::npos != nFind) && (0 != nFind))
			str.erase(0, nFind);
	}

	void RemoveHeader(std::wstring &str, wchar_t cHeader)
	{
		auto		nFind = str.find_first_not_of(cHeader);

		if ((std::string::npos != nFind) && (0 != nFind))
			str.erase(0, nFind);
	}

	void RemoveHeader(std::string &str, const std::string &strHeader)
	{
		auto		nFind = str.find_first_not_of(strHeader);

		if ((std::string::npos != nFind) && (0 != nFind))
			str.erase(0, nFind);
	}

	void RemoveHeader(std::wstring &str, const std::wstring &strHeader)
	{
		auto		nFind = str.find_first_not_of(strHeader);

		if ((std::wstring::npos != nFind) && (0 != nFind))
			str.erase(0, nFind);
	}

	void RemoveTail(std::string &str, char cTail)
	{
		auto		nFind = str.find_last_not_of(cTail);

		if ((std::string::npos != nFind) && ((nFind + 1) != str.length()))
			str.erase(nFind + 1);
	}

	void RemoveTail(std::wstring &str, wchar_t cTail)
	{
		auto		nFind = str.find_last_not_of(cTail);

		if ((std::wstring::npos != nFind) && ((nFind + 1) != str.length()))
			str.erase(nFind + 1);
	}

	void RemoveTail(std::string &str, const std::string &strTail)
	{
		auto		nFind = str.find_last_not_of(strTail);

		if ((std::string::npos != nFind) && ((nFind + 1) != str.length()))
			str.erase(nFind + 1);
	}

	void RemoveTail(std::wstring &str, const std::wstring &strTail)
	{
		auto		nFind = str.find_last_not_of(strTail);

		if ((std::wstring::npos != nFind) && ((nFind + 1) != str.length()))
			str.erase(nFind + 1);
	}

	#define MAX_SPRINTF_SIZE (1024*1024*2) 
	std::string StringFormat(const char *fmt, ...)
	{
		std::string strResult = "";

		if (nullptr == fmt) 
			return strResult;

		va_list marker;
		va_start(marker, fmt);
		{
			int  size = 512; 
		
			while (size < MAX_SPRINTF_SIZE) 
			{ 
				strResult.resize(size); 
				va_list marker_local; 
				va_copy(marker_local, marker); 
				int n = vsnprintf((char *)strResult.data(), size, fmt, marker_local);  //FIX bug: can NOT recursive using ap (va_list type) 
				va_end(marker_local); 
		
				if (n < 0) 
				{ 
					LOG_ERROR << "err! StringFormat:" << fmt;
					strResult = ""; 
					break; 
				} 
		
				if (n < size)   // Everything worked 
				{ 
					strResult.resize(n); 
					break; 
				} 
		
				size = size* 2; 
			} 
		}
		va_end(marker);

		return strResult;
	}

	std::wstring StringFormat(const wchar_t *fmt, ...)
	{
		std::wstring	strResult = L"";

		if (nullptr == fmt) 
			return strResult;

		va_list marker;
		va_start(marker, fmt);
		size_t nLength = vwprintf(fmt, marker);
		if (nLength == 0)
		{
			//LOG_ERROR << "err! StringFormat:" << fmt;
			return strResult;
		}

		strResult.resize(nLength + 1);
		vswprintf(const_cast<wchar_t *>(strResult.data()), strResult.size(), fmt, marker);
		va_end(marker);

		return strResult.c_str();
	}

	std::wstring Ascii2Unicode(const std::string& strInput)
	{

#if defined(WIN32) || defined(WIN64)
		int nLength = ::MultiByteToWideChar(CP_ACP, 0, strInput.c_str(), -1, nullptr, 0);
		if (nLength <= 0) return std::wstring(L"");
		wchar_t *szbuffer = new wchar_t[nLength + 2];
		::MultiByteToWideChar(CP_ACP, 0, strInput.c_str(), -1, szbuffer, nLength);
		std::wstring strnew = szbuffer;
		delete[] szbuffer;
		return strnew;
#else
		setlocale(LC_CTYPE, "");
		std::size_t iWLen = mbstowcs(0, strInput.c_str(), 0);
		wchar_t* wStr = new wchar_t[iWLen + 1];
		memset(wStr, 0, sizeof(wchar_t) *(iWLen + 1));
		mbstowcs(wStr, strInput.c_str(), iWLen + 1);
		std::wstring ret(wStr);
		delete[] wStr;
		return ret;
#endif

	}


	std::string Unicode2Ascii(const std::wstring& strInput)
	{
#if defined(WIN32) || defined(WIN64)
		int nLength = ::WideCharToMultiByte(CP_OEMCP, 0, strInput.c_str(), -1, nullptr, 0, nullptr, nullptr);
		if (nLength <= 0) return std::string("");
		char *szbuffer = new char[nLength + 2];
		memset(szbuffer, 0, nLength + 2);
		int len = ::WideCharToMultiByte(CP_OEMCP, 0, strInput.c_str(), -1, szbuffer, nLength, nullptr, nullptr);
		if (len > 0) {
			szbuffer[len] = 0;
		}
		std::string strnew = szbuffer;
		delete[] szbuffer;
		return strnew;
#else
		setlocale(LC_CTYPE, "");
		std::size_t iLen = wcstombs(nullptr, strInput.c_str(), 0);
		char* str = new char[iLen + 1];
		memset(str, 0, sizeof(char) *(iLen + 1));
		wcstombs(str, strInput.c_str(), iLen + 1);
		std::string ret(str);
		delete[] str;
		return ret;
#endif
	}

	bool ICaseCompare(const std::string &str1, const std::string& str2)
	{
		if (str1.length() == str1.length())
		{
			return std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(), 
				[](char a, char b) {
					return tolower(a) == tolower(b);
				});
		}
		else 
		{
			return false;
		}
	}
	
	bool ICaseCompare(const std::wstring& str1, const std::wstring& str2)
	{
		if (str1.length() == str1.length())
		{
			return std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(),
				[](wchar_t a, wchar_t b) {
					return tolower(a) == tolower(b);
				});
		}
		else
		{
			return false;
		}
	}

	std::string Lowercase(const std::string& strInput)
	{
		std::string			strOutput = strInput;

		std::transform(strInput.begin(), strInput.end(), strOutput.begin(), [](char c) { return (char)tolower(c); });
		return strOutput;
	}

	std::wstring Lowercase(const std::wstring& strInput)
	{
		std::wstring			strOutput = strInput;

		std::transform(strInput.begin(), strInput.end(), strOutput.begin(), [](wchar_t c) { return (char)tolower(c); });
		return strOutput;
	}

	uint32_t UTF8Len(const std::string &utf8)
	{
		int32_t count = 0;

		for (auto it = utf8.begin(); it != utf8.end(); count++)
		{
			if ((*it & 0x80) == 0)
			{
				it++;
			}
			else if ((*it & 0xC0) == 0x80)
			{
				while ((*it & 0xC0) == 0x80) {
					it++;
				}
				it++;
			}
		}

		return count;
	}
}