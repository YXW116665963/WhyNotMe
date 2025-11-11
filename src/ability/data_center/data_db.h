#pragma once

#include "data_center_def.h"
#include "named_data_interface.h"

#include "sqlite3.h"

namespace why
{
	/**
	*@class DataDB
	*@brief 封装命名数据数据库操作
	*/
	class DataDB
	{
	public:
		DataDB();
		~DataDB();

		/**
		* @brief 开启配置数据库
		* @param strFileName 配置数据库的文件名
		* @return
		* - true 启动成功
		* - false 启动失败
		*/
		bool Startup(const std::string& strFileName);

		/**
		* @brief 关闭配置数据库
		* @return void
		*/
		void Shutdown();

		/**
		* @brief 数据库是否已经被正确初始化
		* @return
		* - true 已经初始化
		* - false 未初始化
		*/
		bool IsStartup();

		/**
		* @brief 数据库中添加一个命名数据
		* @param pData 命名数据接口
		* @return
		* - true 添加成功
		* - false 添加失败
		* @warning 注意此处仅仅是添加一个数据记录（及包含分类、名称、类型，不保存数据得值）
		*/
		bool InsertData(INamedData* pData);

		/**
		* @brief 从数据库加载命名数据得值
		* @param pData 命名数据接口
		* @return
		* - true 加载成功
		* - false 加载失败
		*/
		bool ReadData(INamedData* pData);

		/**
		* @brief 命名数据的值
		* @param pData 命名数据接口
		* @return void
		*/
		void WriteData(INamedData* pData);

	private:
		/**
		* @brief 装载命名数据
		* @param pData 命名数据的接口
		* @param pStmt SQLite3 数据的选择集游标
		* @return void
		*/
		void LoadData(INamedData* pData, sqlite3_stmt* pStmt);

		/**
		* @brief 装载命名数据(整形)
		* @param pData 命名数据的接口
		* @param eType 数据的类型
		* @param pStmt SQLite3 数据的选择集游标
		* @return void
		*/
		void LoadIntData(INamedData* pData, ValueType eType, sqlite3_stmt* pStmt);

		/**
		* @brief 装载命名数据(浮点型)
		* @param pData 命名数据的接口
		* @param pStmt SQLite3 数据的选择集游标
		* @return void
		*/
		void LoadFloatData(INamedData* pData, sqlite3_stmt* pStmt);

		/**
		* @brief 装载命名数据(字符串型)
		* @param pData 命名数据的接口
		* @param pStmt SQLite3 数据的选择集游标
		* @return void
		*/
		void LoadStringData(INamedData* pData, sqlite3_stmt* pStmt);

		/**
		* @brief 装载命名数据(对象类型)
		* @param pData 命名数据的接口
		* @param pStmt SQLite3 数据的选择集游标
		* @return void
		*/
		void LoadObjectData(INamedData* pData, sqlite3_stmt* pStmt);

		/**
		* @brief 装载命名数据(数组型)
		* @param pData 命名数据的接口
		* @param eType 数据的类型
		* @param pStmt SQLite3 数据的选择集游标
		* @return void
		*/
		void LoadArrayData(INamedData* pData, ValueType eType, uint32_t uSize, sqlite3_stmt* pStmt);

		/**
		* @brief 更新命名数据的值（整数）
		* @param strDomain 数据分类名称
		* @param strName 数据名称
		* @param uData 命名数据的值
		* @return void
		*/
		void UpdateData(const std::string& strDomain, const std::string& strName, uint64_t uData);

		/**
		* @brief 更新命名数据的值（浮点数）
		* @param strDomain 数据分类名称
		* @param strName 数据名称
		* @param fData 命名数据的值
		* @return void
		*/
		void UpdateData(const std::string& strDomain, const std::string& strName, float64_t fData);

		/**
		* @brief 更新命名数据的值（字符串）
		* @param strDomain 数据分类名称
		* @param strName 数据名称
		* @param strText 命名数据的值
		* @return void
		*/
		void UpdateData(const std::string& strDomain, const std::string& strName, const std::string& strText);

		/**
		* @brief 更新命名数据的值（二进制流）
		* @param strDomain 数据分类名称
		* @param strName 数据名称
		* @param lpFormat 数据格式化文本
		* @param uType 命名数据的值的类型
		* @param pData 命名数据的值的首地址
		* @param pData 缓冲区中有效数据的长度（单位：字节）
		* @return void
		*/
		void UpdateData(const std::string& strDomain, const std::string& strName, const int8_t* pData, uint32_t uLen);

		/**
		* @brief 查找指定命名数据
		* @param strDomain 数据分类名称
		* @param strName 数据名称
		* @return
		* - true 命名数据已经存在
		* - false 命名数据不存在
		*/
		bool Find(const std::string& strDomain, const std::string& strName);

		/**
		* @brief 获取命名数据得值得类型
		* @param pData 命名数据得接口
		* @return 值得类型
		* @warning 对于非数组类型，值得类型为命名数据得类型，对于数组类型，值得类型为数组成员得类型
		* 取值范围为ValueType枚举，此处返回整型，是为了方便写入数据库
		*/
		int32_t GetNameDataType(INamedData* pData);

		/**
		* @brief 获取命名数据得值得数量
		* @param pData 命名数据得接口
		* @return 值得数量
		* @warning 对于非数组类型，值为1，对于数组类型，值得类型为数组成员得数量
		*/
		uint32_t GetNameDataSize(INamedData* pData);

	public:
		sqlite3* m_db;				//!< 保存命名数据的数据库对象	
	};
}