#include "util.h"

#include <random>
#include <ctime>
#include <chrono>
#include <fstream>
#include <thread>
#include <iomanip>
#include <random>
#include <sstream>

#include "text_util.h"

namespace
{
	std::thread::id m_mainThreadID;
}

namespace why
{
	uint16_t ChangeByteOrder(uint16_t data)
	{
		uint32_t			len = sizeof(data);
		uint16_t			ret;
		const uint8_t*		pFrom = (const uint8_t *)&data;
		uint8_t*			pTo = (uint8_t *)&ret;

		for (uint32_t i = 0; i < len; i++)
			pTo[i] = pFrom[len - i - 1];

		return ret;
	}

	uint32_t ChangeByteOrder(uint32_t data)
	{
		uint32_t			len = sizeof(data);
		uint32_t			ret;
		const uint8_t*		pFrom = (const uint8_t *)&data;
		uint8_t*			pTo = (uint8_t *)&ret;

		for (uint32_t i = 0; i < len; i++)
			pTo[i] = pFrom[len - i - 1];

		return ret;
	}

	int32_t GetIZone32()
	{
		time_t		t = time(nullptr);
		tm			localTime;
		tm			utcTime;

		localtime_r(&t, &localTime);
		gmtime_r(&t, &utcTime);
		return localTime.tm_hour - utcTime.tm_hour;
	}

	uint32_t GetTimeStamp32()
	{
		auto	tp = std::chrono::system_clock::now();
		auto	currentSeconds = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();

		return (uint32_t)currentSeconds;
	}

	uint64_t GetTimeStamp64()
	{
		auto	tp = std::chrono::system_clock::now();
		auto	currentMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();

		return (uint64_t)currentMilliseconds;
	}

	uint32_t GetStartTime()
	{
		auto		today = std::chrono::system_clock::now();
		time_t		todayTime = std::chrono::system_clock::to_time_t(today);
		tm			todayTM;

		localtime_r(&todayTime, &todayTM);

		todayTM.tm_hour = 0;
		todayTM.tm_min = 0;
		todayTM.tm_sec = 0;

		time_t	startTime = mktime(&todayTM);
		auto	then_tp = std::chrono::system_clock::from_time_t(startTime);
		auto	currentSeconds = std::chrono::duration_cast<std::chrono::seconds>(then_tp.time_since_epoch()).count();
		return (uint32_t)currentSeconds;
	}

	uint32_t GetEndTime()
	{
		auto		today = std::chrono::system_clock::now();
		time_t		todayTime = std::chrono::system_clock::to_time_t(today);
		tm			todayTM;

		localtime_r(&todayTime, &todayTM);
		todayTM.tm_hour = 23;
		todayTM.tm_min = 59;
		todayTM.tm_sec = 59;

		time_t	endTime = mktime(&todayTM);
		auto	then_tp = std::chrono::system_clock::from_time_t(endTime);
		auto	currentSeconds = std::chrono::duration_cast<std::chrono::seconds>(then_tp.time_since_epoch()).count();
		return (uint32_t)currentSeconds;
	}

	uint32_t ByteAligned(uint32_t count, uint32_t based)
	{
		return (count + (based - count % based));
	}

	std::string ToHex(const uint8_t* pBuf, uint32_t uBufLen)
	{
		std::stringstream		formatStream;

		if ((nullptr != pBuf) && (0 != uBufLen))
		{
			formatStream.fill('0');
			formatStream.width(2);
			formatStream << std::hex << static_cast<int>(pBuf[0]);

			for (size_t i = 1; i < uBufLen; i++)
				formatStream << " " << std::hex << static_cast<int>(pBuf[i]);
		}

		return formatStream.str();
	}

	std::string ToHex(const std::vector<uint8_t> &buf)
	{
		return ToHex(buf.data(), buf.size());
	}

