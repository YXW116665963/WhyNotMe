#pragma once

#include "standard.h"
#include "data_center_def.h"

namespace why
{
	/**
	* @brief 定义命名数据的通用值类型
	*/
	class IDataValue
	{
	public:
		/**
		* @brief 获取值的类型
		* @return 返回值的类型
		*/
		virtual ValueType Type() const = 0;

		/**
		* @brief 获取整型值(有符号数)
		* @return 返回整型的值
		* @warning 对于非整型类型，该函数返回值为0
		*/
		virtual int32_t GetIntValue() const = 0;

		/**
		* @brief 设置数据的值为整型值(有符号数)
		* @param nValue 待设置的新的整型值(有符号数)
		* @return
		* - true 设置成功
		* - flase 设置失败
		*@warning 对于非整型类型，调用该函数返回值为false
		*/
		virtual bool SetIntValue(int32_t nValue) = 0;

		/**
		* @brief 获取整型值(无符号数)
		* @return 返回整型的值
		* @warning 对于非整型类型，该函数返回值为0
		*/
		virtual uint32_t GetUIntValue() const = 0;

		/**
		* @brief 设置数据的值为整型值(无符号数)
		* @param uValue 待设置的新的整型值(无符号数)
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning 对于非整型类型，调用该函数返回值为false
		*/
		virtual bool SetUIntValue(uint32_t uValue) = 0;

		/**
		* @brief 获取大整型值(有符号数)
		* @return 返回大整型的值
		* @warning 对于非大整型类型，该函数返回值为0
		*/
		virtual int64_t GetBigIntValue() const = 0;

		/**
		* @brief 设置数据的值为大整型值(有符号数)
		* @param nValue 待设置的新的大整型值(有符号数)
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning 对于非大整型类型，调用该函数返回值为false
		*/
		virtual bool SetBigIntValue(int64_t nValue) = 0;

		/**
		* @brief 获取大整型值(无符号数)
		* @return 返回大整型的值
		* @warning 对于非大整型类型，该函数返回值为0
		*/
		virtual uint64_t GetBigUIntValue() const = 0;

		/**
		* @brief	设置数据的值为大整型值(无符号数)
		* @param uValue 待设置的新的大整型值(无符号数)
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning 对于非大整型类型，调用该函数返回值为false
		*/
		virtual bool SetBigUIntValue(uint64_t uValue) = 0;

		/**
		* @brief 获取浮点值
		* @return 返回浮点型的值
		* @warning 对于非浮点类型，该函数返回值为0.0
		*/
		virtual float64_t GetFloatValue() const = 0;

		/**
		* @brief 设置数据的值为浮点型值
		* @param fValue 待设置的新的大浮点值
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning 对于非浮点类型，调用该函数返回值为false
		*/
		virtual bool SetFloatValue(float64_t fValue) = 0;

		/**
		* @brief 获取字符串值
		* @param pText 用来保存获取得字符串得缓冲区
		* @param uTextSize 当pText不为nullptr得时候，标识pText得大小
		* @return 返回字符串值实际长度(含字符串结束符)
		* @warning 下列场景不修改缓冲区pText得内容
		* - 对于非字符串类型，该函数返回值为0
		* - 当提供得uTextSize 小于字符串得实际长度，函数返回实际需要得缓冲区长度。
		*/
		virtual uint32_t GetStringValue(char* pText, uint32_t uTextSize) const = 0;

		/**
		* @brief 设置数据的值为字符串
		* @param pValue 待设置的新的字符串值的首地址
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning 对于非字符类型，调用该函数返回值为false
		*/
		virtual bool SetStringValue(const char* pValue) = 0;

		/**
		* @brief 使用另外一个数据值来给当前数据赋值
		* @param pOther 数据值的来源
		* @return
		* - true 赋值成功
		* - false 赋值失败
		*/
		virtual bool Assign(IDataValue* pOther, bool& changed) = 0;
	};

