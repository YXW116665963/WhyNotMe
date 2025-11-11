#pragma once

#include "standard.h"
#include "named_data_interface.h"
#include "data_observer_interface.h"

namespace why
{
	/**
	* @brief 定义数据中心接口
	* @details 数据中心接口为数据中心模块的入口接口，通过方法CreateDataCenter创建，当不在需要数据中心的时候，调用
	* 数据中心接口的Destroy方法释放资源。
	*/
	class IDataCenter
	{
	public:
		/**
		* @brief 注册命名数据(有符号整型)
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @param nDefault 数据的默认值
		* @param eStyle 数据的风格
		* @return 
		* - true 注册成功，或者同名数据(分类和名称都相同)已存在，并且数据类型一致
		* - flase 注册失败，注册失败的原因一般为同名数据(分类和名称都相同)已存在，并且数据类型不一致导致
		* @warning 数据默认值仅在当前管理器中不存在创建新的命名数据的时候使用。
		*/
		virtual bool RegisterIntData(const char* pDomain, const char* pName, int32_t nDefault, DataStyle eStyle) = 0;

		/**
		* @brief 注册命名数据(无符号整型)
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @param uDefault 数据的默认值
		* @param eStyle 数据的风格
		* @return
		* - true 注册成功，或者同名数据(分类和名称都相同)已存在，并且数据类型一致
		* - flase 注册失败，注册失败的原因一般为同名数据(分类和名称都相同)已存在，并且数据类型不一致导致
		* @warning 数据默认值仅在当前管理器中不存在创建新的命名数据的时候使用。
		*/
		virtual bool RegisterUIntData(const char* pDomain, const char* pName, uint32_t uDefault, DataStyle eStyle) = 0;

		/**
		* @brief 注册命名数据(有符号大整型)
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @param nDefault 数据的默认值
		* @param eStyle 数据的风格
		* @return
		* - true 注册成功，或者同名数据(分类和名称都相同)已存在，并且数据类型一致
		* - flase 注册失败，注册失败的原因一般为同名数据(分类和名称都相同)已存在，并且数据类型不一致导致
		* @warning 数据默认值仅在当前管理器中不存在创建新的命名数据的时候使用。
		*/
		virtual bool RegisterBigIntData(const char* pDomain, const char* pName, int64_t nDefault, DataStyle eStyle) = 0;

		/**
		* @brief 注册命名数据(无符号大整型)
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @param uDefault 数据的默认值
		* @param eStyle 数据的风格
		* @return
		* - true 注册成功，或者同名数据(分类和名称都相同)已存在，并且数据类型一致
		* - flase 注册失败，注册失败的原因一般为同名数据(分类和名称都相同)已存在，并且数据类型不一致导致
		* @warning 数据默认值仅在当前管理器中不存在创建新的命名数据的时候使用。
		*/
		virtual bool RegisterBigUIntData(const char* pDomain, const char* pName, uint64_t uDefault, DataStyle eStyle) = 0;

		/**
		* @brief 注册命名数据(浮点数类型)
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @param fDefault 数据的默认值
		* @param eStyle 数据的风格
		* @return
		* - true 注册成功，或者同名数据(分类和名称都相同)已存在，并且数据类型一致
		* - flase 注册失败，注册失败的原因一般为同名数据(分类和名称都相同)已存在，并且数据类型不一致导致
		* @warning 数据默认值仅在当前管理器中不存在创建新的命名数据的时候使用。
		*/
		virtual bool RegisterFloatData(const char* pDomain, const char* pName, float64_t fDefault, DataStyle eStyle) = 0;

		/**
		* @brief 注册命名数据(字符串)
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @param pDefault 数据的默认值
		* @param eStyle 数据的风格
		* @return
		* - true 注册成功，或者同名数据(分类和名称都相同)已存在，并且数据类型一致
		* - flase 注册失败，注册失败的原因一般为同名数据(分类和名称都相同)已存在，并且数据类型不一致导致
		* @warning 数据默认值仅在当前管理器中不存在创建新的命名数据的时候使用。
		*/
		virtual bool RegisterStringData(const char* pDomain, const char* pName, const char* pDefault, DataStyle eStyle) = 0;

		/**
		* @brief 注册命名数据(数组类型)
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @param eType 数组成员的类型
		* @param uSize 数组成员的数量
		* @param pDefault 数据成员的默认值(以";"为分隔符分隔多个数组成员的值)
		* @param eStyle 数据的风格
		* @return
		* - != nullptr 注册成功，或者同名数据(分类和名称都相同)已存在，并且数据类型一致。返回数量类型数据的值对象，
		* - == nullptr 注册失败，注册失败的原因一般为同名数据(分类和名称都相同)已存在，并且数据类型不一致导致
		* @warning
		* - 数据默认值仅在当前管理器中不存在创建新的命名数据的时候使用。
		* - 如果pDefault的以";"分隔的成的值的数量n大于uSize，则只有前面uSize个值有效，每个对应一个数组成员。
		* - 如果从pDefault中获取的值的数量n小于uSize，则只有前面n个成员使用初始值，其他已经变量有默认值
		*/
		virtual IListDataValue* RegisterListData(const char* pDomain, const char* pName, ValueType eType, uint32_t uSize, const char* pDefault, DataStyle eStyle) = 0;

		/**
		* @brief 注册命名数据(对象类型)
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @param eStyle 数据的风格
		* @return
		* - != nullptr 注册成功，或者同名数据(分类和名称都相同)已存在，并且数据类型一致。返回数量类型数据的值对象，
		* - == nullptr 注册失败，注册失败的原因一般为同名数据(分类和名称都相同)已存在，并且数据类型不一致导致
		* @warning 暂不提供自动初始化对象变量的支持
		*/
		virtual IObjectDataValue* RegisterObjectData(const char* pDomain, const char* pName, DataStyle eStyle) = 0;

