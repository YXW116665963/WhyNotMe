#pragma once

#include "standard.h"

#define GLOG_NO_ABBREVIATED_SEVERITIES
#pragma warning(push)
#pragma warning(disable:4251 4244 4996)
#include "glog/logging.h"
#include "glog/raw_logging.h"
#pragma warning(pop)

namespace why
{
	class Logger
	{
	public:
		Logger(const std::string &strLogPath, const std::string &strLogName, int32_t nLogLevel);
		~Logger();
	};
}

#define LOG_DEBUG VLOG(1)
#define LOG_INFO LOG(INFO)
#define LOG_WARNING LOG(WARNING)
#define LOG_ERROR LOG(ERROR)
#define LOG_FATAL LOG(FATAL)