	/**
	* @brief 定义数组类型的值
	* @warning 数组类型的值中的每一个元素的类型必须保持一致
	*/
	class IListDataValue : public IDataValue
	{
	public:
		/**
		* @brief 获取数组值的类型
		* @return 返回值的类型
		*/
		virtual ValueType ChildType() const = 0;

		/**
		* @brief 获取数组值的成员数量
		* @return 成员数量
		*/
		virtual uint32_t Size() const = 0;

		/**
		* @brief 重新设置数组的大小
		* @param 数组新的大小
		* @return void
		* @warning 如果新的数组大小小于之前的，则数组后面的数据将被丢弃；如果新的数组大小大于之前的，则数组后面会自动新添加新的元素
		*/
		virtual void Resize(uint32_t uSize) = 0;

		/**
		* @brief 获取数组成员的值
		* @param uIndex 成员在数组中的位置，取值范围[0, Size())
		* @return 成员的值对象指针，如果uIndex值越界，返回nullptr
		*/
		virtual IDataValue* GetAt(uint32_t uIndex) = 0;
		virtual const IDataValue* GetAt(uint32_t uIndex) const = 0;

		/**
		* @brief 获取数组成员的值(有符号整数)
		* @param uIndex 成员在数组中的位置，取值范围[0, Size())
		* @return 成员的值
		* @warning 数组成员为非有符号整型类型，该函数返回值为0
		*/
		virtual int32_t GetAtAsInt(uint32_t uIndex) const = 0;

		/**
		* @brief 获取数组成员的值(无符号整数)
		* @param uIndex 成员在数组中的位置，取值范围[0, Size())
		* @return 成员的值
		* @warning 数组成员为非无符号整型类型，该函数返回值为0
		*/
		virtual uint32_t GetAtAsUInt(uint32_t uIndex) const = 0;

		/**
		* @brief 获取数组成员的值(有符号大整数)
		* @param uIndex 成员在数组中的位置，取值范围[0, Size())
		* @return 成员的值
		* @warning 数组成员为非有符号大整型类型，该函数返回值为0
		*/
		virtual int64_t GetAtAsBigInt(uint32_t uIndex) const = 0;

		/**
		* @brief 获取数组成员的值(无符号大整数)
		* @param uIndex 成员在数组中的位置，取值范围[0, Size())
		* @return 成员的值
		* @warning 数组成员为非无符号大整型类型，该函数返回值为0
		*/
		virtual uint64_t GetAtAsBigUInt(uint32_t uIndex) const = 0;

		/**
		* @brief 获取数组成员的值(浮点数)
		* @param uIndex 成员在数组中的位置，取值范围[0, Size())
		* @return 成员的值
		* @warning 数组成员为非浮点数类型，该函数返回值为0.0
		*/
		virtual float64_t GetAtAsFloat(uint32_t uIndex) const = 0;

		/**
		* @brief 获取数组成员的值(字符串)
		* @param uIndex 成员在数组中的位置，取值范围[0, Size())
		* @param pText 用来保存获取得字符串得缓冲区
		* @param uTextSize 当pText不为nullptr得时候，标识pText得大小
		* @return 返回字符串值实际长度(含字符串结束符)
		* @warning 下列场景不修改缓冲区pText得内容
		* - 数组成员为非字符串类型，该函数返回值为0
		* - 当提供得uTextSize 小于字符串得实际长度，函数返回实际需要得缓冲区长度。
		*/
		virtual uint32_t GetAtAsString(uint32_t uIndex, char* pText, uint32_t uTextSize) const = 0;

		/**
		* @brief 设置数组成员的值(有符号整数)
		* @param uIndex 成员在数组中的位置，取值范围[0, Size())
		* @param nValue 成员的值
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning 对于位置越界或成员类型非有符号整数类型，调用该函数返回值为false
		*/
		virtual bool SetAtAsInt(uint32_t uIndex, int32_t nValue) = 0;

		/**
		* @brief 获取数组成员的值(无符号整数)
		* @param uIndex 成员在数组中的位置，取值范围[0, Size())
		* @param uValue 成员的值
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning 对于位置越界或成员类型非无符号整数类型，调用该函数返回值为false
		*/
		virtual bool SetAtAsUInt(uint32_t uIndex, uint32_t uValue) = 0;

