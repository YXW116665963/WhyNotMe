#pragma once

#include "standard.h"

#include <vector>
#include <string>
#include<sstream>

namespace why
{
	/**
	* @brief 调整字节顺序
	* @param data 调整前的字节顺序
	* @return 调整后的字节顺序
	*/
	uint16_t ChangeByteOrder(uint16_t data);
	uint32_t ChangeByteOrder(uint32_t data);

	/**
	* @brief 获取当前所在的时区
	* @return 当前时区
	*/
	int32_t GetIZone32();

	/**
	* @brief 获取当前时间戳（单位：秒）
	* @return 当前时间戳
	*/
	uint32_t GetTimeStamp32();

	/**
	* @brief 获取当前时间戳（单位：毫秒）
	* @return 当前时间戳
	*/
	uint64_t GetTimeStamp64();

	/**
	* @brief 获取当当前天开始时的时间戳（单位：秒）
	* @return 当前天开始时的时间戳
	*/
	uint32_t GetStartTime();

	/**
	* @brief 获取当当前天结束时的时间戳（单位：秒）
	* @return 当前天结束时的时间戳
	*/
	uint32_t GetEndTime();

	/**
	* @brief 按倍数对齐数据
	* @param 待对齐的数据
	* @param 待对齐的倍数
	* @return 对齐后的数据
	* @example count为12， based为5，对齐后的数据为15
	*/
	uint32_t ByteAligned(uint32_t count, uint32_t based);

	/**
	* @brief 将无符号数转换为16进制表示的字符串
	* @param data 无符号数
	* @return 16进制表示的字符串
	*/
	template <class T>
	inline std::string ToHex(T data)
	{
		std::stringstream		formatStream;

		formatStream << std::hex << data;
		return formatStream.str();
	}

	/**
	* @brief 将二进制数据字节流转为16进制表示的字符串
	* @param pBuf 二进制数据字节流的首地址
	* @param uBufLen 缓冲区pBuf的长度
	* @param buf 二进制数据字节流缓冲区
	* @return 16进制表示的字符串
	*/
	std::string ToHex(const uint8_t* pBuf, uint32_t uBufLen);
	std::string ToHex(const std::vector<uint8_t> &buf);

	/**
	* @brief 生成一个和时间相关的名称
	* @param pPrefix 名称的前缀
	* @param pSuffix 名称的后缀
	* @return 生成的名称
	*/
	std::string GenerateNameByTime(const char *pPrefix, const char *pSuffix);

	/**
	* @brief 获取当日期和时间的字符串
	* @return 当前日期和时间的字符串
	*/
	std::string GetDataAndTime();

	/**
	* @brief 通用的将数组序列化到字符串的方法
	* @param out 序列化输出字符串使用的输出流对象
	* @param vectorValue 等待序列化的数组
	* @return out 包含序列化后内容的输出流对象
	* @warning 如果T是自定义类型，这该类型必须有对操作符"<<"重载
	*/
	template <class T>
	inline std::ostream& operator << (std::ostream& out, const std::vector<T>& vectorValue)
	{
		out << "[";
		for (auto value : vectorValue)
			out << value << ", ";
		out << "-- " << (uint32_t)vectorValue.size() << " --]";
		return out;
	}

	/**
	* @brief 通用的接口删除仿函数
	* @param DerivedT 需要释放的派生类
	* @param BasedT 需要释放派生类的基类
	*/
	template <class DerivedT, class BasedT>
	class InterfaceDeleter
	{
	public:
		void operator()(BasedT* pBasedObject)
		{
			DerivedT* pDerivedObject = dynamic_cast<DerivedT *>(pBasedObject);

			if (pDerivedObject)
				delete pDerivedObject;
		}
	};

	/**
	* @brief 获取指定范围的随机数
	* @nMin 随机数的最小值
	* @nMax 随机数的最大值
	* @return 返回获取到的随机数
	* @warning 
	* - 整数为闭区间[nMin, nMax]
	* - 浮点数为半闭区间[fMin, fMax)
	*/
	int32_t GenerateRandomNumber(int32_t nMin, int32_t nMax);
	float64_t GenerateRandomFloat64(float64_t fMin, float64_t fMax);
	/**
	* @brief 格式化数据的值
	* @param nMin 最小值
	* @param nMax 最大值
	* @param nInc 数据的对齐值，即数据必须能被nInc整除
	* @param nValue 数据的当前值
	* @return 格式化后的数据
	*/
	int32_t FormatValue(int32_t nMin, int32_t nMax, int32_t nInc, int32_t nValue);
	
	/**
	* @brief 比较浮点数相等
	* @a 值a
	* @b 值b
	* @epsilon 最小值的精度
	* @return bool
	*/
	bool AreEqual(const float64_t& fA, const float64_t& fB, const float64_t& fEpsilon = 1e-9);

	/**
	* @brief 比较浮点数比较大小
	* @a 值a
	* @b 值b
	* @epsilon 最小值的精度
	* @return 返回 0 :a=b，1 :a>b ,-1 :a<b
	*/
	int32_t CompareFloats(const float64_t& fA, const float64_t& fB, const float64_t& fEpsilon = 1e-9);

	void SetMainThread();

	bool IsMainThread();

	std::string ToStringWithDecimals(const float64_t& f, const uint32_t & t);

	std::string ToStringWithDecimals(const int32_t& v, const uint32_t & t);

	std::string ToStringWithOutZero(const float64_t &f, const uint32_t & t);

	std::string TimeString(uint64_t uTime);

	// 反转字节序
	template<typename T>
	T SwapEndian(const T &u) {
		static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

		union {
			T u;
			unsigned char u8[sizeof(T)];
		} source, dest;

		source.u = u;

		for (size_t k = 0; k < sizeof(T); k++)
			dest.u8[k] = source.u8[sizeof(T) - k - 1];

		return dest.u;
	}
}