		/**
		* @brief 查找命名数据
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @return 
		* - nullptr 命名数据不存在
		* - 其他 找到得命名数据得接口
		*/
		virtual INamedData* FindData(const char* pDomain, const char* pName) = 0;

		/**
		* @brief 查找Domain是否存在
		* @param pDomain 数据的分类名称
		* @return
		*/
		virtual bool HasDomain(const char* pDomain) = 0;

		/**
		* @brief 查找命名数据
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @return
		* - nullptr 命名数据不存在
		* - 其他 找到得命名数据得接口
		*/
		virtual IDataValue* GetDataValue(const char* pDomain, const char* pName) = 0;

		/**
		* @brief 查找命名数据
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @return
		* - nullptr 命名数据不存在
		* - 其他 找到得命名数据得接口
		*/
		virtual IListDataValue* GetListDataValue(const char* pDomain, const char* pName) = 0;

		/**
		* @brief 查找命名数据
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @return
		* - nullptr 命名数据不存在
		* - 其他 找到得命名数据得接口
		*/
		virtual IObjectDataValue* GetObjectDataValue(const char* pDomain, const char* pName) = 0;

		/**
		* @brief 注销命名数据
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @return void
		*/
		virtual void UnregisterData(const char* pDomain, const char* pName) = 0;

		/**
		* @brief 订阅数据
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @param pObserver 观察者对象的接口
		* @return void
		* @warning 
		* - 被订阅式的数据在订阅时可以还未被注册
		* - 当数据被注册的时候，如果数据有被订阅，会自动触发一次观察者回调的调用
		* - 订阅者必须保证提供的观察者对象在被取消订阅之前都是可用的。
		*/
		virtual void Subscribe(const char* pDomain, const char* pName, IDataObserver *pObserver) = 0;

		/**
		* @brief 订阅数据
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @param pMember 子元素的名称，仅对object类型的数据有效，如果为nullptr则object中任一数据发生改变都会接收到更新事件
		* @param pObserver 观察者对象的接口
		* @return void
		* @warning
		* - 被订阅式的数据在订阅时可以还未被注册
		* - 当数据被注册的时候，如果数据有被订阅，会自动触发一次观察者回调的调用
		* - 订阅者必须保证提供的观察者对象在被取消订阅之前都是可用的。
		*/
		virtual void SubscribeEx(const char* pDomain, const char* pName, const char* pMember, IDataObserver* pObserver) = 0;

		/**
		* @brief 取消对数据的订阅
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @param pObserver 观察者对象的接口
		* @return void
		*/
		virtual void Unsubscribe(const char* pDomain, const char* pName, IDataObserver* pObserver) = 0;
		
		/**
		* @brief 强制更新命名数据
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @return
		* - nullptr 命名数据不存在
		* - 其他 找到得命名数据得接口
		*/
		virtual void UpdateData(const char* pDomain, const char* pName) = 0;

		/**
		* @brief 强制更新命名数据
		* @param pDomain 数据的分类名称
		* @param pName 数据的名称
		* @param pMember 数据成员的名称
		* @return
		* - nullptr 命名数据不存在
		* - 其他 找到得命名数据得接口
		*/
		virtual void UpdateDataEx(const char* pDomain, const char* pName, const char* pMember) = 0;

		/**
		* @brief 从已有数据复制一份新的数据
		* @param pFromDomain 复制来源的数据分类
		* @param pToDomain  目标的数据分类
		* @return
		* - true 复制成功
		* - false 复制失败
		*/
		virtual bool Clone(const char* pFromDomain, const char* pToDomain) = 0;

		/**
         * @brief 销毁前置动作，防止线程阻塞
         * @return void
         */
		virtual void PrepareDestroy() = 0;

		/**
		* @brief 销毁数据管理器
		* @return void
		*/
		virtual void Destroy() = 0;
	};
}

/**
* @brief 创建数据中心对象
* @param pFileNmae 用于持久化数据存储得数据库的全路径名
* @return 数据中心对象接口
*/
why::IDataCenter* CreateDataCenter(const char* pFileName);

/**
* @brief 从数据配置文件注册数据
* @param pDataCenter 数据中心对象
* @param pDataFile 数据定义文件
* @return 
* - true 加载成功
* - false 加载失败
*/
bool RegisterDataFromFile(why::IDataCenter* pDataCenter, const char* pDataFile);

/**
* @brief 从数据配置文件Clone一份数据到新的Domain
* @param pDataCenter 数据中心对象
* @param pFromDomain 复制来源
* @param pToDomain  目标Domain
* @return
* - true 成功
* - false 失败
*/
bool CloneData(why::IDataCenter* pDataCenter, const char* pFromDomain, const char* pToDomain);

/**
* @brief 从数据配置文件注册数据到指定Domain，用于导入切割数据等
* @param pDataCenter 数据中心对象
* @param pDataFile 数据定义文件
* @param pDomain 目标Domain
* @return
* - true 加载成功
* - false 加载失败
*/
bool ImportDataFromFile(why::IDataCenter* pDataCenter, const char* pDataFile, const char* pDomain, const char* pDB = nullptr);

/**
* @brief 从数据中心导出数据到文件
* @param pDataCenter 数据中心对象
* @param pDomain 目标Domain
* @param pDataFile 数据定义文件
* @return
* - true 加载成功
* - false 加载失败
*/
bool ExportDataToFile(why::IDataCenter* pDataCenter, const char* pDomain, const char* pDataFile);