		/**
		* @brief 设置数组成员的值(有符号大整数)
		* @param uIndex 成员在数组中的位置，取值范围[0, Size())
		* @param nValue 成员的值
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning 对于位置越界或成员类型非有符号大整数类型，调用该函数返回值为false
		*/
		virtual bool SetAtAsBigInt(uint32_t uIndex, int64_t nValue) = 0;

		/**
		* @brief 获取数组成员的值(无符号大整数)
		* @param uIndex 成员在数组中的位置，取值范围[0, Size())
		* @param uValue 成员的值
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning 对于位置越界或成员类型非无符号大整数类型，调用该函数返回值为false
		*/
		virtual bool SetAtAsBigUInt(uint32_t uIndex, uint64_t uValue) = 0;

		/**
		* @brief 设置数组成员的值(浮点数)
		* @param uIndex 成员在数组中的位置，取值范围[0, Size())
		* @param fValue 成员的值
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning 对于位置越界或成员类型非浮点数类型，调用该函数返回值为false
		*/
		virtual bool SetAtAsFloat(uint32_t uIndex, float64_t fValue) = 0;

		/**
		* @brief 获取数组成员的值(字符串)
		* @param uIndex 成员在数组中的位置，取值范围[0, Size())
		* @param pValue 成员的值
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning 对于位置越界或成员类型非字符串类型，调用该函数返回值为false
		*/
		virtual bool SetAtAsString(uint32_t uIndex, const char* pValue) = 0;

		/**
		* @brief 一次性设置值
		*/
		virtual bool SetValue(ValueType eType, const std::string& strValue, bool unknowSize = true) = 0;
	};

	/**
	* @brief 定义对象类型的值
	* @details 所谓对象类型，保存的数据为key、value对，其中key为字符串类型，value为基本类型
	* value暂不支持数组与对象类型。
	*/
	class IObjectDataValue : public IDataValue
	{
	public:
		/**
		* @brief 获取当前所有子元素的数量
		* @return 子元素的数量
		* @warning 主要使用在不确认对象数据类型含有的子类型名称是使用，该方法结合下面的GetAt方法能枚举所有子元素
		*/
		virtual uint32_t Size() const = 0;

		/**
		* @brief 获取子元素的名称
		* @param uIndex 子元素在对象中的位置，取值范围[0, Size())
		* @return 子元素名称
		*/
		virtual const char* GetName(uint32_t uIndex) = 0;

		/**
		* @brief 获取子元素的值
		* @param uIndex 子元素在对象中的位置，取值范围[0, Size())
		* @return 子元素值对象指针，如果uIndex值越界，返回nullptr
		*/
		virtual IDataValue* GetAt(uint32_t uIndex) = 0;
		virtual const IDataValue* GetAt(uint32_t uIndex) const = 0;

		/**
		* @brief 获取子元素的值
		* @param pName 子元素的名称
		* @return 子元素的值对象指针，如果子元素不存在，返回nullptr
		*/
		virtual IDataValue* Get(const char* pName) = 0;
		virtual const IDataValue* Get(const char* pName) const = 0;

		/**
		* @brief 获取子元素的值(有符号整数)
		* @param pName 子元素的名称
		* @return 子元素的值
		* @warning 子元素不存在或子元素值类型为非有符号整型类型，该函数返回值为0
		*/
		virtual int32_t GetAsInt(const char* pName) const = 0;

		/**
		* @brief 获取子元素的值(无符号整数)
		* @param pName 子元素的名称
		* @return 子元素的值
		* @warning 子元素不存在或子元素值类型为非无符号整型类型，该函数返回值为0
		*/
		virtual uint32_t GetAsUInt(const char* pName) const = 0;

		/**
		* @brief 获取子元素的值(有符号大整数)
		* @param pName 子元素的名称
		* @return 子元素的值
		* @warning 子元素不存在或子元素值类型为非有符号大整型类型，该函数返回值为0
		*/
		virtual int64_t GetAsBigInt(const char* pName) const = 0;