	std::string GenerateNameByTime(const char *pPrefix, const char *pSuffix)
	{
		char 		buf[512] = { 0 };
		auto      	now = std::chrono::system_clock::now();
		auto       	ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()
								   - std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() * 1000;
		auto       	tt = std::chrono::system_clock::to_time_t(now);
		tm			tm;
			
		localtime_r(&tt, &tm);
		snprintf(
				buf,
				512,
				("%s%.4d-%.2d-%.2d[%.2d-%.2d-%.2d-%.3d]%s"),
				pPrefix,
				tm.tm_year + 1900,
				tm.tm_mon + 1,
				tm.tm_mday,
				tm.tm_hour,
				tm.tm_min,
				tm.tm_sec,
				static_cast<int>(ms),
				pSuffix
		);

		return std::string(buf);
	}

	std::string GetDataAndTime()
	{
		auto				now = std::chrono::system_clock::now();
		auto				tt = std::chrono::system_clock::to_time_t(now);
		tm					tm;
		std::stringstream	strOutput;

		localtime_r(&tt, &tm);
		strOutput << std::setw(4) << std::setfill('0') << tm.tm_year + 1900 << "-" << std::setw(2)
			<< tm.tm_mon+1 << "-" << std::setw(2) << tm.tm_mday << " " << std::setw(2) << tm.tm_hour << ":" << std::setw(2) << tm.tm_min << ":" << std::setw(2) << tm.tm_sec;
	
		return strOutput.str();
	}

	int32_t GenerateRandomNumber(int32_t nMin, int32_t nMax)
	{
		static std::random_device rd;  // 用于获得种子
		static std::mt19937 gen(rd()); // 以随机设备生成种子初始化Mersenne Twister生成器
		std::uniform_int_distribution<> dis(nMin, nMax); // 定义分布范围
		return dis(gen); // 返回一个在指定范围内的随机数
	}

	float64_t GenerateRandomFloat64(float64_t fMin, float64_t fMax)
	{
		static std::random_device rd;  // 用于获得种子
		static std::mt19937 gen(rd()); // 以随机设备生成种子初始化Mersenne Twister生成器
		std::uniform_real_distribution<> dis(fMin, fMax); // 定义分布范围
		return dis(gen); // 返回一个在指定范围内的随机数
	}
	int32_t FormatValue(int32_t nMin, int32_t nMax, int32_t nInc, int32_t nValue)
	{
		int32_t		nReturn = nValue;

		if (0 != nInc)
			nReturn = (nReturn / nInc) * nInc;

		if (nReturn < nMin)
			nReturn = nMin;

		if (nReturn > nMax)
			nReturn = nMax;

		return nReturn;
	}
	
	bool AreEqual(const float64_t& fA, const float64_t& fB, const float64_t& fEpsilon) 
	{
		return std::fabs(fA - fB) < fEpsilon;
	}

	int32_t CompareFloats(const float64_t& fA, const float64_t& fB, const float64_t& fEpsilon) 
	{
		if (AreEqual(fA, fB, fEpsilon)) 
			return 0;
		
		return (fA < fB) ? -1 : 1;
	}

	void SetMainThread()
	{
		m_mainThreadID = std::this_thread::get_id();
	}

	bool IsMainThread()
	{
		return (m_mainThreadID == std::this_thread::get_id());
	}

	std::string ToStringWithDecimals(const float64_t& f, const uint32_t & t)
	{
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(t) << f;
		return oss.str();
	}

	std::string ToStringWithDecimals(const int32_t& v, const uint32_t & t)
	{
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(t) << v;
		return oss.str();
	}

	std::string ToStringWithOutZero(const float64_t &f, const uint32_t & t)
	{
		std::string x = ToStringWithDecimals(f, t);

		RemoveTail(x, "0");
		RemoveTail(x, ".");

		return x;
	}

	std::string TimeString(uint64_t uTime)
	{
		uTime = uTime / 1000;

		uint64_t uSecond = uTime % 60;
		uTime -= uSecond;

		uint64_t uMinute = uTime % 3600;
		uTime -= uMinute;
		uMinute = uMinute / 60;

		uint64_t uHour = uTime / 3600;

		std::stringstream ss;
		ss << std::setw(2) << std::setfill('0') << uHour << ":"
			<< std::setw(2) << std::setfill('0') << uMinute << ":"
			<< std::setw(2) << std::setfill('0') << uSecond;

		return ss.str();
	}
}
