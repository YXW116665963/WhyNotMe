#pragma once

#include "standard.h"

#include <vector>
#include <string>

namespace why
{
    /**
    * @brief 将当前字符串转换成UTF-8的格式
    * @param strInput 等待转换的字符串
    * @return 返回转码后得结果
    */
    std::string UTF8Encode(const std::wstring &strInput);

    /**
    * @brief 将UTF-8的格式的字符串转换成当前格式
    * @param strInput 等待转换的字符串
    * @return 返回转码后得结果
    */
    std::wstring UTF8Decode(const std::string &strInput);

    /**
    * @brief URL解码
    * @param strInput 等待解码的字符串
    * @return 返回解码后得结果
    */
	std::string URLDecode(const std::string &src);

    /**
    * @brief URL编码
    * @param strInput 等待编码的字符串
    * @return 返回编码后得结果
    */
	std::string URLEncode(const std::string &src);

    /**
    * @brief 分割字符串
    * @param str 等待分割的字符串
    * @param separator 分割字符
    * @param strList 保存分割出的子字符串的列表
    * @return void
    */
    void StringSplit(const std::string &str, std::string::value_type separator, std::vector<std::string> &strList);
    void StringSplit(const std::wstring &str, std::wstring::value_type separator, std::vector<std::wstring> &strList);

    /**
    * @brief 分割字符串
    * @param str 等待分割的字符串
    * @param separator 分割字符集（该字符串中所包含的字符的任意组合作为分隔符）
    * @param strList 保存分割出的子字符串的列表
    * @return void
    */
    void StringSplit(const std::string &str, const std::string &separator, std::vector<std::string> &strList);
    void StringSplit(const std::wstring &str, const std::wstring &separator, std::vector<std::wstring> &strList);

	/**
	 * @brief 分割字符串
	 * @param str 等待分割的字符串
	 * @param separator 分割字符集(分割符完全匹配)
	 * @param strList 保存分割出的子字符串的列表
     * @return void
	 */
	void Split(const std::string& s, const std::string& delim, std::vector<std::string>& ret);
	void Split(const std::wstring& s, const std::wstring& delim, std::vector<std::wstring>& ret);

	/**
	* @brief 分割字符串后直接转换为相应的数据。
	* @param str 等待分割的字符串
	* @param separator 分割字符集（该字符串中所包含的字符的任意组合作为分隔符）
	* @param userList 保存分割出的子字符串的列表
    * @return void
	*/
	void StringSplit2DataVec(const std::string &str, const std::string &separator, std::vector<uint32_t> &userList);

    /**
    * @brief 获取从字符串获取由限定字符构成的子字符串
    * @param str 等待拾取的字符串
    * @param strSub 构成子字符串的字符集
    * @param strList 保存获取的子字符串的列表
    * @return void
    */
    void PickupString(const std::string &str, const std::string &strSub, std::vector<std::string> &strList);
    void PickupString(const std::wstring &str, const std::wstring &strSub, std::vector<std::wstring> &strList);

    /**
    * @brief 将字符串中指定的子串替换为新的子串
    * @param str 等待查找替换的字符串
    * @param strFind 被替代的字串
    * @param strReplaced 用于替换的新的子串
    * @return void
    */
    void ReplaceString(std::string &str, const std::string &strFind, const std::string &strReplaced);
    void ReplaceString(std::wstring &str, const std::wstring &strFind, const std::wstring &strReplaced);

    /**
    * @brief 从字符串中获取指定的子串
    * @param strFrom 等待查找字符串
    * @param strSub 被替代的字串
    * @param strHeader 子串的开头字符（保存到strSub中的子串不包含这个开头字符)
    * @param strTail 子串的结尾字符（保存到strSub中的子串不包含这个结尾字符)
    * @return void
    */
    void GetSubString(const std::string &str, std::string &strSub, const std::string &strHeader, const std::string &strTail);
    void GetSubString(const std::wstring &str, std::wstring &strSub, const std::wstring &strHeader, const std::wstring &strTail);

    /**
    * @brief 移除字符串头部所有指定字符
    * @param strFrom 等待查找字符串
	* @param cHeader 待移除的字符
    * @param strHeader 待移除的字符
    * @return void
    */
	void RemoveHeader(std::string &str, char cHeader);
	void RemoveHeader(std::wstring &str, wchar_t cHeader);
    void RemoveHeader(std::string &str, const std::string &strHeader);
    void RemoveHeader(std::wstring &str, const std::wstring &strHeader);

    /**
    * @brief 移除字符串尾部所有指定字符
    * @param strFrom 等待查找字符串
	* @param cTail 待移除的字符
    * @param strTail 待移除的字符
    * @return void
    */
	void RemoveTail(std::string &str, char cTail);
	void RemoveTail(std::wstring &str, wchar_t cTail);
    void RemoveTail(std::string &str, const std::string &strTail);
    void RemoveTail(std::wstring &str, const std::wstring &strTail);

    /**
    * @brief 提供一个格式化到输出到文本的方法
    * @return 格式化输出的结果
    */
    std::string StringFormat(const char *fmt, ...);
    std::wstring StringFormat(const wchar_t *fmt, ...);

    /**
    * @brief 将当前字符串转换成unicode-16的格式
    * @param strInput 等待转换的字符串
    * @return 转换后得结果
    */
	std::wstring Ascii2Unicode(const std::string& strInput);

    /**
    * @brief 将当前字符串转换成Ascii的格式
    * @param strInput 等待转换的字符串
    * @return 转换后得结果
    */
	std::string Unicode2Ascii(const std::wstring& strInput);

    /**
    * @brief 判断两个字符串得内容是否一致（忽略大小写得差异）
    * @param str1 待比较得字符串1
    * @param str2 待比较得字符串2
    * @return
    * @ - true 内容一致
    * @ - false 内容不一致
    */
    bool ICaseCompare(const std::string& str1, const std::string& str2);
    bool ICaseCompare(const std::wstring& str1, const std::wstring& str2);

    /**
    * @brief 将字符串转为全小写的字符串
    * @param strInput 待转换的字符串
    * @return 转换后的字符串
    */
    std::string Lowercase(const std::string& strInput);
    std::wstring Lowercase(const std::wstring& strInput);

	uint32_t UTF8Len(const std::string &utf8);
}