		/**
		* @brief 获取子元素的值(无符号大整数)
		* @param pName 子元素的名称
		* @return 子元素的值
		* @warning 子元素不存在或子元素值类型为非无符号大整型类型，该函数返回值为0
		*/
		virtual uint64_t GetAsBigUInt(const char* pName) const = 0;

		/**
		* @brief 获取子元素的值(浮点数)
		* @param pName 子元素的名称
		* @return 子元素的值
		* @warning 子元素不存在或子元素值类型为非浮点类型，该函数返回值为0.0
		*/
		virtual float64_t GetAsFloat(const char* pName) const = 0;

		/**
		* @brief 获取子元素的值(字符串
		* @param pName 子元素的名称
		* @param pText 用来保存获取得字符串得缓冲区
		* @param uTextSize 当pText不为nullptr得时候，标识pText得大小
		* @return 返回字符串值实际长度(含字符串结束符)
		* @warning 下列场景不修改缓冲区pText得内容
		* - 子元素不存在或子元素值类型为非字符串类型，该函数返回值为0
		* - 当提供得uTextSize 小于字符串得实际长度，函数返回实际需要得缓冲区长度。
		*/
		virtual uint32_t GetAsString(const char* pName, char* pText, uint32_t uTextSize) const = 0;

		/**
		* @brief 设置子元素的值(有符号整数)
		* @param pName 子元素的名称
		* @param nValue 成员的值
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning
		* - 子元素不存在，则自动添加一个新的子元素
		* - 已经存在的子元素的类型非有符号整数类型，调用该函数返回值为false
		*/
		virtual bool SetAsInt(const char* pName, int32_t nValue) = 0;

		/**
		* @brief 设置子元素的值(无符号整数)
		* @param pName 子元素的名称
		* @param uValue 成员的值
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning
		* - 子元素不存在，则自动添加一个新的子元素
		* - 已经存在的子元素的类型非无符号整数类型，调用该函数返回值为false
		*/
		virtual bool SetAsUInt(const char* pName, uint32_t uValue) = 0;

		/**
		* @brief 设置子元素的值(有符号大整数)
		* @param pName 子元素的名称
		* @param nValue 成员的值
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning
		* - 子元素不存在，则自动添加一个新的子元素
		* - 已经存在的子元素的类型非有符号大整数类型，调用该函数返回值为false
		*/
		virtual bool SetAsBigInt(const char* pName, int64_t nValue) = 0;

		/**
		* @brief 设置子元素的值(无符号大整数)
		* @param pName 子元素的名称
		* @param uValue 成员的值
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning
		* - 子元素不存在，则自动添加一个新的子元素
		* - 已经存在的子元素的类型非无符号大整数类型，调用该函数返回值为false
		*/
		virtual bool SetAsBigUInt(const char* pName, uint64_t uValue) = 0;

		/**
		* @brief 设置子元素的值(浮点数)
		* @param pName 子元素的名称
		* @param fValue 成员的值
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning
		* - 子元素不存在，则自动添加一个新的子元素
		* - 已经存在的子元素的类型非浮点类型，调用该函数返回值为false
		*/
		virtual bool SetAsFloat(const char* pName, float64_t fValue) = 0;

		/**
		* @brief 设置子元素的值(字符串)
		* @param pName 子元素的名称
		* @param pValue 成员的值
		* @return
		* - true 设置成功
		* - flase 设置失败
		* @warning
		* - 子元素不存在，则自动添加一个新的子元素
		* - 已经存在的子元素的类型非字符串类型，调用该函数返回值为false
		*/
		virtual bool SetAsString(const char* pName, const char* pValue) = 0;

		virtual bool SetListFloatValue(const char* pName, const char* pValue) = 0;
		virtual bool SetListFloatOneValue(const char* pName, uint32_t uIndex, float64_t fValue) = 0;
		virtual float64_t GetFloatOneValue(const char* pName, uint32_t uIndex) = 0;
	};
}