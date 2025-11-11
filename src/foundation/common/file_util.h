#pragma once

#include "standard.h"
#include <list>
#include <vector>
#include <string>
#include <map>
#include <filesystem>

namespace why
{
	/**
	* @brief 从UTF8编码的文件名获取文件路径对象
	* @param strUTF8 文件的全路径名(UTF-8编码)
	* @return 返回路径对象
	*/
	std::filesystem::path UTF8ToPath(const std::string& strUTF8);

	/**
	* @brief 从文件对象获取UTF-8编码的文件名称
	* @param strUTF8 文件的全路径名(UTF-8编码)
	* @return 返回路径对象
	*/
	std::string PathToUTF8(const std::filesystem::path& filePath);

	/**
	* @brief 将UTF-8编码的文件名称转变为本地编码的文件名
	* @param strUTF8 文件的全路径名(UTF-8编码)
	* @return 返回本地编码的文件名称
	*/
	std::string UTF8ToLocal(const std::string& strUTF8);

	/**
	* @brief 将本地编码的文件名称转变为UTF-8编码的文件名称
	* @param strLocal 本地编码的文件名称
	* @return 返回文件的全路径名(UTF-8编码)
	*/
	std::string LocalToUTF8(const std::string& strLocal);

    /**
    * @brief 创建目录
    * @param dir 待创建得目录名称
    * @return
    * @ - 0 创建成功
    * @ - 其他 创建失败
    * @warning
    * 在windows平台上，使用GB2312编码
    * 在非windwos平台上，使用UTF-8编码
    */
    bool CreateDirectories(const std::string& dir);

    /**
    * @brief 指定得路径是否为一个目录
    * @param dir 待检测得路径
    * @return
    * @ - true 是目录
    * @ - false 非目录
    * @warning
    * 在windows平台上，使用GB2312编码
    * 在非windwos平台上，使用UTF-8编码
    */
    bool IsFolder(const std::string& dir);

	/**
	* @brief 是否时文件
	* @param filePath 文件绝对路径
	* @return
	* @ - true 是文件
	* @ - false 非文件
	* @warning
	* 在windows平台上，使用GB2312编码
	* 在非windwos平台上，使用UTF-8编码
	*/
	bool IsFile(const std::string& filePath);

    /**
    * @brief 加载文本文件(*.txt)
    * @param strFileName 待加载得文件名称
    * @parame lineList 文本文件得内容，向量得每一个成员为文件得一行内容
    * @return
    * @ - true 加载成功
    * @ - false 加载失败
    */
    bool LoadTextFile(const std::string &strFileName, std::vector<std::string>& lineList);
	bool SaveTextTile(const std::string& strFileName, const std::vector<std::string>& lineList);

	//<! 获取所有的行，包含空行
	bool LoadTextFileAllLine(const std::string& strFileName, std::vector<std::string>& lineList);

	/**
	* @brief 加载文本文件(*.txt)，若对原始数据修改用list
	* @param strFileName 待加载得文件名称
	* @parame lineList 文本文件得内容，向量得每一个成员为文件得一行内容
	* @return
	* @ - true 加载成功
	* @ - false 加载失败
	*/
	bool LoadTextFile(const std::string &strFileName, std::list<std::string>& lineList);
	bool SaveTextTile(const std::string& strFileName, const std::list<std::string>& lineList);

    /**
    * @brief 加载逗号文件(*.csv)
    * @param strFileName 待加载得文件名称
    * @param dataList 逗号文件得内容，第一个向量表示一行得信息，第二个向量标识一行中以逗号分割得内容
    * @return
    * @ - true 加载成功
    * @ - false 加载失败
    */
    bool LoadCSVFile(const std::string &strFileName, std::vector<std::vector<std::string>>& dataList);
	
	/**
   * @brief 写入csv文件
   * @param dataList 逗号文件得内容，第一个向量表示一行得信息，第二个向量标识一行中以逗号分割得内容
   * @param strFileName 待加载得文件名称
   * @return
   * @ - true 加载成功
   * @ - false 加载失败
   */
	bool WriteCSVFile(std::vector<std::vector<std::string>>& dataList, const std::string& strFileName);
	bool WriteCSVFile(std::list<std::vector<std::string>>& dataList, const std::string& strFileName);
	bool WriteCSVFile(std::list<std::string>& dataList, const std::string& strFileName);

	/**
   * @brief 获取目录中所有子文件夹，由于是递归函数，请确保传入的subs是空的
   * @param dir 绝对路径
   * @param subs 子文件夹路径，map key：父文件夹路径（目录下无子文件夹则不在key中），list表示子文件夹名称和绝对路径
   * @return
   * @ - true 成功
   * @ - false 失败
   */
	bool GetSubFolders(const std::string &dir, std::map<std::string, std::list<std::pair<std::string, std::string>>> &subs);

	/**
   * @brief 只获取一层子文件夹
   * @param dir 绝对路径
   * @param subs 子文件夹路径，std::pair first名称 second绝对路径
   * @return
   * @ - true 成功
   * @ - false 失败
   */
	bool GetOneSubFolders(const std::string &dir, std::list<std::pair<std::string, std::string>> &subs);

	/**
   * @brief 只获取一层文件
   * @param dir 绝对路径
   * @param ext 后缀，输入""或者".*"表示所有文件，（例子.csv）
   * @param subs 子文件路径，std::pair first名称 second绝对路径
   * @return
   * @ - true 成功
   * @ - false 失败
   */
	bool GetOneSubFiles(const std::string &dir, const std::string &ext, std::list<std::pair<std::string, std::string>> &subs);

	/**
	* @brief 获取到文件名或最后文件夹名(包括后缀)
	* @param dir 绝对路径
	* @return 文件名或最后文件夹名
	* @note 
	* - 输入"C:\\path\\last\\file.name" 输出  "file.name"
	* - 输入"C:\\path\\last" 输出 "last"
	*/
	std::string GetFolderName(const std::string &dir);

	/**
	* @brief 获取到文件名(不包括后缀)
	* @param path 绝对路径
	* @return 文件名
	* @note
	* - 输入"C:\\path\\last\\file.name" 输出  "file"
	* - 输入"C:\\path\\last" 输出 "last"
	*/
	std::string GetFileName(const std::string &path);

	/**
	* @brief 获取到上一层绝对路径
	* @param path 绝对路径
	* @return 文件名
	* @note
	* - 输入"C:\\path\\last\\file.name" 输出  "C:\\path\\last"
	* - 输入"C:\\path\\last" 输出 "C:\\path"
	*/
	std::string GetDirectory(const std::string &path);

	/**
	* @brief 获取到无后缀名的出对路径
	* @param path 绝对路径
	* @return 文件名
	* @note
	* - 输入"C:\\path\\last\\file.name" 输出  "C:\\path\\last\\file"
	* - 输入"C:\\path\\last" 输出 "C:\\path\\last"
	*/
	std::string GetFileNameWithoutExtension(const std::string &path);

	int32_t DeleteFileEx(const std::wstring& path);

	bool CopyFile(const std::string &src, const std::string &dst);

	bool IsExistFileInDir(const std::string &dir, const std::string &name);
}
